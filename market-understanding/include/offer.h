#ifndef _OFFER_H_
#define _OFFER_H_

typedef enum {NONE, BUY, SELL} OfferType;
/*
 * An offer is of type BUY or SELL, it has an amount of SNK and a price for each.
 * It's also made by an account.
 *
 */

typedef struct {
	OfferType type;
	long unsigned int amount;
	long unsigned int price;
	Account *account;
} Offer; // size 32



// Simply should print (or not) an Offer
void printOffer(Offer*);
// Print ALL the offers o/
void printAllOffers(Offer**, long unsigned int*);
// Simple min and max printer from a list of offers
void findMinMaxFromOffers(Offer**, long unsigned int*);

// Obviously adds an offer
void addOffer(Offer*, Offer**, long unsigned int*);

// This creates it and returns it
// Caution it can return NULL or 0x0 (not sure, see the implementation)
Offer *createOffer(OfferType, long unsigned int, long unsigned int, Account*, Offer**, long unsigned int*, Offer*);

// Storing is a different thing. Not all offers are stored. Some are processed inside the
// addOffer(). However, since there is the possibility of having an offer partially completed
// we **store** it to the OrderBook or an Offer** 
void storeOffer(Offer*, Offer**, long unsigned int*);

// Offer setters
void setOfferType(Offer*, OfferType);
void setOfferPrice (Offer*, long unsigned int);
void setOfferAmount(Offer*, long unsigned int);
void setOfferAccount(Offer*, Account*);

// Offer getters
OfferType *getOfferType(Offer*);
long unsigned int *getOfferPrice(Offer*);
long unsigned int *getOfferAmount(Offer*);
Account *getOfferAccount(Offer*);

// Swap offers
void swapOffers(Offer *a, Offer *b);

// Finding offers based on price
Offer *findOfferWithPrice(Offer**, long unsigned int*, long unsigned int);

// Finding offers based on price and OfferType
Offer *findSellOfferWithPrice(Offer**, long unsigned int*, long unsigned int);
Offer *findBuyOfferWithPrice(Offer**, long unsigned int*, long unsigned int);

// Find the first empty place in Offer**
Offer *findFirstEmptyOffer(Offer**, long unsigned int* );

// Obvious, returns price
long unsigned int findBuyPrice(Offer**, long unsigned int*);
long unsigned int findSellPrice(Offer**, long unsigned int*);

// What is better than to start with a big but initialized orderbook?
void initOrderBook(Offer**);
void resetOffer(Offer*);

// Transaction based on offers
void sendMoneyFromOffer(Offer *, Offer *, long unsigned int );
void sendSnkFromOffer(Offer *, Offer *, long unsigned int );


// Sort the offers (based on price, see implementation)
void sortOffers(Offer**);

// Prints a given trade
void printTrade(OfferType, long unsigned int, long unsigned int);

#endif // _OFFER_H_
