#include "akstuff.h"
#include "casting.h"
#include "config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

char *AK_zblock = "zblock";
char *AK_block = "block";
char *AK_block_signature = "block_signature";
char *AK_action = "action";
char AK_data[46];
char *AK_ipfs = "ipfs";
char *AK_module = "module";
char *AK_gpg = "gpg";
char *AK_timestamp = "timestamp";
char *AK_detach = "detach";
char *AK_previous = "previous";

Zblock zblocks[256];
long unsigned int zblock_counter = 0;

enum expectedState {
	NONE,
	ZBLOCK,
	BLOCK,
	BLOCK_SIGNATURE,
	DATA,
	ACTION,
	MODULE,
	DETACH,
	IPFS,
	PREVIOUS,
	GPG,
	TIMESTAMP,
	GENESIS
};

// We expect a ZBLOCK to come first but whatever :)
short next_statement = ZBLOCK;

short inner_detach_occurence = 0;

Data current_data;

Gpg current_gpg;
Action current_action;
Module current_module;
Timestamp current_timestamp;
DataSignature current_data_signature;
Previous current_previous;
DataLink current_data_link;
SignedDataLink current_signed_data_link;

//Block block = {current_data, current_gpg, current_action, current_module, current_timestamp.timestamp, current_data_signature, current_previous};

Block current_block;
//current_block->data=&current_data;


BlockSignature current_block_signature;
//BlockSignature *cbsp = &current_block_signature;
Zblock current_zblock;// = { .block_signature = current_block_signature; };

void lol(){
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	int a = 0;
	a++;
	printf("%s", AK_zblock	);
	check_tag(AK_zblock);
}

void check_tag(char *key){
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	if ( !strcmp(key,AK_zblock) ){
		next_statement = ZBLOCK;
//		printf("zblock");
	}
	else if ( !strcmp(key,AK_block) ){
		next_statement = BLOCK;
//		printf("block\t");
	}
	else if ( !strcmp(key,AK_action) ){
		next_statement = ACTION;
//		printf("action\t");
	}
	else if ( !strcmp(key,AK_ipfs) ){
		next_statement = IPFS;
//		printf("IPFS\t");
	}
	else if ( !strcmp(key,AK_detach) ){
		inner_detach_occurence++;
		next_statement = DETACH;
//		printf("detach[%i]\t",inner_detach_occurence);
	}
	else if ( !strcmp(key,AK_timestamp) ){
		next_statement = TIMESTAMP;
//		printf("timestamp\t");
	}
	else if ( !strcmp(key,AK_module) ){
		next_statement = MODULE;
//		printf("module\t");
	}
	else if ( !strcmp(key,AK_gpg) ){
		next_statement = GPG;
//		printf("gpg\t");
	}
	else if ( !strcmp(key,AK_previous) ){
		inner_detach_occurence = 0;
		next_statement = PREVIOUS;
//		printf("previous\t");
	}
	else if ( !strcmp(key,AK_block_signature) ){
		next_statement = BLOCK_SIGNATURE;
//		printf("blocksignature\t");
	}
	else if ( strlen(key) == 46 ) {
//		next_statement = DATA;
		assignValueToData(key, &current_data);
//		printf("data\t");
	}
	else if ( !strcmp(key,"genesis") ){
		next_statement = GENESIS;
	}
	else {
		printf("Aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa!!!!\n");
	}
}

