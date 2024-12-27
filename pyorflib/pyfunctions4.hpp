/**
@file  pyfunctions4.hpp
@brief Implementation of Python callable functions
*/
#include <pyorflib/pyutils.hpp>

#include <orflib/market/market.hpp>
#include <orflib/products/europeancallput.hpp>
#include <orflib/products/digitalcallput.hpp>
#include <orflib/products/americancallput.hpp>
#include <orflib/products/bermudancallput.hpp>
#include <orflib/methods/pde/pde1dsolver.hpp>

using namespace std;
using namespace orf;

static
PyObject*  pyOrfEuroBSPDE(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyPayoffType(NULL);
  PyObject* pySpot(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyDiscountCrv(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);
  PyObject* pyPdeParams(NULL);
  PyObject* pyAllResults(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOOOO", &pyPayoffType, &pyStrike, &pyTimeToExp, 
    &pySpot, &pyDiscountCrv, &pyDivYield, &pyVolatility, &pyPdeParams, &pyAllResults))
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

  // read the PDE parameters
  orf::PdeParams pdeparams = asPdeParams(pyPdeParams);
  // read the allresults flag
  bool allresults = asBool(pyAllResults);

  // create the product
  orf::SPtrProduct spprod(new orf::EuropeanCallPut(payoffType, strike, timeToExp));
  // create the PDE solver
  orf::Pde1DResults results;
  orf::Pde1DSolver solver(spprod, spyc, spot, divYield, spvol, results);
  solver.solve(pdeparams);

  // write results
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Price"), asPyScalar(results.prices[0]));

  if (allresults) {
    Vector spots;
    results.getSpotAxis(0, spots);
    Matrix values(results.times.size(), results.values.front().size());
    for (size_t i = 0; i < results.times.size(); ++i)
      for (size_t j = 0; j < results.values.front().size(); ++j)
        values(i, j) = results.values[i](j, 0);

    PyDict_SetItem(ret, asPyScalar("Times"), asNumpy(results.times));
    PyDict_SetItem(ret, asPyScalar("Spots"), asNumpy(spots));
    PyDict_SetItem(ret, asPyScalar("Values"), asNumpy(values));
  }
  return ret;

PY_END;
}

static
PyObject*  pyOrfAmerBSPDE(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyPayoffType(NULL);
  PyObject* pySpot(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyDiscountCrv(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);
  PyObject* pyPdeParams(NULL);
  PyObject* pyAllResults(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOOOO", &pyPayoffType, &pyStrike, &pyTimeToExp, 
    &pySpot, &pyDiscountCrv, &pyDivYield, &pyVolatility, &pyPdeParams, &pyAllResults))
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

  // read the PDE parameters
  orf::PdeParams pdeparams = asPdeParams(pyPdeParams);
  // read the allresults flag
  bool allresults = asBool(pyAllResults);

  // create the product
  SPtrProduct spprod(new AmericanCallPut(payoffType, strike, timeToExp));
  // create the PDE solver
  Pde1DResults results;
  bool storeAllResults = true;
  Pde1DSolver solver(spprod, spyc, spot, divYield, spvol, results, storeAllResults);
  solver.solve(pdeparams);

  // write results
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Price"), asPyScalar(results.prices[0]));

  if (allresults) {
    Vector spots;
    results.getSpotAxis(0, spots);
    Matrix values(results.times.size(), results.values.front().size());
    for (size_t i = 0; i < results.times.size(); ++i)
      for (size_t j = 0; j < results.values.front().size(); ++j)
        values(i, j) = results.values[i](j, 0);

    PyDict_SetItem(ret, asPyScalar("Times"), asNumpy(results.times));
    PyDict_SetItem(ret, asPyScalar("Spots"), asNumpy(spots));
    PyDict_SetItem(ret, asPyScalar("Values"), asNumpy(values));
  }
  return ret;

PY_END;
}
