/** 
@file  pyfunctions0.hpp
@brief Implementation of Python callable functions
*/
#include <pyorflib/pyutils.hpp>

#include <orflib/defines.hpp>
#include <orflib/math/matrix.hpp>
#include <orflib/utils.hpp>
#include <orflib/math/interpol/piecewisepolynomial.hpp>
#include <orflib/math/optim/roots.hpp>
#include <orflib/math/optim/polyfunc.hpp>
#include <orflib/math/linalg/linalg.hpp>

using namespace std;
using namespace orf;

static 
PyObject*  pyOrfVersion(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  return asPyScalar(std::string(ORF_VERSION_STRING));
PY_END;
}

static 
PyObject*  pyOrfMatrixEcho(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyMat(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyMat))
    return NULL;
  orf::Matrix mat = asMatrix(pyMat);
  return asNumpy(mat);
PY_END;
}

static
PyObject*  pyOrfCholDcmp(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyMat(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyMat))
    return NULL;
  orf::Matrix inmat = asMatrix(pyMat);
  orf::Matrix outmat;
  choldcmp(inmat, outmat);
  return asNumpy(outmat);
PY_END;
}

static 
PyObject*  pyOrfEigenSym(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyMat(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyMat))
    return NULL;
  orf::Matrix mat = asMatrix(pyMat);
  Vector eigenvals;
  Matrix eigenvecs;
  orf::eigensym(mat, eigenvals, eigenvecs);

  // write eigenvalues and eigenvectors into a Python dictionary
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Eigenvals"), asNumpy(eigenvals));
  ok = PyDict_SetItem(ret, asPyScalar("Eigenvecs"), asNumpy(eigenvecs));
  return ret;

PY_END;
}

static 
PyObject*  pyOrfSpecTrunc(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyMat(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyMat))
    return NULL;

  orf::Matrix mat = asMatrix(pyMat);
  orf::spectrunc(mat);

  return asNumpy(mat);
PY_END;
}

