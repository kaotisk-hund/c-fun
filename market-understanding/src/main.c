#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include "account.h"
#include "offer.h"
#include "market-cit.h"
#include "config.h"

#define ARRAYSIZE(arr) sizeof(arr)/sizeof(arr[0])

// For understanding the market we need our two basic things set up. 1) Demand,
// 2) Supply. To get it to as low level as we can, we will assume that we are
// tracking the price of something for money.
// 1) Demand means how much in terms of amount volume is asked in the pool
//    (market). Therefore, these questions would be the "buy" thing.
// 2) Supply is the opposite of demand. There should be a definite or changing
//    amount of supply. This depends of the "quality" of the supplied thing.
//    For example, sunlight can't be sold cause there's relatively infinite
//    supply. A stock with a thousand shares is something defined. This is the
//    part of "sell" thing.
// However, supply can be changing in terms of how much is already bought and
// how much of it is available in the market.
//
// Also, demand is kinda a what-the-fuck zone.
//
// Let's kinda pretend that this is kinda a real but isolated case.
//
// We will pretend to have $MONEY and SNK.
//
// We will have 10000 of SNK to sell
// And we will have let's say a definite amount of money
// From that we can have two divisions which will provide us
// with the price in $MONEY of each SNK and  numbers.
//
// How much $MONEY a SNK costs is symbolised with SNK/MONEY (price of SNK)
//
// To calculate that, we will assume that it is equal (price) with the ...
//
// All-SNKs = 10000
// Starting Evaluation = 1 <- Money on the pool already
// 1 / 10000 = 0.0001 money / snk <- this is the price of each SNK
//
// How much SNK you can get for 1$MONEY is symbolised with MONEY/SNK
// 10000 / 1 = 10000  snk / money <- this is t, Market markethe amount of SNK
// for 1$MONEY
//
// Someone "buys" some amount... how that would be expressed?
// we want 100 SNK, what's the price?

int xd(void);
int main(void)
{
	printf("Hello!!\n");
	Offer orderBook[ORDERBOOK_SIZE];
	Offer *offers = &orderBook[0];
	void* offers_start = &orderBook[0];
	//long unsigned int sizeOfOffer = sizeof(orderBook[0]);

	initOrderBook(&offers);
	//offerPtr = &orderBook[0];
	printf("START:%p\n",offers_start);
	long unsigned int sizeOfOffers = 0;

	Account one;
	one.money = 100.00;
	one.snk = 100.0;
	one.lmoney = 0;
	one.lsnk = 0;
	one.last_price_bought=0;
	one.last_price_sold =0;

	Account onle;
	onle.money = 200.00;
	onle.snk = 200.00;
	onle.lmoney = 0;
	onle.lsnk = 0;
	onle.last_price_bought=0;
	onle.last_price_sold =0;

	run_test( &one, &onle, &offers, &sizeOfOffers );
	//printAllOffers( &offers, &sizeOfOffers);
	Offer b;
	createOffer(SELL, 1, 10, &onle, &offers, &sizeOfOffers, &b);
	//printAllOffers( &offers, &sizeOfOffers);
	//printAccount(&onle);

	printAllOffers( &offers, &sizeOfOffers);
	Offer a;
	createOffer(BUY, 1, 1, &one, &offers, &sizeOfOffers, &a);
//	printAccount(&onle);
//	printAccount(&one);

	Offer c;
        createOffer(SELL, 1.81, 1, &one, &offers, &sizeOfOffers, &c);
	printAllOffers( &offers, &sizeOfOffers);
	Offer d;
	createOffer(BUY, 0.49, 1, &one, &offers, &sizeOfOffers, &d);
	printAllOffers( &offers, &sizeOfOffers);

	Offer e;
	createOffer(SELL, 1.2, 20, &onle, &offers, &sizeOfOffers, &e);
	printAllOffers( &offers, &sizeOfOffers);

	int counter = 0;
	while ( counter < 3 ){

	Offer k;
//		printAllOffers( &offers, &sizeOfOffers);

		createOffer(SELL, findBuyPrice( &offers, &sizeOfOffers), 0.1, &onle,
                                        &offers, &sizeOfOffers, &k);
	//	printAllOffers( &offers, &sizeOfOffers);

		counter++;

	//	sleep(1);
	}
	for (int i = 0; i < 3; i++){
		Offer l;
		createOffer(BUY, findSellPrice( &offers, &sizeOfOffers), 0.1, &one, &offers, &sizeOfOffers, &l);
	}

	Offer y;
	createOffer(SELL, findBuyPrice( &offers, &sizeOfOffers), 1, &onle, &offers, &sizeOfOffers, &y);
	//printAllOffers( &offers, &sizeOfOffers);
	int high_spot = findSellPrice( &offers, &sizeOfOffers);
	//printOffer(high_spot);

	int sell_spot = findBuyPrice( &offers, &sizeOfOffers);

	findMinMaxFromOffers( &offers, &sizeOfOffers);
	printAllOffers( &offers, &sizeOfOffers);
//	printAccount(&onle);
//	printAccount(&one);
	(void)high_spot;
	(void)sell_spot;
///	printOffer(sell_spot);

	Account ole;
	ole.lmoney = 0;
	ole.lsnk = 0;
	ole.last_price_bought=0;
	ole.last_price_sold =0;
	printf("\nInsert money amount: ");
	scanf("%lu", &ole.money);
	printf("\nInsert SNK amount: ");
	scanf("%lu", &ole.snk);
	printAccount(&ole);

	char action = '\0';
	while ( true ) {
		Offer uo;
		long unsigned int uam = 0;
		long unsigned int prc = 0;
        printf("\nSelect action (b)uy, buy(l)mit, (s)ell, sells(t)op, (p)ass round, (q)uit: ");
		scanf("%c", &action);
		switch (action){
			case 'b':
				printf("\nEnter the amount you want to buy :");
				scanf("%lu", &uam);
				createOffer(BUY, findBuyPrice( &offers, &sizeOfOffers), uam, &ole, &offers, &sizeOfOffers, &uo);
				break;
			case 'l':
				printf("\nEnter the amount you want to buy :");
				scanf("%lu", &uam);
				printf("\nEnter the price you want to buy at :");
				scanf("%lu", &prc);
                printf("\n Gonna make operation BUY %lu @ %lu", uam, prc);
				createOffer(BUY, prc, uam, &ole, &offers, &sizeOfOffers, &uo);
				break;
			case 't':
				printf("\nEnter the amount you want to sell :");
				scanf("%lu", &uam);
				printf("\nEnter the price you want to sell at :");
				scanf("%lu", &prc);
                printf("\n Gonna make operation SELL %lu @ %lu", uam, prc);
				createOffer(SELL, prc, uam, &ole, &offers, &sizeOfOffers, &uo);
				break;
			case 's':
				printf("\nEnter the amount you want to sell :");
				scanf("%lu", &uam);
				createOffer(SELL, findSellPrice( &offers, &sizeOfOffers), uam, &ole, &offers, &sizeOfOffers, &uo);
				break;
			case 'p':
				break;
			case 'q':
				exit(0);
				break;
		}
		printAllOffers( &offers, &sizeOfOffers);
        printAccount(&ole);
	}
	//findOfferWithPrice( &offers, &sizeOfOffers, 1.000);
	//findBuyOfferWithPrice( &offers, &sizeOfOffers, 1);
	//findSellOfferWithPrice( &offers, &sizeOfOffers, 2);
//	xd();
	return 0;
}
