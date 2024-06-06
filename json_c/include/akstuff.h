#ifndef AK_STUFF
#define AK_STUFF
#define IPFS_CID_V0_LENGTH 46

typedef struct{
	char ipfs[47];
} ipfsCID;

typedef struct{
	char *ipns;
} ipns;

typedef struct {
	ipfsCID self;	
} Gpg;

typedef struct {
	ipfsCID self;
} Previous;

typedef struct {
	char action[32];	
} Action;

typedef struct {
	char module[32];
} Module;

typedef struct {
	char timestamp[12];
} Timestamp;

typedef struct {
	ipfsCID self;
} DataLink;

typedef struct {
	ipfsCID self;
} SignedDataLink;

typedef struct {
	ipfsCID self;
	DataLink *data_link;
	SignedDataLink *signed_data_link;
} Data;

typedef struct {
	ipfsCID self;
} BlockSignature;

typedef struct {
	ipfsCID self;
} DataSignature;

typedef struct {
	ipfsCID self;
	Timestamp *timestamp;	
	Module *module;
	Action *action;
	Data *data;
	Gpg *gpg;
	DataSignature *data_signature;
	Previous *previous;
} Block;

typedef struct {
	ipfsCID self;
	Block *block;
	BlockSignature *block_signature;
} Zblock;

/*
char *AK_zblock;

char *AK_block;

char *AK_block_signature;

char *AK_action;

char AK_data[46];

char *AK_ipfs;

char *AK_detach;

char *AK_previous;

char *AK_timestamp;
*/
void lol();

void check_tag(char *);

void assignValueToZblock(char *, Zblock *);

void assignValueToBlock(char *, Block *);

void assignValueToBlockSignature(char *, BlockSignature *);

void assignValueToAction(char *, Action *);

void assignValueToModule(char *, Module *);

void assignValueToData(char *, Data *);

void assignValueToDataSignature(char *, DataSignature *);

void assignValueToGpg(char *, Gpg *);

void assignValueToDataLink(char *, DataLink *);

void assignValueToSignedDataLink(char *, SignedDataLink *);

void assignValueToTimestamp(char *, Timestamp *);

void assignValueToPrevious(char *, Previous *);

void do_something_cause_with(char *);

void complete_the_zblock();

void print_zblock(Zblock *);

void append_zblock(Zblock *, long unsigned int);

#endif
// k51qzi5uqu5dlocpfn8w8wpi55w2uqilcuq86ihoi5ksfjz9sgqgva06prfgbg
