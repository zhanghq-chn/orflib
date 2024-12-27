/**
@file  pyfunctions3.hpp
@brief Implementation of Python callable functions
*/
#include <pyorflib/pyutils.hpp>

#include <orflib/defines.hpp>
#include <orflib/products/europeancallput.hpp>
#include <orflib/products/digitalcallput.hpp>
#include <orflib/products/asianbasketcallput.hpp>
#include <orflib/products/worstofdigitalcallput.hpp>
#include <orflib/pricers/bsmcpricer.hpp>
#include <orflib/pricers/multiassetbsmcpricer.hpp>
#include <orflib/math/stats/meanvarcalculator.hpp>
#include <orflib/math/random/rng.hpp>

using namespace std;
using namespace orf;

static
PyObject*  pyOrfEuroBSMC(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyPayoffType(NULL);
  PyObject* pySpot(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyDiscountCrv(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);
  PyObject* pyMcParams(NULL);
  PyObject* pyNPaths(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOOOO", &pyPayoffType, &pyStrike, &pyTimeToExp, 
    &pySpot, &pyDiscountCrv, &pyDivYield, &pyVolatility, &pyMcParams, &pyNPaths))
    return NULL;

  int payoffType = asInt(pyPayoffType);
  double spot = asDouble(pySpot);
  double strike = asDouble(pyStrike);
  double timeToExp = asDouble(pyTimeToExp);

  std::string name = asString(pyDiscountCrv);
  orf::SPtrYieldCurve spyc = orf::market().yieldCurves().get(name);
  ORF_ASSERT(spyc, "error: yield curve " + name + " not found");

  double divYield = asDouble(pyDivYield);
  // read volatility, either number or term structure
  orf::SPtrVolatilityTermStructure spvol;
  if (isString(pyVolatility)) { // check if input is an object name
    std::string volname = asString(pyVolatility);
    spvol = orf::market().volatilities().get(volname);
  }
  else { // assume real number
    double vol = asDouble(pyVolatility);
    spvol.reset(new orf::VolatilityTermStructure(&timeToExp, &timeToExp + 1,
      &vol, &vol + 1));
  }

  // read the MC parameters
  orf::McParams mcparams = asMcParams(pyMcParams);
  // read the number of paths
  unsigned long npaths = asInt(pyNPaths);

  // create the product
  orf::SPtrProduct spprod(new orf::EuropeanCallPut(payoffType, strike, timeToExp));
  // create the pricer
  orf::BsMcPricer bsmcpricer(spprod, spyc, divYield, spvol, spot, mcparams);
  // create the statistics calculator
  orf::MeanVarCalculator<double *> sc(bsmcpricer.nVariables());
  // run the simulation
  bsmcpricer.simulate(sc, npaths);
  // collect results
  orf::Matrix const& results = sc.results();
  // read out results
  size_t nsamples = sc.nSamples();
  double mean = results(0, 0);
  double stderror = results(1, 0);
  stderror = std::sqrt(stderror / nsamples);

  // write mean and standard error into a Python dictionary
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Mean"), asPyScalar(mean));
  PyDict_SetItem(ret, asPyScalar("StdErr"), asPyScalar(stderror));
  return ret;

PY_END;
}

static
PyObject*  pyOrfSobolSeq(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyNPoints(NULL);
  PyObject* pyNDims(NULL);
  PyObject* pyDistrib(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOO", &pyNPoints, &pyNDims, &pyDistrib))
    return NULL;

  size_t npts = asInt(pyNPoints);
  size_t ndim = asInt(pyNDims);
  int distrib = asInt(pyDistrib);

  std::shared_ptr<orf::SobolURng> urng(std::make_shared<orf::SobolURng>(ndim));
  std::shared_ptr<orf::NormalRngSobol> nrng(std::make_shared<orf::NormalRngSobol>(ndim));
  orf::Matrix seq(npts, ndim);

  if (distrib == 0) {
    for (size_t i = 0; i < npts; ++i) {
      urng->next(seq.begin_row(i), seq.end_row(i));
    }
  }
  else if (distrib == 1) {
    for (size_t i = 0; i < npts; ++i) {
      nrng->next(seq.begin_row(i), seq.end_row(i));
    }
  }

  return asNumpy(seq);
PY_END;
}
