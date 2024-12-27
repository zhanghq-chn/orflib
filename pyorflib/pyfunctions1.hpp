/**
@file  pyfunctions1.hpp
@brief Implementation of Python callable functions
*/
#include <pyorflib/pyutils.hpp>

#include <orflib/defines.hpp>
#include <orflib/math/stats/errorfunction.hpp>
#include <orflib/math/stats/normaldistribution.hpp>
#include <orflib/pricers/simplepricers.hpp>

using namespace std;
using namespace orf;

static
PyObject*  pyOrfErf(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyX(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyX))
    return NULL;

  double x = asDouble(pyX);
  return asPyScalar(ErrorFunction::erf(x));
PY_END;
}

static
PyObject*  pyOrfInvErf(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyX(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyX))
    return NULL;

  double x = asDouble(pyX);
  return asPyScalar(ErrorFunction::inverf(x));
PY_END;
}

static
PyObject*  pyOrfNormalCdf(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyX(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyX))
    return NULL;

  double x = asDouble(pyX);
  NormalDistribution f;

  return asPyScalar(f.cdf(x));
PY_END;
}

static
PyObject*  pyOrfNormalInvCdf(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyX(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyX))
    return NULL;

  double x = asDouble(pyX);
  NormalDistribution f;

  return asPyScalar(f.invcdf(x));
PY_END;
}

static
PyObject*  pyOrfFwdPrice(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pySpot(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyIntRate(NULL);
  PyObject* pyDivYield(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOO", &pySpot, &pyTimeToExp, &pyIntRate, &pyDivYield))
    return NULL;

  double spot = asDouble(pySpot);
  double timeToExp = asDouble(pyTimeToExp);
  double intRate = asDouble(pyIntRate);
  double divYield = asDouble(pyDivYield);

  double fwd = fwdPrice(spot, timeToExp, intRate, divYield);

  return asPyScalar(fwd);
PY_END;
}

static
PyObject*  pyOrfDigiBS(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyPayoffType(NULL);
  PyObject* pySpot(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyIntRate(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOO", &pyPayoffType, &pySpot, &pyStrike,
    &pyTimeToExp, &pyIntRate, &pyDivYield, &pyVolatility))
    return NULL;

  int payoffType = asInt(pyPayoffType);
  double spot = asDouble(pySpot);
  double strike = asDouble(pyStrike);
  double timeToExp = asDouble(pyTimeToExp);
  double intRate = asDouble(pyIntRate);
  double divYield = asDouble(pyDivYield);
  double vol = asDouble(pyVolatility);

  Vector greeks = digitalOptionBS(payoffType, spot, strike, timeToExp, intRate, divYield, vol);

  // write price and Greeks into a Python dictionary
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Price"), asPyScalar(greeks[0]));
  PyDict_SetItem(ret, asPyScalar("Delta"), asPyScalar(greeks[1]));
  PyDict_SetItem(ret, asPyScalar("Gamma"), asPyScalar(greeks[2]));
  return ret;
PY_END;
}

static
PyObject*  pyOrfEuroBS(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyPayoffType(NULL);
  PyObject* pySpot(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyIntRate(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOO", &pyPayoffType, &pySpot, &pyStrike,
    &pyTimeToExp, &pyIntRate, &pyDivYield, &pyVolatility))
    return NULL;


  int payoffType = asInt(pyPayoffType);
  double spot = asDouble(pySpot);
  double strike = asDouble(pyStrike);
  double timeToExp = asDouble(pyTimeToExp);
  double intRate = asDouble(pyIntRate);
  double divYield = asDouble(pyDivYield);
  double vol = asDouble(pyVolatility);

  Vector greeks = europeanOptionBS(payoffType, spot, strike, timeToExp,
    intRate, divYield, vol);

  // write price and Greeks into a Python dictionary
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Price"), asPyScalar(greeks[0]));
  PyDict_SetItem(ret, asPyScalar("Delta"), asPyScalar(greeks[1]));
  PyDict_SetItem(ret, asPyScalar("Gamma"), asPyScalar(greeks[2]));
  return ret;
PY_END;
}

static
PyObject* pyOrfSqEuroBS(PyObject* pyDummy, PyObject* pyArgs)
{
  PY_BEGIN;

  PyObject* pyPayoffType(NULL);
  PyObject* pySpot(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyIntRate(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOO", &pyPayoffType, &pySpot, &pyStrike,
    &pyTimeToExp, &pyIntRate, &pyDivYield, &pyVolatility))
    return NULL;


  int payoffType = asInt(pyPayoffType);
  double spot = asDouble(pySpot);
  double strike = asDouble(pyStrike);
  double timeToExp = asDouble(pyTimeToExp);
  double intRate = asDouble(pyIntRate);
  double divYield = asDouble(pyDivYield);
  double vol = asDouble(pyVolatility);

  double price = squaredOptionBS(payoffType, spot, strike, timeToExp,
    intRate, divYield, vol);

  // return price
  return asPyScalar(price);
  PY_END;
}

static
PyObject* pyOrfKOFwd(PyObject* pyDummy, PyObject* pyArgs)
{
  PY_BEGIN;

  PyObject* pySpot(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyKOLevel(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyTimeToKO(NULL);
  PyObject* pyIntRate(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOO)", &pySpot, &pyStrike, &pyKOLevel,
    &pyTimeToExp, &pyTimeToKO , &pyIntRate, &pyDivYield, &pyVolatility))
    return NULL;


  double spot = asDouble(pySpot);
  double strike = asDouble(pyStrike);
  double kolevel = asDouble(pyKOLevel);
  double timeToExp = asDouble(pyTimeToExp);
  double timeToKO = asDouble(pyTimeToKO);
  double intRate = asDouble(pyIntRate);
  double divYield = asDouble(pyDivYield);
  double vol = asDouble(pyVolatility);

  double price = knockoutFwd(spot, strike, kolevel, timeToExp, timeToKO,
    intRate, divYield, vol);

  // return price
  return asPyScalar(price);
  PY_END;
}
