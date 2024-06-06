#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include "account.h"
#include "offer.h"
#include "market-cit.h"
#include "config.h"

void run_test(Account *a, Account *b, Offer **offers, long unsigned int *s){
//	(void)b;
	long unsigned int price, amount = 0;

	Account onle;
	onle.money = INT_MAX;
	onle.snk = 200;
	onle.lmoney = 0;
	onle.lsnk = 0;
	onle.last_price_bought=0;
	onle.last_price_sold =0;

	printf("Running tests...\n");
	printf("[00] Testing some of the functions on clear order book...\n");
	printf("\tBuy should be INT_MAX...\t");
	assert ( findBuyPrice(offers,s) == INT_MAX );
	printf("PASS!!\n");

	printf("\tSell should be zero...\t");
	assert ( findSellPrice(offers,s) == 0 );
	printf("PASS!!\n");

	price =10;
       	amount = 10;
	printf("[01] Creating a SELL offer for account a for %11lu SNK @ %11lu MONEY...\n", amount, price);

	Offer test_offer;
	createOffer(SELL, price, amount, a, offers, s, &test_offer);
//	printOffer(&test_offer);

	printf("\tOffer amount set...\t");
	assert(test_offer.amount == 10);
	printf("PASS!!\n");

	printf("\tOffer price set...\t");
       	assert(test_offer.price == 10);
	printf("PASS!!\n");

	printf("\tOffer type set...\t");
        assert(test_offer.type == SELL);
	printf("PASS!!\n");

	printf("\tOffer account set...\t");
        assert(test_offer.account == a);
	printf("PASS!!\n");

	printf("\tAccount SNK locked...\t");
        assert(test_offer.account->lsnk == 10);
	printf("PASS!!\n");

	price = 1;
	amount = 1;
	printf("[02] Creating a BUY offer for account b for %11lu SNK @ %11lu MONEY...\n", amount, price);
	createOffer(BUY, price, amount, b, offers, s, &test_offer);

	printf("\tOffer amount set...\t");
	assert(test_offer.amount == 1);
	printf("PASS!!\n");

	printf("\tOffer price set...%lu\t", test_offer.price);
       	assert(test_offer.price == 1);
	printf("PASS!!\n");

	printf("\tOffer type set...\t");
        assert(test_offer.type == BUY);
	printf("PASS!!\n");

	printf("\tOffer account set...\t");
        assert(test_offer.account == b);
	printf("PASS!!\n");

	printf("\tAccount money locked...\t");
        assert(test_offer.account->lmoney == 0);
	printf("PASS!!\n");

	price = findBuyPrice(offers,s);
	amount = 1;
	printf("[03] Creating a BUY offer for account b for %11lu SNK @ %11lu (current price)...\n", amount, price);
	createOffer(BUY, price, amount, b, offers, s, &test_offer);

	printf("\tAccount money locked...\t");
        assert(a->lsnk == 9);
	printf("PASS!!\n");

	printf("\tOffer amount set...\t");
	if ( findSellOfferWithPrice( offers,s,10) != NULL ){
		test_offer = *findSellOfferWithPrice( offers,s,10);
		assert(test_offer.amount == 9);
		printf("PASS!!\n");
	} else {
		printf("FAIL!!\n");
	}

	// printAllOffers(offers, s);
	price = 10;
	amount = 9;
	printf("[04] Creating a BUY offer for account b for %11lu SNK @ %11lu (current price)...\n", amount, price);
	createOffer(BUY, price, amount, b, offers, s, &test_offer);

	printAccount(b);
	printf("\tNo SELL orders on orderbook now...\t");
	assert( findSellOfferWithPrice( offers,s,10) == NULL );
	printf("PASS!!\n");

	printf("\tAccount a has right amount of money...\t");
	assert( getAccountMoney(a) == 200 );
	printf("PASS!!\n");

	printf("\tAccount b has right amount of SNK...\t");
	assert( getAccountSnk(b) == 210 );
	printf("PASS!!\n");

	printf("\tAccount b has right amount of money...\t");
	assert( getAccountMoney(b) == 100);
	printf("PASS!!\n");

	printf("\tAccount a has right amount of SNK...\t");
	assert( getAccountSnk(a) == 90 );
	printf("PASS!!\n");

	price = 1;
	amount = 2;
	// printAllOffers(offers, s);
	printf("[05] Creating a BUY offer for account a for %11lu SNK @ %11lu...\n", amount, price);
	createOffer(BUY, price, amount, b, offers, s, &test_offer);
	printAllOffers(offers, s);
	printAccount(a);
	printf("\tAccount a has right amount of money...\t");
	assert( a->money == 200 );
	printf("PASS!!\n");

	printf("\tAccount b has right amount of locked money...\t");
	assert( b->lmoney == 2 );
	printf("PASS!!\n");

	printf("\tAccount a has right amount of money...\t");
	assert( a->money == 200 );
	printf("PASS!!\n");

	printf("\tAccount b has right amount of locked SNK...\t");
	assert( a->snk == 90 );
	printf("PASS!!\n");

	price = 1;
	amount = 9;
	printf("[06] Creating a BUY offer for account a for %11lu SNK @ %11lu...\n", amount, price);
//	if ( findBuyPrice(offers,s) != 0 ){
		createOffer(BUY, price, amount, b, offers, s, &test_offer);
//	}
//	TODO: Figure out the rest of testing assertions
//

	price = 1;
	amount = 1;
	printf("[07] Creating a SELL offer for account a for %11lu SNK @ %11lu (current price)...\n", amount, price);
	createOffer(SELL, price, amount, b, offers, s, &test_offer);

	printAllOffers(offers, s);
	printf("[08] Creating a SELL offer for account a for %11lu SNK @ %11lu ...\n", amount, price);
	createOffer(SELL, price, amount, b, offers, s, &test_offer);
	printAllOffers(offers, s);

	printf("[09] Creating a BUY offer for account a for %11lu SNK @ %11lu (current price)...\n", amount, price);
	createOffer(BUY, price, amount, a, offers, s, &test_offer);
	printAllOffers(offers, s);

	price = 1;
	amount = 20;
	printf("[10] Creating a SELL offer for account a for %11lu SNK @ %11lu (current price)...\n", amount, price);
	createOffer(SELL, price, amount, b, offers, s, &test_offer);

	price = 10;
	amount = (long unsigned int) INT_MAX / 100000;
	printAllOffers(offers, s);
	printf("[11] Creating a BUY offer for account onle for %11lu SNK @ %11lu (current price)...\n", amount, price);
	// createOffer(BUY, 10, 18, a, offers, s, &test_offer);
	createOffer(BUY, price, amount, &onle, offers, s, &test_offer);

	price = 1;
	amount = getAccountSnk(a);
	printAllOffers(offers, s);
	printf("[12] Creating a SELL offer for account a for %11lu SNK @ %11lu (current price)...\n", amount, price);
	createOffer(SELL, price, amount, a, offers, s, &test_offer);

	printAllOffers(offers, s);
	printAccount(a);
	printAccount(b);

}