void do_something_cause_with(char *value){
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	switch ( next_statement ){
		case ZBLOCK:
			assignValueToZblock(value,&current_zblock);
			break;
		case BLOCK:
			assignValueToBlock(value,&current_block);
			break;
		case BLOCK_SIGNATURE:
			assignValueToBlockSignature(value,&current_block_signature);
			break;
		case DATA:
			// Data *dp = current_data;
			if (DEBUG) printf(value);
			assignValueToData(value,&current_data);
			break;
		case ACTION:
			assignValueToAction(value,&current_action);
			break;
		case MODULE:
			assignValueToModule(value,&current_module);
			break;
		case DETACH:
			if (inner_detach_occurence == 1){
				assignValueToDataSignature(value, &current_data_signature);
			}
			else if (inner_detach_occurence == 2){
				assignValueToSignedDataLink(value, &current_signed_data_link);
			}
			else {
				fprintf(stderr,"ERROR: Unexpected level (%i) of detached signature: %s !", inner_detach_occurence, value);
				exit(1);
			}
			break;
		case IPFS:
			assignValueToDataLink(value, &current_data_link);
			break;
		case GPG:
			assignValueToGpg(value, &current_gpg);
			break;
		case TIMESTAMP:
			assignValueToTimestamp(value,&current_timestamp);
			break;
		case PREVIOUS:
			assignValueToPrevious(value,&current_previous);
			break;
		case NONE:
			if (DEBUG) printf("\n\033[2;35mNONE WAS CALLED!!!\n\033[0;0m");
			break;
		case GENESIS:
			if (DEBUG) printf("\n\033[2;35mGENESIS WAS CALLED!!!\n\033[0;0m");
			break;
	}
}
void assignValueToZblock(char *value, Zblock *z)
{
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	// z->self.ipfs = value;
	strncpy(z->self.ipfs, value, sizeof(z->self.ipfs));
	/*
	char *filename = "czb";
	FILE *f = fopen(filename,"wb");
	if ( !f ) {
		printf("ERROR: %s -> %s", filename, strerror(errno));
	}
	fwrite (z->self.ipfs, sizeof(*z),1,f);
	fclose(f);
	*/
	// if (DEBUG) printf("\n%s value is a Zblock!!!!\n", z->self.ipfs);
}

void assignValueToBlock(char *value, Block *z)
{
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	//z->self.ipfs = value;
	strcpy( z->self.ipfs , value);
	if (DEBUG) printf("\n%s value is a Block!!!!\n", z->self.ipfs);
}
void assignValueToBlockSignature(char *value, BlockSignature *z)
{
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	// z->self.ipfs = value;
	strcpy( z->self.ipfs , value);
	if (DEBUG) printf("\n%s value is a BlockSignature!!!!\n", z->self.ipfs);
}
void assignValueToData(char *value, Data *z)
{
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	// z->self.ipfs = value;
	strcpy( z->self.ipfs , value);
	if (DEBUG) printf("\n%s value is Data\n", z->self.ipfs);

}
void assignValueToAction(char *value, Action *z)
{
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	//(void)value;
	// z->action = value;
	strcpy(z->action, value);
	if (DEBUG) printf("\n%s value is Action\n", z->action);

}
void assignValueToModule(char *value, Module *z)
{
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	strcpy( z->module, value);
	if (DEBUG) printf("\nMODULE value .... %s --> %s\n",value,z->module);
	if (DEBUG) printf("\n%s value is Module\n", z->module);

}
void assignValueToTimestamp(char *value, Timestamp *z)
{
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
//	z->timestamp = ch2int(value, strlen(value)+1);
	strcpy(z->timestamp, value);
	if (DEBUG) printf("\n%s was casted from %s is Timestamp\n", z->timestamp, value);
}
void assignValueToPrevious(char *value, Previous *z)
{
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	strcpy(z->self.ipfs, value);
	if (DEBUG) printf("\n%s value is Previous\n", z->self.ipfs);
	complete_the_zblock();
}
void assignValueToGpg(char *value, Gpg *z)
{
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	strcpy(z->self.ipfs, value);
	if (DEBUG) printf("\n%s value is Gpg\n", z->self.ipfs);
}
void assignValueToDataSignature(char *value, DataSignature *z)
{
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	strcpy(z->self.ipfs , value);
	if (DEBUG) printf("\n%s value is DataSignature\n", z->self.ipfs);
}
void assignValueToSignedDataLink(char *value, SignedDataLink *z)
{
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	strcpy(z->self.ipfs, value);
	if (DEBUG) printf("\n%s value is SignedDataLink\n", z->self.ipfs);
}
void assignValueToDataLink(char *value, DataLink *z)
{
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	strcpy(z->self.ipfs, value);
	if (DEBUG) printf("\n%s value is DataLink\n", z->self.ipfs);
}

