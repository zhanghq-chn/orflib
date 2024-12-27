/**
@file  pyfunctions5.hpp
@brief Implementation of Python callable functions
*/
#include <pyorflib/pyutils.hpp>

#include <orflib/pricers/ptpricers.hpp>
#include <cmath>

using namespace std;
using namespace orf;

static
PyObject*  pyOrfPtRisk(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyPtWghts(NULL);
  PyObject* pyAssetRets(NULL);
  PyObject* pyAssetVols(NULL);
  PyObject* pyCorrelMat(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOO", &pyPtWghts, &pyAssetRets, 
    &pyAssetVols, &pyCorrelMat))
    return NULL;

  Vector ptwghts = asVector(pyPtWghts);
  Vector assetRets = asVector(pyAssetRets);
  Vector assetVols = asVector(pyAssetVols);
  Matrix correlMat = asMatrix(pyCorrelMat);

  std::tuple<double, double> res = ptRisk(ptwghts, assetRets, assetVols, correlMat);

  // write mean and standard deviation into a Python dictionary
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Mean"), asPyScalar(get<0>(res)));
  PyDict_SetItem(ret, asPyScalar("StdDev"), asPyScalar(get<1>(res)));
  return ret;

PY_END;
}

static
PyObject* pyOrfMvpWghts(PyObject* pyDummy, PyObject* pyArgs)
{
  PY_BEGIN;

  PyObject* pyAssetRets(NULL);
  PyObject* pyAssetVols(NULL);
  PyObject* pyCorrelMat(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOO", &pyAssetRets, &pyAssetVols, &pyCorrelMat))
    return NULL;

  Vector assetRets = asVector(pyAssetRets);
  Vector assetVols = asVector(pyAssetVols);
  Matrix correlMat = asMatrix(pyCorrelMat);

  Vector wghts = mvpWeights(assetRets, assetVols, correlMat);

  return asNumpy(wghts);

  PY_END;
}
