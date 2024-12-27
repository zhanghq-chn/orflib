/**
@file  ptpricers.cpp
@brief Implementation of portfolio pricing functions
*/

#include <orflib/pricers/ptpricers.hpp>
#include <orflib/math/linalg/linalg.hpp>
#include <orflib/utils.hpp>
#include <cmath>

using namespace arma;

BEGIN_NAMESPACE(orf)

/** Validates the inputs and returns the number of assets */
size_t validatePtInputs(Vector const& assetRets, Vector const& assetVols, Matrix const& correlMat)
{
  // get the number of assets and check inputs for size
  size_t nassets = assetRets.size();
  ORF_ASSERT(nassets > 0, "must have at least one asset return!");
  ORF_ASSERT(assetVols.size() == nassets, "need as many volatilities as asset returns!");
  for (size_t i = 0; i < nassets; ++i)
    ORF_ASSERT(assetVols[i] > 0.0, "volatilities must be positive!");
  ORF_ASSERT(correlMat.n_rows == nassets, "need as many correlation matrix rows as asset returns!");
  ORF_ASSERT(correlMat.n_cols == nassets, "the correlation matrix must be square!");
  for (size_t i = 0; i < nassets; ++i) {
    for (size_t j = 0; j < i; ++j) {
      ORF_ASSERT(correlMat(i, j) == correlMat(j, i), "the correlation matrix must be symmetric!");
    }
  }
  return nassets;
}

/** The mean return and standard deviation of returns for the portfolio.
    The first element of the tuple is the mean and the second is the stdev
*/
std::tuple<double, double> ptRisk(Vector const& weights,
                                  Vector const& assetRets,
                                  Vector const& assetVols,
                                  Matrix const& correlMat)
{
  size_t nassets = validatePtInputs(assetRets, assetVols, correlMat);
  ORF_ASSERT(weights.size() == nassets, "number of weights must equal the number of asset returns");

  Matrix Sigma = correlMat;
  // convert correlation to covariance matrix
  for (size_t i = 0; i < nassets; ++i)
    for (size_t j = 0; j < nassets; ++j)
      Sigma(i, j) *= assetVols(i) * assetVols(j);

  double meanret = dot(weights, assetRets);
  double ptvar = dot(weights, Sigma * weights);
  double ptvol = sqrt(ptvar);
  return std::make_tuple(meanret, ptvol);
}

/** The weights of the minimum variance portfolio */
Vector mvpWeights(Vector const& assetRets, Vector const& assetVols, Matrix const& correlMat)
{
  size_t nassets = validatePtInputs(assetRets, assetVols, correlMat);
  Vector iota(nassets, fill::ones);
  Matrix SigmaInv = correlMat;
  // convert correlation to covariance matrix
  for (size_t i = 0; i < SigmaInv.n_rows; ++i)
    for (size_t j = 0; j < SigmaInv.n_cols; ++j)
      SigmaInv(i, j) *= assetVols(i) * assetVols(j);

  SigmaInv = SigmaInv.i();   // invert in place
  Vector wghts = SigmaInv * iota;
  double c = dot(iota, wghts);
  wghts = (1.0 / c) * wghts;
  return wghts;
}


/** The weights of the CAPM market portfolio */
Vector mktWeights(Vector const& assetRets, Vector const& assetVols, Matrix const& correlMat, double rfreeRate)
{
  size_t nassets = validatePtInputs(assetRets, assetVols, correlMat);
  Vector iota(nassets, fill::ones);
  Matrix SigmaInv = correlMat;
  // convert to covariance matrix
  for (size_t i = 0; i < SigmaInv.n_rows; ++i)
    for (size_t j = 0; j < SigmaInv.n_cols; ++j)
      SigmaInv(i, j) *= assetVols(i) * assetVols(j);

  SigmaInv = SigmaInv.i();   // invert in place
  Vector wghts = SigmaInv * (assetRets - rfreeRate *iota);
  double lambda_mkt = 1.0 / dot(iota, wghts);
  wghts = lambda_mkt * wghts;
  return wghts;
}

