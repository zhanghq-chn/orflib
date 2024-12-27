/**
@file  pyutils.hpp
@brief Utility functions for Python3 to orflib bidirectional type translation.
*/

#ifndef PYORFLIB_PYUTILS_HPP
#define PYORFLIB_PYUTILS_HPP

#include <orflib/utils.hpp>
#include <orflib/math/matrix.hpp>
#include <orflib/methods/montecarlo/mcparams.hpp>
#include <orflib/methods/pde/pdeparams.hpp>
#include <pyorflib/pycpp.hpp>   // NOTE: include the python headers last (before armadillo)

/** Converts a numpy 1-D array to an orf::Vector.
*/
static orf::Vector asVector(PyObject* pyVec)
{
  return orf::Vector(asDblVec(pyVec));
}

/** Converts an orf::Vector to a numpy vector
*/
static PyObject* asNumpy(orf::Vector const & vec)
{
  return asPyArray(arma::conv_to<std::vector<double>>::from(vec));
}

/** Converts a numpy 2-D array to an orf::Matrix.
*/
static orf::Matrix asMatrix(PyObject* pyMat)
{
  std::vector<std::vector<double>> dvecvec = asDblVecVec(pyMat);
  size_t nrows = dvecvec.size();
  size_t ncols = dvecvec.front().size();
  orf::Matrix mat(nrows, ncols);
  for (size_t i = 0; i < nrows; ++i)
    for (size_t j = 0; j < ncols; ++j)
      mat(i, j) = dvecvec[i][j];
  return mat;
}

/** Converts an orf::Matrix to numpy
*/
static PyObject* asNumpy(orf::Matrix const & mat)
{
  size_t nrows = mat.n_rows;
  size_t ncols = mat.n_cols;
  std::vector<std::vector<double>> dvecvec(nrows);
  for (size_t i = 0; i < nrows; ++i) {
    dvecvec[i].resize(ncols);
    for (size_t j = 0; j < ncols; ++j)
      dvecvec[i][j] = mat(i, j);
  }
  return asPyArray(dvecvec);
}

/** Converts a Python dictionary with name-value pairs to an McParams structure.
*/
static orf::McParams asMcParams(PyObject* dict)
{
  ORF_ASSERT(PyDict_Check(dict) == 1, "asMcParams: input param must be a dictionary");

  orf::McParams mcparams;
  std::string paramvalue;

  std::string paramname = "PATHGENTYPE";
  ORF_ASSERT(PyDict_Contains(dict, asPyScalar(paramname)) == 1, 
    "asMcParams: input dictionary does not contain key PATHGENTYPE");
  paramvalue = asString(PyDict_GetItemString(dict, paramname.c_str()));
  paramvalue = orf::trim(paramvalue);
  std::transform(paramvalue.begin(), paramvalue.end(), paramvalue.begin(), ::toupper);
  if (paramvalue == "EULER")
    mcparams.pathGenType = orf::McParams::PathGenType::EULER;
  else
    ORF_ASSERT(0, "asMcParams: invalid value for McParam " + paramname + "!");

  paramname = "URNGTYPE";
  ORF_ASSERT(PyDict_Contains(dict, asPyScalar(paramname)) == 1,
    "asMcParams: input dictionary does not contain key URNGTYPE");
  paramvalue = asString(PyDict_GetItemString(dict, paramname.c_str()));
  paramvalue = orf::trim(paramvalue);
  std::transform(paramvalue.begin(), paramvalue.end(), paramvalue.begin(), ::toupper);
  if (paramvalue == "MINSTDRAND")
    mcparams.urngType = orf::McParams::UrngType::MINSTDRAND;
  else if (paramvalue == "MT19937")
    mcparams.urngType = orf::McParams::UrngType::MT19937;
  else if (paramvalue == "RANLUX3")
    mcparams.urngType = orf::McParams::UrngType::RANLUX3;
  else if (paramvalue == "RANLUX4")
    mcparams.urngType = orf::McParams::UrngType::RANLUX4;
  else if (paramvalue == "SOBOL")
    mcparams.urngType = orf::McParams::UrngType::SOBOL;
  else
    ORF_ASSERT(0, "asMcParams: invalid value for McParam " + paramname + "!");

  paramname = "CONTROLVARTYPE";
  if (PyDict_Contains(dict, asPyScalar(paramname))) {
    paramvalue = asString(PyDict_GetItemString(dict, paramname.c_str()));
    paramvalue = orf::trim(paramvalue);
    std::transform(paramvalue.begin(), paramvalue.end(), paramvalue.begin(), ::toupper);
    if (paramvalue == "ANTITHETIC")
      mcparams.controlVarType = orf::McParams::ControlVarType::ANTITHETIC;
    else if (paramvalue == "NONE" || paramvalue.empty())
      mcparams.controlVarType = orf::McParams::ControlVarType::NONE; // do nothing
    else
      ORF_ASSERT(0, "asMcParams: unknown McParam " + paramname + "!");
  }
  else {  // list does not contain paramname
    mcparams.controlVarType = orf::McParams::ControlVarType::NONE; // do nothing, default
  }

  return mcparams;
}

/** Converts a Python dictionary with name-value pairs to an PdeParams structure.
*/
static orf::PdeParams asPdeParams(PyObject* dict)
{
  ORF_ASSERT(PyDict_Check(dict) == 1, "asPdeParams: input param must be a dictionary");

  orf::PdeParams pdeparams;
  std::string paramvalue;

  std::string paramname = "NTIMESTEPS";
  ORF_ASSERT(PyDict_Contains(dict, asPyScalar(paramname)) == 1,
    "asPdeParams: input dictionary does not contain key NTIMESTEPS");
  pdeparams.nTimeSteps = (size_t) asInt(PyDict_GetItemString(dict, paramname.c_str()));

  paramname = "NSPOTNODES";
  ORF_ASSERT(PyDict_Contains(dict, asPyScalar(paramname)) == 1,
    "asPdeParams: input dictionary does not contain key NSPOTNODES");
  pdeparams.nSpotNodes[0] = (size_t) asInt(PyDict_GetItemString(dict, paramname.c_str()));

  paramname = "NSTDDEVS";
  ORF_ASSERT(PyDict_Contains(dict, asPyScalar(paramname)) == 1,
    "asPdeParams: input dictionary does not contain key NSTDDEVS");
  pdeparams.nStdDevs[0] = asDouble(PyDict_GetItemString(dict, paramname.c_str()));

  paramname = "THETA";
  ORF_ASSERT(PyDict_Contains(dict, asPyScalar(paramname)) == 1,
    "asPdeParams: input dictionary does not contain key THETA");
  pdeparams.theta = asDouble(PyDict_GetItemString(dict, paramname.c_str()));

  return pdeparams;
}

#endif // PYORFLIB_PYUTILS_HPP