void complete_the_zblock()
{
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	current_data.data_link = &current_data_link;
	current_data.signed_data_link = &current_signed_data_link;
	current_block.timestamp = &current_timestamp;
	current_block.action = &current_action;
	current_block.module = &current_module;
	current_block.data = &current_data;
	current_block.gpg = &current_gpg;
	current_block.data_signature = &current_data_signature;
	current_block.previous = &current_previous;
	current_zblock.block = &current_block;
	current_zblock.block_signature = &current_block_signature;
	Zblock *final = &current_zblock;
	zblock_counter++;
	append_zblock(final,sizeof(&final));
//	printf("zblock: %s\nblock: %s\nblock_signature: %s\ntimestamp: %lli\nmodule: %s\naction: %s\ngpg: %s\ndata: %s\ndata_link: %s\nsigned_data_link: %s\ndata_signature: %s\nprevious: %s\n",
//		current_zblock.self.ipfs,
//		current_zblock.block->self.ipfs,
///		current_zblock.block_signature->self.ipfs,
//		current_zblock.block->timestamp->timestamp,
//		current_zblock.block->module->module,
//		current_zblock.block->action->action,
//		current_zblock.block->gpg->self.ipfs,
//		current_zblock.block->data->self.ipfs,
//		current_zblock.block->data->data_link.ipfs,
//		current_zblock.block->data->signed_data_link.ipfs,
//		current_zblock.block->data_signature->self.ipfs,
//		current_zblock.block->previous->self.ipfs);
	print_zblock(final);
}

void print_zblock(Zblock *z)
{
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	printf("zblock: %s\nblock: %s\nblock_signature: %s\ntimestamp: %s\nmodule: %s\naction: %s\ngpg: %s\ndata: %s\ndata_link: %s\nsigned_data_link: %s\ndata_signature: %s\nprevious: %s\n",
	z->self.ipfs,
	z->block->self.ipfs,
	z->block_signature->self.ipfs,
	z->block->timestamp->timestamp,
	z->block->module->module,
	z->block->action->action,
	z->block->gpg->self.ipfs,
	z->block->data->self.ipfs,
	z->block->data->data_link->self.ipfs,
	z->block->data->signed_data_link->self.ipfs,
	z->block->data_signature->self.ipfs,
	z->block->previous->self.ipfs);
	printf("=================================================================================\n");
}

void append_zblock(Zblock *z, long unsigned int s)
{
	//printf("\nTrying to add ZBLOCK:%s with %lu\t%p\n", z->self.ipfs, s, (void*)zblocks);
	//*(*zblocks+zblock_counter) = *z;
	//zblocks++;
	//printf("%p\n",(void*)zblocks);
	//printf("---------------------------------------------------------------------------------\n");
	(void)s;
	char *filename = z->self.ipfs;
	FILE *f = fopen(filename,"wb");
	if (!f) {
		printf("ERROR");
	} 
	fwrite( z->self.ipfs,                                    sizeof(z->self.ipfs),1,f);
	fwrite( z->block->self.ipfs,                             sizeof(z->block->self.ipfs),1,f);
	fwrite( z->block_signature->self.ipfs,                   sizeof(z->block_signature->self.ipfs),1,f);
	fwrite( z->block->timestamp->timestamp,                  sizeof(z->block->timestamp->timestamp),1,f);
	fwrite( z->block->module->module,                        sizeof(z->block->module->module),1,f);
	fwrite( z->block->action->action,                        sizeof(z->block->action->action),1,f);
	fwrite( z->block->gpg->self.ipfs,                        sizeof(z->block->gpg->self.ipfs),1,f);
	fwrite( z->block->data->self.ipfs,                       sizeof(z->block->data->self.ipfs),1,f);
	fwrite( z->block->data->data_link->self.ipfs,            sizeof(z->block->data->data_link->self.ipfs),1,f);
	fwrite( z->block->data->signed_data_link->self.ipfs,     sizeof(z->block->data->signed_data_link->self.ipfs),1,f);
	fwrite( z->block->data_signature->self.ipfs,             sizeof(z->block->data_signature->self.ipfs),1,f);
	fwrite( z->block->previous->self.ipfs,                   sizeof(z->block->previous->self.ipfs),1,f);
	fclose(f);
}
// k51qzi5uqu5dlocpfn8w8wpi55w2uqilcuq86ihoi5ksfjz9sgqgva06prfgbg
