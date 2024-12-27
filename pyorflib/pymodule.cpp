/**
@file  pymodule.cpp
@brief Python callable function registration and module definition
*/

#include "orflib/defines.hpp"
#include "pytestfunc.hpp"
#include "pyfunctions0.hpp"
#include "pyfunctions1.hpp"
#include "pyfunctions2.hpp"
#include "pyfunctions3.hpp"
#include "pyfunctions4.hpp"
#include "pyfunctions5.hpp"

static PyMethodDef PyOrflibMethods[] = 
{
  { "__echoBool__", echoBool, METH_VARARGS, "echoes a boolean" },
  { "__echoInt__", echoInt, METH_VARARGS, "echoes an integer" },
  { "__echoDouble__", echoDouble, METH_VARARGS, "echoes a double" },
  { "__echoString__", echoString, METH_VARARGS, "echoes a string" },
  { "__echoIntList__", echoIntList, METH_VARARGS, "echoes a list of integers" },
  { "__echoDblList__", echoDblList, METH_VARARGS, "echoes a list of doubles" },
  { "__echoStrList__", echoStrList, METH_VARARGS, "echoes a list of strings" },
  { "__echoIntArray__", echoIntArray, METH_VARARGS, "echoes an 1D array of integers" },
  { "__echoDblArray__", echoDblArray, METH_VARARGS, "echoes an 1D array of doubles" },
  { "__echoStrArray__", echoStrArray, METH_VARARGS, "echoes an 1D array of strings" },
  { "__echoDblArray2D__", echoDblArray2D, METH_VARARGS, "echoes a 2D array of doubles" },
  { "__echoDblVecField__", echoDblVecField, METH_VARARGS, "echoes an 1D array of doubnles in an object field" },
// functions 0
  { "version", pyOrfVersion, METH_VARARGS, "orflib library version"},
  { "matrixEcho", pyOrfMatrixEcho, METH_VARARGS, "echoes a matrix" },
  { "cholDcmp", pyOrfCholDcmp, METH_VARARGS, "Cholesky decomposition of symmetric, positive definite matrix" },
  { "eigenSym", pyOrfEigenSym, METH_VARARGS, "eigenvalues/eigenvectors of a real symmetric matrix" },
  { "specTrunc", pyOrfSpecTrunc, METH_VARARGS, "spectral truncation of a read symmetric matrix" },
  { "ppolyEval", pyOrfPPolyEval, METH_VARARGS, "evaluation of derivatives of a piecewise polynomial curve" },
  { "ppolyIntegral", pyOrfPPolyIntegral, METH_VARARGS, "integration of a piecewise polynomial curve" },
  { "ppolySum", pyOrfPPolySum, METH_VARARGS, "sum of two piecewise polynomial curves" },
  { "polyBracket", pyOrfPolyBracket, METH_VARARGS, "bracketing roots of a polynomial" },
  { "polySecant", pyOrfPolySecant, METH_VARARGS, "secant method for roots of a polynomial" },
  { "toContCmpd", pyOrfToContCmpd, METH_VARARGS, "converts rate from periodic to continuous compounding" },
  { "fromContCmpd", pyOrfFromContCmpd, METH_VARARGS, "converts rate from continuous to periodic compounding" },
// functions 1
  { "erf", pyOrfErf, METH_VARARGS, "the value of the error function." },
  { "invErf", pyOrfInvErf, METH_VARARGS, "the value of the inverse error function." },
  { "normalCdf", pyOrfNormalCdf, METH_VARARGS, "the value of the standard normal cumulative distribution." },
  { "normalInvCdf", pyOrfNormalInvCdf, METH_VARARGS, "the value of the inverse standard normal cumulative distribution." },
  { "fwdPrice", pyOrfFwdPrice, METH_VARARGS, "the forward price of an asset." },
  { "digiBS", pyOrfDigiBS, METH_VARARGS, "price of a digital option in the Black-Scholes model." },
  { "euroBS", pyOrfEuroBS, METH_VARARGS, "price and Greeks of a European option in the Black-Scholes model." },
  { "sqeuroBS", pyOrfEuroBS, METH_VARARGS, "price of a Squared European option in the Black-Scholes model." },
  { "kofwd", pyOrfKOFwd, METH_VARARGS, "price of a knock-out forward contract." },
  // functions 2
  { "mktList", pyOrfMktList, METH_VARARGS, "lists all market objects." },
  { "mktClear", pyOrfMktClear, METH_VARARGS, "deletes all market objects." },
  { "ycCreate", pyOrfYCCreate, METH_VARARGS, "creates a yield curve." },
  { "discount", pyOrfDiscount, METH_VARARGS, "discount factor to maturity." },
  { "fwdDiscount", pyOrfFwdDiscount, METH_VARARGS, "fwd discount factor between the two maturities." },
  { "spotRate", pyOrfSpotRate, METH_VARARGS, "spot rate to maturity." },
  { "fwdRate", pyOrfFwdRate, METH_VARARGS, "fwd rate between the two maturities." },
  { "volCreate", pyOrfVolCreate, METH_VARARGS, "creates a volatility curve." },
  { "spotVol", pyOrfSpotVol, METH_VARARGS, "spot volatility to maturity." },
  { "fwdVol", pyOrfFwdVol, METH_VARARGS, "fwd volatility between the two maturities." },
  { "capFloorletBS", pyOrfCapFloorletBS, METH_VARARGS, "present value of a caplet/floorlet on fwd rate." },
  { "cdsPV", pyOrfCDSPV, METH_VARARGS, "present value of a CDS." },
  // functions 3
  { "euroBSMC", pyOrfEuroBSMC, METH_VARARGS, "price of a European option in the Black-Scholes model using Monte Carlo." },
  { "sobolSeq", pyOrfSobolSeq, METH_VARARGS, "sequence of Sobol numbers in various dimensions." },
  // functions 4
  { "euroBSPDE", pyOrfEuroBSPDE, METH_VARARGS, "price of a European option in the Black-Scholes model using PDE." },
  { "amerBSPDE", pyOrfAmerBSPDE, METH_VARARGS, "price of an American option in the Black-Scholes model using PDE." },
  // functions 5
  { "ptRisk", pyOrfPtRisk, METH_VARARGS, "mean return and standard deviation of a portfolio" },
  { "mvpWghts", pyOrfMvpWghts, METH_VARARGS, "weights of the minimum variance portfolio" },
  {NULL, NULL, 0, NULL}
};

// ONLY PYTHON 3

// Definition of module
static struct PyModuleDef modef =
{
  PyModuleDef_HEAD_INIT,
  "orflib",
  "Python interface to the orflib library",
  -1,
  PyOrflibMethods
};

// Creation of module
PyMODINIT_FUNC
PyInit_pyorflib(void)
{
  PyObject* m = PyModule_Create(&modef);
  if (m == NULL)
    return NULL;
  import_array();
  return m;
}
