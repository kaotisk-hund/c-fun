#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include "account.h"
#include "offer.h"
#include "market-cit.h"
#include "config.h"

void resetOffer(Offer *ptr)
{
	ptr->type = NONE;
	ptr->amount = 0x0;
	ptr->price = 0x0;
	ptr->account = 0x0;
}

void initOrderBook(Offer **a)
{
	Offer *ptr = NULL;
	for ( ptr = *a; ptr < *a + ORDERBOOK_SIZE; ++ptr ){
		//printf("%p\t", (void*)ptr);
		resetOffer(ptr);
	}
}

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
	printf(" MAX_PRICE: %lu @ %lu -- %i\n", max, max_thesis, b.type);
	b = *(*a+min_thesis);
	printf(" MIN_PRICE: %lu @ %lu -- %i\n", min, min_thesis, b.type);

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
    // TODO: Change the sorting into merge sort
    // The key is when the left over is 1 we simply add it to the end
    // We would get the array of offers and split it in two arrays
    // use recursion (call this function) to sort each split.
    //
    // Problems that I see up to now:
    // 1. We have no idea how big is the array of offers!!!
    // 2. Where do we merge? In place? Should I guess with pointers?
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
void addOffer(Offer *b, Offer **a, long unsigned int *s)
{
	if ( DEBUG ) printf("** %s called\n",__func__);

	if ( *s == 0 ){
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

					// The code below is not helping us complete as many offers as we can
					// Let's remember that many offers on the same price can exist.
					// Since we complete the first, we should go and create an new offer
					// instead of replacing the completed one.
					unlockAccountMoney(getOfferAccount(c),mv);
					sendSnkFromOffer(b,c,am);
					sendMoneyFromOffer(c,b,mv);

					resetOffer(c);

						printf("\t-EEEE->\tSold %lu @ %lu\n", am, price);
					printTrade(b->type, price, am);
					if ( findBuyOfferWithPrice(a,s,*getOfferPrice(b)) != NULL ){
						createOffer(*getOfferType(b), price, left, getOfferAccount(b), a, s, b);
					}
					else {
						setOfferType(c, *getOfferType(b));
						setOfferAccount(c, getOfferAccount(b));
						setOfferPrice(c, price);
						lockAccountSnk(getOfferAccount(b), left);
						setOfferAmount(c, left);
						printf("\t-EEEE->\tSold %lu @ %lu\n", am, price);
					}
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
						printTrade(b->type, price, am);
					//	printf("\t-***>\tSold %lu @ %lu\n", am, price);
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
					//printf("\t-FFFF->\tSold %lu @ %lu\n", am, price);
					printTrade(b->type, price, am);
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
						//printf("\t-GGGG->\tSold %lu @ %lu\n", am, price);
						printTrade(b->type, price, am);
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
				//*(*a+(*s)) = *b;
				//*s=(*s)+1;
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
						if ( DEBUG ) printf("\t->\t AAAA Bought %lu @ %lu\n", am, price);
						printTrade(b->type, price, am);
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
					if ( DEBUG ) printf("\t->\t BBBB Bought %lu @ %lu\n", am, price);
					printTrade(b->type, price, am);
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
					printf("\t->\t CCCC Bought %lu @ %lu\n", am, price);
					printTrade(b->type, price, am);
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
						printf("\t->\t DDDD Bought %lu @ %lu\n", am, price);
						printTrade(b->type, price, am);
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
			printf("--------> [ERROR] Can't create SELL offer: %lu > %lu\n", amount, acc->snk);
			return NULL;
		}
	}
	else if (type== BUY){
//		assert(price*amount <= acc->money);
		if (!(price*amount <= acc->money)) {

			printf("--------> [ERROR] Can't create BUY offer: %lu > %lu\n", price*amount, acc->money);
			return NULL;
		}
	}
	if ( price <= 0 || amount <= 0 ){
        printf("--------> [ERROR] Can't create negative offers.\n");
		return NULL;
	}
	Offer *l = findOfferWithPrice(a, s, price);
	if ( l != NULL && getOfferAccount(l) == acc && *getOfferType(l) != type ) {
        printf("--------> [ERROR] Can't cross offer. Skipping...\n");
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
	if ( DEBUG ) printf("%lu %lu %p\n", *getOfferPrice(a) , *getOfferAmount(a) , (void*)getOfferAccount(a) );
	if ( *getOfferPrice(a) != 0x0 && *getOfferAmount(a) != 0x0 && getOfferAccount(a) != 0x0){
		if ( *getOfferType(a) == SELL ) printf("\033[0;31mSELL\033[0;0m: ");
		else if ( *getOfferType(a) == BUY ) printf("\033[0;32mBUY\033[0;0m : ");
		printf("%11lu @ %11lu\tby", *getOfferAmount(a), *getOfferPrice(a));
		printAccount(getOfferAccount(a));
	}
}

void printTrade(OfferType type, long unsigned int price, long unsigned int amount)
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	if ( type == SELL ) printf("\033[30;41m");
	else if ( type == BUY ) printf("\033[30;42m");
	printf("%11lu @ %11lu\033[0;0m\n", amount, price);
}

void printAllOffers(Offer **a, long unsigned int *s )
{
	if ( DEBUG ) printf("** %s called\n",__func__);
	printf("\n---------- ORDER BOOK ----------\n");
	sortOffers(a);
	sortOffers(a);
	Offer *ptr = NULL;
	(void)*s;
	for (  ptr = *a; ptr < *a + ORDERBOOK_SIZE ; ++ptr){
		printOffer(ptr);
	}
	printf("--------------------------------\n");
}

