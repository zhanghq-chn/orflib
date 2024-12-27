/**
@file  bermudancallput.hpp
@brief The payoff of a Bermudan Call/Put option
*/

#ifndef ORF_BERMUDANCALLPUT_HPP
#define ORF_BERMUDANCALLPUT_HPP

#include <orflib/products/europeancallput.hpp>

BEGIN_NAMESPACE(orf)

/** The Bermudan call/put class
*/
class BermudanCallPut : public EuropeanCallPut
{
public:
  /** Initializing ctor */
  BermudanCallPut(int payoffType, double strike, Vector const& timesToExer);

  /** Evaluates the product at fixing time index idx
  */
  virtual void eval(size_t idx, Vector const& pricePath, double contValue);

};

///////////////////////////////////////////////////////////////////////////////
// Inline definitions

inline
BermudanCallPut::BermudanCallPut(int payoffType, double strike, Vector const& timesToExer)
: EuropeanCallPut(payoffType, strike, timesToExer[timesToExer.size() - 1])
{
  // count the number of days between 0 and timeToExp
  size_t nfixings = timesToExer.size();
  ORF_ASSERT(nfixings > 0, "BermudanCallPut: no exercise times!");
  ORF_ASSERT(timesToExer[0] >= 0.0, "BermudanCallPut: exercise time in the past!");
  fixTimes_.resize(nfixings);
  fixTimes_[0] = timesToExer[0];
  for (size_t i = 1; i < nfixings; ++i) {
    ORF_ASSERT(timesToExer[i] >= 0.0, "BermudanCallPut: exercise time in the past!");
    ORF_ASSERT(timesToExer[i] > timesToExer[i - 1], "BermudanCallPut: exercise times out of order or duplicated!");
    fixTimes_[i] = timesToExer[i];
  }

  payTimes_ = fixTimes_;

  // this product could generate a payment on each exercise time
  payAmounts_.resize(payTimes_.size());
}

// This product has as many fixings as days between 0 and time to expiration.
inline void BermudanCallPut::eval(size_t idx, Vector const& spots, double contValue)
{
  double spot = spots[0];

  if (idx == payAmounts_.size() - 1) { // this is the last index
    double payoff = (spot - strike_) * payoffType_;
    payAmounts_[idx] = payoff > 0.0 ? payoff : 0.0;
  }
  else {  // this is not the last index, check the exercise condition
    double intrinsicValue = (spot - strike_) * payoffType_;
    intrinsicValue = intrinsicValue >= 0.0 ? intrinsicValue : 0.0;
    payAmounts_[idx] = contValue >= intrinsicValue ? contValue : intrinsicValue;
    // zero out the amounts after this index
    for (size_t j = idx + 1; j < payAmounts_.size(); ++j)
      payAmounts_[j] = 0.0;
  }
}

END_NAMESPACE(orf)

#endif // ORF_BERMUDANCALLPUT_HPP