double qcid(long unsigned int a)
{
	return (double) a / DECIMAL_PRECISION;
}

long unsigned int qcdi(double a)
{
	return floor(a * DECIMAL_PRECISION);
}

long unsigned int applyPrecision( long unsigned int number)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
//	return (floor(pow(10,5) * number)) / pow(10,5);
	return number;
}
/*
void findMinMaxFromOffers(Offer **a, long unsigned int *s )
{
	if ( DEBUG ) printf("** %s called\n",__func__);

	Offer b;
	b = **a;
	long unsigned int max = b.price;
	long unsigned int min = b.price;
	long unsigned int max_thesis = 0;
	long unsigned int min_thesis = 0;

	for ( long unsigned int i = 0; i < (*s) ; i=i+1){
		b = *(*a+i);
		if (max < b.price){
			max = b.price;
			max_thesis = i;
		}
		if (min > b.price){
			min = b.price;
			min_thesis = i;
		}
	}
	b = *(*a+max_thesis);
	printf(" MAX_PRICE: %i @ %lu -- %i\n", max, max_thesis, b.type);
	b = *(*a+min_thesis);
	printf(" MIN_PRICE: %i @ %lu -- %i\n", min, min_thesis, b.type);

}

void swapOffers(Offer *a, Offer *b)
{
	Offer c;
	Offer *ptr = &c;
	setOfferType(ptr, *getOfferType(a));
	setOfferAmount(ptr, *getOfferAmount(a));
	setOfferPrice(ptr, *getOfferPrice(a));
	setOfferAccount(ptr, getOfferAccount(a));
	setOfferType(a, *getOfferType(b));
	setOfferAmount(a, *getOfferAmount(b));
	setOfferPrice(a, *getOfferPrice(b));
	setOfferAccount(a, getOfferAccount(b));
	setOfferType(b, *getOfferType(ptr));
	setOfferAmount(b, *getOfferAmount(ptr));
	setOfferPrice(b, *getOfferPrice(ptr));
	setOfferAccount(b, getOfferAccount(ptr));
}

void sortOffers(Offer **a)
{
	Offer *ptr = NULL;
	for (  ptr = *a; ptr < *a + ORDERBOOK_SIZE - 1; ++ptr){
		for (  ptr = *a; ptr < *a + ORDERBOOK_SIZE - 1; ++ptr){
			if ( *getOfferPrice(ptr) != 0x0 && *getOfferAmount(ptr) != 0x0 && getOfferAccount(ptr) != 0x0){
				if ( *getOfferPrice(ptr) < *getOfferPrice(ptr+1) ){
					swapOffers((ptr),(ptr+1));
				}
			}
		}
	}
}

long unsigned int findBuyPrice(Offer **a, long unsigned int *s)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	long unsigned int min = INT_MAX;

	for ( long unsigned int i = 0; i < (*s) ; i=i+1){
		if ( *getOfferType(*a+i) == SELL && min > *getOfferPrice(*a+i)){
			min = *getOfferPrice(*a+i);
		}
	}
	return min;
}
*/
/*
void setOfferAmount(Offer *a,long unsigned int amount)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	a->amount = applyPrecision(amount);
}

void setOfferAccount(Offer *a, Account *account)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	a->account = account;
}

void setOfferPrice(Offer *a, long unsigned int price)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	a->price = applyPrecision(price);
}

void setOfferType(Offer *a, OfferType type)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	a->type = type;
}

OfferType *getOfferType(Offer *a)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	return &(a->type);
}

long unsigned int *getOfferPrice(Offer *a)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	return &(a->price);
}

long unsigned int *getOfferAmount(Offer *a)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	return &(a->amount);
}

Account *getOfferAccount(Offer *a)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	return (a->account);
}

void setAccountMoney(Account *a, long unsigned int money)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	a->money = applyPrecision(money);
}

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

void lockAccountSnk(Account *a, unsigned  int snk)
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

void setAccountSnk(Account *a, long unsigned int snk)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	a->snk = applyPrecision(snk);
}
long unsigned int getAccountMoney(Account *a)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	return a->money;
}

long unsigned int getAccountSnk(Account *a)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	return a->snk;
}

long unsigned int findSellPrice(Offer **a, long unsigned int *s)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	// int max = *getOfferPrice(*a);
	long unsigned int max = 0;
	for ( long unsigned int i = 0; i < (*s) ; i=i+1){
		if ( *getOfferType(*a+i) == BUY ){
			 max = *getOfferPrice(*a+i);
			 break;
		}
	}

	for ( long unsigned int i = 0; i < (*s) ; i=i+1){
		if ( *getOfferType(*a+i) == BUY && max < *getOfferPrice(*a+i)){
			max = *getOfferPrice(*a+i);
		}
	}
	return max;
}

void printAccount(Account *a)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	if ( a != 0x0 ) {
		printf("\t%p [M(L): %f (%f), S(L): %f(%f), LB: %f, LS: %f]\n", (void*)a, qcid(a->money), qcid(a->lmoney), qcid(a->snk), qcid(a->lsnk), qcid(a->last_price_bought), qcid(a->last_price_sold));
	}
	else {
		printf("No account");
	}
}

Offer *findOfferWithPrice(Offer **a, long unsigned int *s, long unsigned int price)
{
	if ( DEBUG ) printf("** %s called\n",__func__);

	Offer *ptr = NULL;
//	for ( long unsigned int i = 0; i < (*s) ; i=i+1){
	for (  ptr = *a; ptr < *a + ORDERBOOK_SIZE - 1; ++ptr){
//		b = *(*a+i);
		if ( price == *getOfferPrice(ptr)){
			//printf("Found: [] ");
			//printOffer(ptr);
			return (ptr);
		}
	}
	(void)s;
	return NULL;
}

Offer *findSellOfferWithPrice(Offer **a, long unsigned int *s, long unsigned int price)
{
	if ( DEBUG ) printf("** %s called\n",__func__);

	Offer b;
	for ( long unsigned int i = 0; i < (*s) ; i=i+1){
		b = *(*a+i);
//		(void)price;
		if ( *getOfferType(&b) == SELL && *getOfferPrice(&b) == price){
			//printf("Found sell offer:\n\t[%lu] ", i);
			//printOffer(&b);
			return (*a+i);
		}
	}
	return NULL;
}

Offer *findBuyOfferWithPrice(Offer **a, long unsigned int *s,  long unsigned int price)
{
	if ( DEBUG ) printf("** %s called\n",__func__);

	Offer b;
	for ( long unsigned int i = 0; i < (*s) ; i=i+1){
		b = *(*a+i);
		if ( b.type == BUY && price == b.price){
			if ( DEBUG ) printf("Found buy offer:\n\t[%lu] ", i);
			if ( DEBUG ) printOffer(&b);
			return (*a+i);
		}
	}
	return NULL;
}
*/
/*
 * Describing the logic of how we add the offer and wether we execute it or
 * store it.
 *
 * 1. In case of empty Orderbook we store
 * 2. We distinct the two OfferTypes (SELL, BUY)
 * 3. SELL
 * 	3.1. Figure out if there is matching BUY order at the same price
 * 	3.2. 
 * 	
 *
 *
 *
 */
