#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include "market-cit.h"
#include "account.h"
#include "config.h"

void lockAccountMoney(Account *a, long unsigned int money)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	a->money = applyPrecision(applyPrecision(a->money) - applyPrecision(money));
	a->lmoney = applyPrecision(applyPrecision(a->lmoney) + applyPrecision(money));
}

void unlockAccountMoney(Account *a, long unsigned int money)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	a->money = applyPrecision(applyPrecision(a->money) + applyPrecision(money));
	a->lmoney = applyPrecision(applyPrecision(a->lmoney) - applyPrecision(money));
}

void lockAccountSnk(Account *a, long unsigned int snk)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	a->snk = applyPrecision(applyPrecision(a->snk) - applyPrecision(snk));
	a->lsnk = applyPrecision(applyPrecision(a->lsnk) + applyPrecision(snk));
}

void unlockAccountSnk(Account *a, long unsigned int snk)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	a->snk = applyPrecision(applyPrecision(a->snk) + applyPrecision(snk));
	a->lsnk = applyPrecision(applyPrecision(a->lsnk) - applyPrecision(snk));
}

void setAccountMoney(Account *a, long unsigned int money)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	a->money = applyPrecision(money);
}

long unsigned int getAccountMoney(Account *a)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	return a->money;
}

void setAccountSnk(Account *a, long unsigned int snk)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	a->snk = applyPrecision(snk);
}

long unsigned int getAccountSnk(Account *a)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	return a->snk;
}

void printAccount(Account *a)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	if ( a != 0x0 ) {
		printf("\t%p [M(L): %11lu (%11lu), S(L): %11lu(%11lu), LB: %11lu, LS: %11lu]\n", (void*)a, a->money, a->lmoney, a->snk, a->lsnk, a->last_price_bought, a->last_price_sold);
	}
	else {
		printf("Not a valid account");
	}
}