/** The weights of a mean-variance optimal portfolio */
Vector meanVarWeights(Vector const& assetRets, Vector const& assetVols, Matrix const& correlMat, double lambda)
{
  size_t nassets = validatePtInputs(assetRets, assetVols, correlMat);
  ORF_ASSERT(lambda >= 0.0, "lambda must be positive!");

  Vector iota(nassets, fill::ones);
  Matrix SigmaInv = correlMat;
  // convert correlation to covariance matrix
  for (size_t i = 0; i < SigmaInv.n_rows; ++i)
    for (size_t j = 0; j < SigmaInv.n_cols; ++j)
      SigmaInv(i, j) *= assetVols(i) * assetVols(j);

  SigmaInv = SigmaInv.i();   // invert in place
  Vector wghts = SigmaInv * iota;
  Vector wghmu = SigmaInv * assetRets;

  double c = dot(iota, wghts);
  double a = dot(iota, wghmu);

  wghts = (1.0 / c) * wghts + lambda * (wghmu - (a / c) * wghts);
  return wghts;
}

/** The mean return, volatility of return and lambda of the CAPM market portfolio
    The tuple contains these quantities in that order.
*/
std::tuple<double, double, double>
mktRisk(Vector const& assetRets, Vector const& assetVols, Matrix const& correlMat, double rfreeRate)
{
  size_t nassets = validatePtInputs(assetRets, assetVols, correlMat);
  Vector iota(nassets, fill::ones);
  Matrix SigmaInv = correlMat;
  // convert to covariance matrix
  for (size_t i = 0; i < SigmaInv.n_rows; ++i)
    for (size_t j = 0; j < SigmaInv.n_cols; ++j)
      SigmaInv(i, j) *= assetVols(i) * assetVols(j);

  SigmaInv = SigmaInv.i();   // invert in place
  Vector wghts = SigmaInv * (assetRets - rfreeRate * iota);
  double lambda_mkt = 1.0 / dot(iota, wghts);
  double h = dot((assetRets - rfreeRate *iota), wghts);

  double meanret = rfreeRate + lambda_mkt * h;
  double stdevret = lambda_mkt * sqrt(h);
  return std::make_tuple(meanret, stdevret, lambda_mkt);
}

/** The mean-variance efficient frontier */
std::tuple<Vector, Vector, Vector>
meanVarFrontier(Vector const& assetRets, Vector const& assetVols, Matrix const& correlMat, 
                double lambdaMax, int nLambdaSteps)
{
  size_t nassets = validatePtInputs(assetRets, assetVols, correlMat);
  ORF_ASSERT(lambdaMax >= 0.0, "lambda must be positive!");
  ORF_ASSERT(nLambdaSteps >= 0.0, "lambda must be positive!");

  double dlambda = lambdaMax / nLambdaSteps;
  Vector iota(nassets, fill::ones);
  Matrix SigmaInv = correlMat;
  // convert correlation to covariance matrix
  for (size_t i = 0; i < SigmaInv.n_rows; ++i)
    for (size_t j = 0; j < SigmaInv.n_cols; ++j)
      SigmaInv(i, j) *= assetVols(i) * assetVols(j);

  SigmaInv = SigmaInv.i();   // invert in place
  Vector sigi = SigmaInv * iota;
  Vector sigmu = SigmaInv * assetRets;
  double c = dot(iota, sigi);
  double a = dot(iota, sigmu);
  double b = dot(assetRets, sigmu);
  double d = b * c - a * a;
  Vector wghts(nassets);

  Vector ptrets(nLambdaSteps + 1), ptvols(nLambdaSteps + 1), lambdas(nLambdaSteps + 1);
  for (size_t i = 0; i < (size_t) nLambdaSteps; ++i) {
    double lambda = i * dlambda;
    wghts = (1.0 / c) * sigi + lambda * (sigmu - (a / c) * sigi);
    ptrets[i] = dot(wghts, assetRets);
    ptvols[i] = (c / d) * pow(ptrets[i] - (a / c), 2.0) + (1.0 / c);
    ptvols[i] = sqrt(ptvols[i]);
    lambdas[i] = lambda;
  }
  // and the max lambda value
  wghts = (1.0 / c) * sigi + lambdaMax * (sigmu - (a / c) * sigi);
  ptrets[nLambdaSteps] = dot(wghts, assetRets);
  ptvols[nLambdaSteps] = (c / d) * pow(ptrets[nLambdaSteps] - (a / c), 2.0) + (1.0 / c);
  ptvols[nLambdaSteps] = sqrt(ptvols[nLambdaSteps]);
  lambdas[nLambdaSteps] = lambdaMax;

  return std::make_tuple(ptrets, ptvols, lambdas);
}

END_NAMESPACE(orf)