/*
void addOffer(Offer *b, Offer **a, long unsigned int *s)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	//printOffer(b);
//	printAccount(getOfferAccount(b));

	if ( *s == 0 ){
//		*a = b;
//		*s=(*s)+1;
		storeOffer(b,a,s);
	}
	else {
		if ( *getOfferType(b) == SELL ){
			Offer *c = findBuyOfferWithPrice(a,s,*getOfferPrice(b));
//			Offer *d = findOfferWithPrice(a,s,*getOfferPrice(b));
			long unsigned int buyprice = findBuyPrice(a,s);
			if ( c != NULL ){
				if ( *getOfferAmount(c) < *getOfferAmount(b) && *getOfferAmount(c) != 0 ){
					// b exceeds
					// so we have leftover in b
					//
					long unsigned int left = applyPrecision(*getOfferAmount(b) - *getOfferAmount(c));
					long unsigned int am = *getOfferAmount(c);
					long unsigned int price = *getOfferPrice(b);
					long unsigned int mv = applyPrecision(am * price);
					setOfferType(c, *getOfferType(b));
					unlockAccountMoney(getOfferAccount(c),mv);
					
					sendSnkFromOffer(b,c,am);
					sendMoneyFromOffer(c,b,mv);

					setOfferAccount(c, getOfferAccount(b));

					lockAccountSnk(getOfferAccount(b), left);

					setOfferAmount(c, left);
					printf("\t-EEEE->\tSold %i @ %i\n", am, price);
				}
				else if ( *getOfferAmount(c) > *getOfferAmount(b) && *getOfferAmount(c) != 0 ){
					// c exceeds
					// so we have leftover in c

					if ( *getOfferAmount(c) != 0 ){
						long unsigned int left = applyPrecision(*getOfferAmount(c) - *getOfferAmount(b));
						long unsigned int am = *getOfferAmount(b);
						long unsigned int price = *getOfferPrice(b);
						long unsigned int mv = applyPrecision(am * price);

						unlockAccountMoney(getOfferAccount(c),mv);

						sendSnkFromOffer(b,c,am);
						sendMoneyFromOffer(c,b,mv);

						setOfferAmount(c, left);
						printf("\t-***>\tSold %i @ %i\n", am, price);
					}
					else {
						printf("\n\n---------------LOL----------------\n\n");
					}
				}
				else if ( *getOfferAmount(c) == *getOfferAmount(b) ){
					// they should just exchange.
					// Question: What should be left in the c?

					long unsigned int am = *getOfferAmount(b);
					long unsigned int price = *getOfferPrice(b);
					long unsigned int mv = applyPrecision(am * price);

					unlockAccountMoney(getOfferAccount(c), am);
					sendSnkFromOffer(b,c,am);
					sendMoneyFromOffer(c,b,mv);

					resetOffer(c);

					*s = (*s) - 1;
					printf("\t-FFFF->\tSold %i @ %i\n", am, price);
				}
			}
			else if ( buyprice != INT_MAX && buyprice!= 0 ) {
				if ( *getOfferPrice(b) < buyprice ){
					Offer *x = findSellOfferWithPrice(a, s, buyprice);
					if ( x != 0x0 && *getOfferAmount(b) > *getOfferAmount(x)){
						long unsigned int left = applyPrecision(*getOfferAmount(b) - *getOfferAmount(x));
						long unsigned int am = *getOfferAmount(x);
						long unsigned int price = *getOfferPrice(x);
						long unsigned int mv = applyPrecision( am * price);

						unlockAccountMoney(getOfferAccount(x), am);
						sendSnkFromOffer(x,b,am);
						sendMoneyFromOffer(b,x,mv);
						resetOffer(x);
						setOfferAmount(b, left);
						storeOffer(b,a,s);
						printf("\t-GGGG->\tSold %i @ %i\n", am, price);
					}
					else {
						printf("lol");
						createOffer(*getOfferType(b), buyprice, *getOfferAmount(b), getOfferAccount(b), a, s, b);
					}
//					createOffer(SELL, 1, 1,&onle, &offers, &sizeOfOffers, &b);
				}
			}
			else {
				storeOffer(b,a,s);
			}
				// *(*a+(*s)) = *b;
				// *s=(*s)+1;
		}
		else if ( b->type == BUY ){
			Offer *c = findSellOfferWithPrice(a,s, *getOfferPrice(b));
//			Offer *d = findOfferWithPrice(a,s,*getOfferPrice(b));
			long unsigned int sellprice = findSellPrice(a,s);
			if ( c != NULL ){
				if ( *getOfferAmount(c) < *getOfferAmount(b) ){
					// b exceeds
					// so we have leftover in b

					if ( *getOfferAmount(c) != 0 ){
						long unsigned int left =applyPrecision( *getOfferAmount(b) - *getOfferAmount(c));
						long unsigned int am = *getOfferAmount(c);
						long unsigned int price = *getOfferPrice(b);
						long unsigned int mv =applyPrecision( am * price);
						setOfferType(c, *getOfferType(b));

						unlockAccountSnk(getOfferAccount(c), am);

						sendSnkFromOffer(c,b,am);
						sendMoneyFromOffer(b,c,mv);

						lockAccountMoney(getOfferAccount(b), left);

						setOfferAccount(c, getOfferAccount(b));
						setOfferAmount(c, left);
						if ( DEBUG ) printf("\t->\t AAAA Bought %i @ %i\n", am, price);
					}
					else {
						printf("\n\n---------------LOL----------------\n\n");
					}
				}
				else if ( *getOfferAmount(c) > *getOfferAmount(b) ){
					// c exceeds
					// so we have leftover in c
					long unsigned int left = applyPrecision( *getOfferAmount(c) - *getOfferAmount(b));
					long unsigned int am = *getOfferAmount(b);
					long unsigned int price = *getOfferPrice(b);
					long unsigned int mv = applyPrecision( am * price);

					unlockAccountSnk(getOfferAccount(c), am);
					sendSnkFromOffer(c,b,am);
					sendMoneyFromOffer(b,c,mv);

					setOfferAmount(c, left);
					//printOffer(b);
					//printOffer(c);
					if ( DEBUG ) printf("\t->\t BBBB Bought %i @ %i\n", am, price);
				}
				else if ( *getOfferAmount(c) == *getOfferAmount(b) ){
					// they should just exchange.
					// Question: What should be left in the c?
					long unsigned int am = *getOfferAmount(b);
					long unsigned int price = *getOfferPrice(b);
					long unsigned int mv = applyPrecision(am * price);

					unlockAccountSnk(getOfferAccount(c),am);
					sendSnkFromOffer(c,b,am);
					sendMoneyFromOffer(b,c,mv);
					resetOffer(c);
					*s = (*s) - 1;
					printf("\t->\t CCCC Bought %i @ %i\n", am, price);
				}
			}
			else {
				if ( *getOfferPrice(b) > sellprice ){
					Offer *x = findSellOfferWithPrice(a, s, sellprice);
					
					if ( x != 0x0 && *getOfferAmount(b) > *getOfferAmount(x)){
						long unsigned int left =applyPrecision( *getOfferAmount(b) - *getOfferAmount(x));
						long unsigned int am = *getOfferAmount(x);
						long unsigned int price = *getOfferPrice(x);
						long unsigned int mv =applyPrecision( am * price);


						unlockAccountSnk(getOfferAccount(x), am);
						sendSnkFromOffer(x,b,am);
						sendMoneyFromOffer(b,x,mv);
						resetOffer(x);
						setOfferAmount(b, left);
						// lockAccountSnk(getOfferAccount(b), am);
						storeOffer(b,a,s);
						printf("\t->\t DDDD Bought %i @ %i\n", am, price);
					}
					else {
						createOffer(*getOfferType(b), sellprice, *getOfferAmount(b), getOfferAccount(b), a, s, b);
					}
				}
				else {
					storeOffer(b,a,s);
				}
			}
		}
	}
}

void sendSnkFromOffer(Offer *from, Offer *to, long unsigned int amount)
{
//	unlockAccountSnk(getOfferAccount(from),amount);
	setAccountSnk( getOfferAccount(from), getAccountSnk(getOfferAccount(from)) - amount);
	setAccountSnk( getOfferAccount(to), getAccountSnk(getOfferAccount(to)) + amount);
}
void sendMoneyFromOffer(Offer *from, Offer *to, long unsigned int amount){
//	unlockAccountMoney(getOfferAccount(from),amount);
	setAccountMoney( getOfferAccount(from), getAccountMoney(getOfferAccount(from)) - amount);
	setAccountMoney( getOfferAccount(to), getAccountMoney(getOfferAccount(to)) + amount);
}

Offer *findFirstEmptyOffer(Offer **a, long unsigned int *s )
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	Offer *ptr = NULL;
	(void)*s;
	for (  ptr = *a; ptr < *a + ORDERBOOK_SIZE ; ++ptr){
		if ( *getOfferPrice(ptr) == 0x0 && *getOfferAmount(ptr) == 0x0 && getOfferAccount(ptr) == 0x0){
			return (ptr);
		}
	}
	return 0x0;
}
void storeOffer(Offer *b, Offer **a, long unsigned int *s)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
//	printf("~~~~~\t %lu \t~~iiii %p iii~~~\n~",*s, (void*)c);
	Offer *ptr = NULL;
	for (  ptr = *a; ptr < *a + ORDERBOOK_SIZE ; ++ptr){
		//if ( DEBUG)
		if ( *getOfferPrice(ptr) == 0x0 && *getOfferAmount(ptr) == 0x0 && getOfferAccount(ptr) == 0x0){
			if ( DEBUG ) printf("{{{{{{{{{{{{{{{{{ %p }}}}}}}}}}}}}}}}}\n", (void*)ptr);
			setOfferType(ptr,  *getOfferType(b));
			if ( *getOfferType(b) == BUY ){
				lockAccountMoney(getOfferAccount(b), *getOfferAmount(b) * *getOfferPrice(b));
			}
			else if ( *getOfferType(b) == SELL ){
				lockAccountSnk(getOfferAccount(b), *getOfferAmount(b));
			}

			setOfferAmount(ptr,  *getOfferAmount(b));
			setOfferPrice(ptr,  *getOfferPrice(b));
			setOfferAccount(ptr,  getOfferAccount(b));
			(*s)++;
			break;
		}
	}
}

Offer *createOffer(OfferType type, long unsigned int price, long unsigned int amount, Account *acc, Offer **a, long unsigned int *s, Offer *b)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	// printf("Trying to add: \t");
	if (type == SELL){
//		assert(amount <= acc->snk);
		if (!(amount <= acc->snk)) {
			printf("--------> a\n");
			return NULL;
		}
	}
	else if (type== BUY){
//		assert(price*amount <= acc->money);
		if (!(price*amount <= acc->money)) {

			printf("--------> b\n");
			return NULL;
		}
	}
	if ( price <= 0 || amount <= 0 ){
		return NULL;
	}
	Offer *l = findOfferWithPrice(a, s, price);
	if ( l != NULL && getOfferAccount(l) == acc && *getOfferType(l) != type ) {
		printf("--------> c\n");
		return NULL;
	}
	setOfferType(b, type);
	setOfferAmount(b, amount);
	setOfferPrice(b, price);
	setOfferAccount(b, acc);
	if (DEBUG) printOffer(b);
	addOffer(b, a, s);
	return b;
}

void printOffer(Offer *a)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	if ( DEBUG ) printf("%i %i %p\n", *getOfferPrice(a) , *getOfferAmount(a) , (void*)getOfferAccount(a) );
	if ( *getOfferPrice(a) != 0x0 && *getOfferAmount(a) != 0x0 && getOfferAccount(a) != 0x0){
		if ( *getOfferType(a) == SELL ) printf("\033[0;31mSELL\033[0;0m: ");
		else if ( *getOfferType(a) == BUY ) printf("\033[0;32mBUY\033[0;0m : ");
		printf("%f @ %f\tby", qcid(*getOfferAmount(a)), qcid(*getOfferPrice(a)));
		printAccount(getOfferAccount(a));
	}
}

void printAllOffers(Offer **a, long unsigned int *s )
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	printf("---------------------------------\n");
	sortOffers(a);
	sortOffers(a);
	Offer *ptr = NULL;
	(void)*s;
	for (  ptr = *a; ptr < *a + ORDERBOOK_SIZE ; ++ptr){
		printOffer(ptr);
	}
	printf("---------------------------------\n");
}
*/
/*
 *
 * The rest is part of the previous attempt. It set the basis for the above
 *

long unsigned int price(Market *market)
{
	return market->money / market->snk;
}

void buy(int ask, Account *bot, Market *market)
{
	int last_money = bot->money;
	int last_price = price(market);
	if ( bot->money > ( last_price * ask ) && (bot->last_price_bought > last_price || bot->snk == 0 )) {
		bot->money = bot->money - ( last_price * ask );
		market->money = market->money + ( last_price * ask );
		market->snk = market->snk - ask;
		bot->snk = bot->snk + ask;
		int total_snk = market->snk + bot->snk;
		bot->last_price_bought = last_price;
		print_stats( last_price, total_snk, bot->money-last_money, __func__, ask, bot, market);
	}
}

void sell(int bid, Account *bot, Market *market)
{
	int last_money = bot->money;
	int last_price = price(market);
	if ( bid < bot->snk ) {
		bot->money = bot->money + ( last_price * bid );
		market->money = market->money - ( last_price * bid );
		market->snk = market->snk + bid;
		bot->snk = bot->snk - bid;
		int total_snk = market->snk + bot->snk;
		bot->last_price_sold = last_price;
		print_stats( last_price, total_snk, bot->money-last_money, __func__, bid, bot, market);
	}
}

long unsigned int getTotalMoneyFromAccounts(Account *bots)
{
	int total_money = 0;
	for (int i = 0; i < BOT_NUMBER; i++) {
		total_money += bots[i].money;
	}
	return total_money;
}

long unsigned int getTotalSnkFromAccounts(Account *bots)
{
	int total_snk = 0;
	for (int i = 0; i < BOT_NUMBER; i++) {
		total_snk += bots[i].snk;
	}
	return total_snk;
}

void getAccountWithMostMoney(Account *bots)
{
	int max_money = 0;
	int thesis = 0;
	for (int i = 0; i < BOT_NUMBER; i++) {
		if (max_money < bots[i].money){
			max_money = bots[i].money;
			thesis = i;
		}
	}
	printf("Most has #%i: %i\t", thesis, max_money);
}

void getAccountWithLessMoney(Account *bots)
{
	int min_money = 1000000;
	int thesis = 0;
	for (int i = 0; i < BOT_NUMBER; i++) {
		if (min_money > bots[i].money){
			min_money = bots[i].money;
			thesis = i;
		}
	}
	printf("Less has # %i: %i\t", thesis, min_money);
}

void print_stats(int last_price, int total_snk, int last_money, const char *last_action, int amount, Account *bot, Market *market)
{
	printf("%s: %i\t", last_action, amount);

	printf("Price: ");
	if (last_price < market->money/market->snk) printf("\033[0;32m");
	if (last_price > market->money/market->snk) printf("\033[0;31m");
	printf("%i -> %i\t", last_price, market->money/market->snk);
	printf("\033[0;0m");

	// printf("money/snk: %i\t",snk/money);
	printf("S(T): %i (%i)\t", market->snk,total_snk);
	printf("S Money: %i\t",market->money);
	printf("B money: %i\t",bot->money);
	if (last_money > 0) printf("\033[0;32m");
	if (last_money < 0) printf("\033[0;31m");
	printf("(%i)\t", last_money);
	printf("\033[0;0m");
	printf("Bot snk: %i", bot->snk);
	printf("\n");
}
long unsigned int xd(void)
{
	int seed = 69;
	int range = BOT_NUMBER;

	Market market;
	market.money = MAX_MONEY;
	market.snk = MAX_SNK;
	Account bots[BOT_NUMBER];
	int init_money = 2.0;
	for (int i = 0; i < BOT_NUMBER; i++) {
		bots[i].money = init_money;
		bots[i].snk = 0;
		bots[i].last_price_bought = 0;
		bots[i].last_price_sold = 0;
		//init_money++;
	}
	int ask = 0.01;
	int bid = 0.01;
	int counter = 0;

	Account mm;
	mm.money = 1;
	mm.snk = 0;
	mm.last_price_bought = 0;
	mm.last_price_sold = 0;
	int last_price = price(&market);
	printf("Starting point\nsnk/money: %i\tmoney/snk: %i\n", last_price, market.snk/market.money);
	while ( market.money > 1 ){
		srand(seed);
		last_price = price(&market);
		for (int i = 0; i < BOT_NUMBER; i++) {
//			if ( bots[i].last_price_bought < last_price) {
			if ( bots[i].last_price_bought <= last_price) {
				bid = bots[i].snk * 0.05;
				if ( bid > 0.00001 ) sell(bid, &bots[i], &market);
			}
			else if ( bots[i].last_price_bought > last_price || bots[i].snk == 0 ) {
				ask = bots[i].money * 0.02;
				if ( ask > 0.00001 ) buy(ask, &bots[i], &market);
			}
		}
		int i = 0 + ( rand() % range );
		if ( bots[i].last_price_bought > last_price || bots[i].snk == 0 ) {
			ask = bots[i].money * 0.02;
			if ( ask > 0.00001 ) buy(ask, &bots[i], &market);
		}
		else if ( bots[i].last_price_bought <= last_price) {
			bid = bots[i].snk * 0.5;
			if ( bid > 0.00001 ) sell(bid, &bots[i], &market);
		}

		if ( mm.last_price_bought > last_price || mm.snk == 0 ) {
			ask = mm.money * 0.2;
			if ( ask > 0.00001 ) buy(ask, &mm, &market);
		}
		if ( mm.last_price_bought <= last_price) {
			bid = mm.snk * 0.75;
			//if ( bid > 0.00001 )
			sell(bid, &mm, &market);

		}
		if ( price(&market) < 0.01 ){
			ask = mm.money * 0.5;
			if ( ask > 0.00001 ) buy(ask, &mm, &market);
		}
		if ( market.money < 0.1 && market.snk + getTotalSnkFromAccounts(bots) < 50 ) {
			market.snk++;
		}
		if (last_price == market.money/market.snk) {
			ask = mm.money * 0.5;
			if ( ask > 0.00001 ) buy(ask, &mm, &market);
		}
		counter++;
		// if ( bot.money > 30.00 ) break;
	//	last_price = market.money/market.snk;
	//	if ( counter % 50 == 0 && market.snk < 100.00 ) market.snk = market.snk + (1.00-(1.00/counter));
	//	if ( counter % 12 == 0 ) bot.money++;
		if (counter % (60*24*7) == 0) {
			printf("[%i]\t", counter);
			printf("Price: ");
			printf("%i -> ", last_price);
			if (last_price < market.money/market.snk) printf("\033[0;32m");
			if (last_price > market.money/market.snk) printf("\033[0;31m");
			printf("%i\t", market.money/market.snk);
			printf("\033[0;0m");
			printf("TBM: %i\t", getTotalMoneyFromAccounts(bots));
			printf("TBS: %i\t", getTotalSnkFromAccounts(bots));
			printf("MMM: %i\t", mm.money);
			printf("MMS: %i\t", mm.snk);
			getAccountWithMostMoney(bots);
			getAccountWithLessMoney(bots);
			printf("Market money: %i\t", market.money);
			printf("Market SNK: %i\t", market.snk);
			printf("\n");
			market.money += market.money*0.001;
//			market.snk += market.snk*0.001;
		}
		//sleep(1);
		seed++;
	}
	return 0;
}

*/
