#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_
typedef struct {
	long unsigned int snk;
	long unsigned int money;
	long unsigned int lsnk;
	long unsigned int lmoney;
	long unsigned int last_price_bought;
	long unsigned int last_price_sold;
} Account;

/*
 * From a list of accounts get MAX and MIN money accounts.
 *
 */
void getAccountWithMostMoney(Account*);
void getAccountWithLessMoney(Account*);

/*
 * Get totals of money and SNK from a list of accounts.
 *
 */
int getTotalMoneyFromAccounts(Account*);
int getTotalSnkFromAccounts(Account*);

// Account setters and getters
void setAccountMoney(Account*, long unsigned int);
void setAccountSnk(Account*, long unsigned int);
long unsigned int getAccountMoney(Account*);
long unsigned int getAccountSnk(Account*);

/* Lock and unlock MONEY and SNK functions
 * These are used to lock or unlock the amount of the offer that is put
 * in the order book. Otherwise, an Account could add as many offers as
 * they 'd wish, even if they wouldn't have the total amount of all their
 * offers.
 */
void lockAccountMoney  (Account *, long unsigned int );
void lockAccountSnk    (Account *, long unsigned int );
void unlockAccountMoney(Account *, long unsigned int );
void unlockAccountSnk  (Account *, long unsigned int );

// Print account
void printAccount(Account *);

#endif // _ACCOUNT_H_
