#ifndef _MARKET_CIT_H_
#define _MARKET_CIT_H_
#include "account.h"
#include "offer.h"

// This was about double and floats but it got very wrong
// so we are using integers anymore.
long unsigned int applyPrecision(long unsigned int);

/*
 * Since we overcame the problem with doubles not being accurate, we
 * converted every value (money, snk) to integers. However, life includes
 * values that are decimal, so in order to present them we are converting
 * them to doubles.
 */
double qcid(long unsigned int);
// Reverse job (to convert eg input)
long unsigned int qcdi(double);

// Finally, tests!
void run_test(Account*, Account*, Offer**, long unsigned int*);

#endif // _MARKET_CIT_H_