static 
PyObject*  pyOrfPPolyEval(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyBkPoints(NULL);
  PyObject* pyValues(NULL);
  PyObject* pyPolyOrder(NULL);
  PyObject* pyXVec(NULL);
  PyObject* pyDerivOrder(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOOO", &pyBkPoints, &pyValues, &pyPolyOrder, &pyXVec, &pyDerivOrder))
    return NULL;

  Vector bkpts = asVector(pyBkPoints);
  Vector vals = asVector(pyValues);
  int degree = asInt(pyPolyOrder);
  Vector xx = asVector(pyXVec);
  ORF_ASSERT(bkpts.size() == vals.size(), "unequal number of breakpoints and vals");
  int derivOrder = asInt(pyDerivOrder);

  // create the curve
  PiecewisePolynomial pp(bkpts.begin(), bkpts.end(), vals.begin(), degree);

  Vector yy(xx.size());
  pp.eval(xx.begin(), xx.end(), yy.begin(), derivOrder);

  return asNumpy(yy);
PY_END;
}

static 
PyObject*  pyOrfPPolyIntegral(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyBkPoints(NULL);
  PyObject* pyValues(NULL);
  PyObject* pyPolyOrder(NULL);
  PyObject* pyXStart(NULL);
  PyObject* pyXVecEnd(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOOO", &pyBkPoints, &pyValues, &pyPolyOrder, &pyXStart, &pyXVecEnd))
    return NULL;

  Vector bkpts = asVector(pyBkPoints);
  Vector vals = asVector(pyValues);
  ORF_ASSERT(bkpts.size() == vals.size(), "unequal number of breakpoints and vals");
  int degree = asInt(pyPolyOrder);
  double x0 = asDouble(pyXStart);
  Vector xx = asVector(pyXVecEnd);
  // create the curve
  PiecewisePolynomial pp(bkpts.begin(), bkpts.end(), vals.begin(), degree);
  // integrate
  Vector yy(xx.size());
  pp.integral(x0, xx.begin(), xx.end(), yy.begin());

  return asNumpy(yy);
PY_END;
}

static 
PyObject*  pyOrfPPolySum(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyBkPoints1(NULL);
  PyObject* pyValues1(NULL);
  PyObject* pyBkPoints2(NULL);
  PyObject* pyValues2(NULL);
  PyObject* pyPolyOrder(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOOO", &pyBkPoints1, &pyValues1, &pyBkPoints2, &pyValues2, &pyPolyOrder))
    return NULL;

  Vector bkpts1 = asVector(pyBkPoints1);
  Vector vals1 = asVector(pyValues1);
  ORF_ASSERT(bkpts1.size() == vals1.size(), "unequal number of breakpoints and vals");

  Vector bkpts2 = asVector(pyBkPoints2);
  Vector vals2 = asVector(pyValues2);
  ORF_ASSERT(bkpts2.size() == vals2.size(), "unequal number of breakpoints and vals");

  int order = asInt(pyPolyOrder);
  ORF_ASSERT(order < 2, "only polynomials of order 0 or 1 can be added with this function");

  // create the curves
  PiecewisePolynomial pp1(bkpts1.begin(), bkpts1.end(), vals1.begin(), order);
  PiecewisePolynomial pp2(bkpts2.begin(), bkpts2.end(), vals2.begin(), order);
  // add them
  PiecewisePolynomial psum = pp1 + pp2;
  Vector const& bkpts = psum.breakPoints();
  Vector vals(bkpts.size());
  psum.eval(bkpts.begin(), bkpts.end(), vals.begin(), 0);

  // TODO: return values, too
  return asNumpy(bkpts);
PY_END;
}

static
PyObject*  pyOrfPolyBracket(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyCoeffs(NULL);
  PyObject* pyLowerLimit(NULL);
  PyObject* pyUpperLimit(NULL);
  PyObject* pyNSubs(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOO", &pyCoeffs, &pyLowerLimit, &pyUpperLimit, &pyNSubs))
    return NULL;

  Vector coeffs = asVector(pyCoeffs);
  double xlo = asDouble(pyLowerLimit);
  double xhi = asDouble(pyUpperLimit);
  int nsubs = asInt(pyNSubs);

  Polynomial p(coeffs);
  Vector xb1, xb2;
  int nroot;

  zbrak(p, xlo, xhi, nsubs, xb1, xb2, nroot);

  // TODO: return xb2, too
  return asNumpy(xb1);
PY_END;
}

static
PyObject*  pyOrfPolySecant(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyCoeffs(NULL);
  PyObject* pyLowerLimit(NULL);
  PyObject* pyUpperLimit(NULL);
  PyObject* pyTol(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOO", &pyCoeffs, &pyLowerLimit, &pyUpperLimit, &pyTol))
    return NULL;

  Vector coeffs = asVector(pyCoeffs);
  double xlo = asDouble(pyLowerLimit);
  double xhi = asDouble(pyUpperLimit);
  double tol = asDouble(pyTol);

  Polynomial p(coeffs);
  double root = rtsec(p, xlo, xhi, tol);
  return asPyScalar(root);
PY_END;
}

static
PyObject*  pyOrfToContCmpd(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyRate(NULL);
  PyObject* pyAnnFreq(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OO", &pyRate, &pyAnnFreq))
    return NULL;

  double ret = toContCmpd(asDouble(pyRate), asLong(pyAnnFreq));
  return asPyScalar(ret);
PY_END;
}

static
PyObject*  pyOrfFromContCmpd(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyRate(NULL);
  PyObject* pyAnnFreq(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OO", &pyRate, &pyAnnFreq))
    return NULL;

  double ret = fromContCmpd(asDouble(pyRate), asLong(pyAnnFreq));
  return asPyScalar(ret);
PY_END;
}
