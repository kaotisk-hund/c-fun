/*
 * JSON parser attempt
 *
 * Notes:
 *
 * 	Valid patterns:
 * 		[
 *			"value",
 *			{"key":"value"},
 *			["more","values"],
 *			{"keys":["many","mannyyyy","values"]}
 * 		]
 *
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "akstuff.h"
#include "config.h"
/*#include "🌲.h" */
 
#define ARRAYSIZE(arr) sizeof(arr)/sizeof(arr[0])

enum JSON_SC {
	NONE,
	ARRAY_OPENING,
	ARRAY_CLOSING,
	OBJECT_OPENING,
	OBJECT_CLOSING,
	ENCAPSULATOR,
	ASSIGNER,
	SEPARATOR,
	ESCAPECHAR,
	TAGFIELD,
	SPACEBAR,
	NEWLINE
};

char JSON_opening_array = '[';
char JSON_closing_array = ']';
char JSON_opening_object = '{';
char JSON_closing_object = '}';
char JSON_encapsulator = '"';
char JSON_escape_char = '\\';
char JSON_assign_value = ':';
char JSON_comma_separator = ',';
char JSON_new_line_char = '\n';
char JSON_spacebar = ' ';

int commas_found = 0;
int newlines_found = 0;
int assignings_found = 0;
int escapechars_found = 0;
int spacebars_found = 0;
int arrayopenings, arrayclosings = 0;
int objectopenings, objectclosings = 0;
int encapsulations = 0;

bool inside_an_object = false;
bool inside_an_array = false;
bool encap_on = false;
bool assigning_started = false;
int arrlvl, objlvl = 0;

char key_string[64];
char value_string[128];

size_t lsp = 0;
size_t vsp = 0;

int previous_state = NONE;
int preprevious_state = NONE;
int prepreprevious_state = NONE;
int preprepreprevious_state = NONE;
int current_state = NONE;

int prvarr[56];
int prearr[56];
int nest_depth = 0;
int max_nest = 0;

void isMaxNestDepth( int a ){
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	if (max_nest < a) max_nest = a;
}

void increaseNestDepth(){
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	nest_depth++;
	isMaxNestDepth(nest_depth);
}

void decreaseNestDepth(){
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	nest_depth--;
}

void initPreviousesArray(){
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	for (int i = 0; i < 56; i++){
		prvarr[i]=NONE;
	}
}

void appendPrevious(int a){
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	if (prvarr[0] != a){
		for (int i = 55; i > 0; i--){
			prvarr[i]=prvarr[i-1];
		}
		prvarr[0] = a;
	}
}

void printPrevious(){
//	printf("\n");
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	for(int i = 0;i<56;i++)
	{
			printf("%i ",prvarr[i]);
	}
	printf("\n");
}

void check_key(char *key)
{
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	//printf("key: %s [%li]", key, strlen(key));
	check_tag(key);
	//Block i;
	//strcpy( i.self.ipfs, "lol");
	//(void)i;
//	printf("%s", i.ipfs.ipfs);

}

void readAsKey(char ch)
{
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	if (ch != '\0') {
		if (lsp == 0 || lsp > 0 ) {
			key_string[lsp]=ch;
			++lsp;
		}
	}
	else {
//		for (int i = 0; i< arrlvl; i++){
//			printf("\t");
//		}
//		for (int i = 0; i< objlvl; i++){
//			printf("\t");
//		}
		if (DEBUG) printf("\033[0;37mKey: \033[1;33m%s\t\033[0;32m", key_string);
		check_key(key_string);
		for ( size_t i = 0; i < 64; ++i){
			key_string[i] = '\0';
		}
		lsp = 0;
	}
}

void readAsValue(char ch)
{ 
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	if (ch != '\0') {
		if (vsp == 0 || vsp > 0 ) {
			value_string[vsp]=ch;
			++vsp;
		}
	}
	else {
//		if(!(arrlvl >= 1 && inside_an_array)){
//			assigning_started = false;
//		}
//		if(!(objlvl >= 0 && inside_an_object)){
//			assigning_started = false;
//		}
		//for (int i = 0; i< arrlvl; i++){
		//	printf("\t");
		//}
		//for (int i = 0; i< objlvl; i++){
		//	printf("\t");
		//}
		if (DEBUG) printf("\t\033[0;37mValue: \033[4;34m%s\n\033[0;32m", value_string);

		do_something_cause_with(value_string);
		for ( size_t i = 0; i < 128; ++i){
			value_string[i] = '\0';
		}
		vsp = 0;
	}
}

void send_to_buffer(char ch)
{ 
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	if (!assigning_started) {
		readAsKey(ch);
	}
	else {
		readAsValue(ch);
	}
}

void encapsulation()
{ 
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	encapsulations++;
	if(encapsulations % 2 != 0){
		encap_on = true;
	}
	else {
		encap_on = false;
		send_to_buffer('\0');
	}
}
void separator(){
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	if (prvarr[3]==6 || prvarr[0]==4 || prvarr[0]==1) assigning_started = false;
	commas_found++;
}
void spacebar(){
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	spacebars_found++;
	if (prvarr[0] == TAGFIELD) send_to_buffer(' ');
}
void newLine(){
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	newlines_found++;
	send_to_buffer('\0');
}

void assigner()
{ 
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	assignings_found++;
	assigning_started = true;
}

void escapeChar()
{ 
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	escapechars_found++;
}

void objectOpening(){
	increaseNestDepth();
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	assigning_started = false;
	if (inside_an_object){
		objlvl++;
	}
	inside_an_object = true;
//	printf("\n");
	objectopenings++;
}

void arrayOpening()
{ 
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	increaseNestDepth();
//	printf("\n\033[0;35mOpening array\033[0;32m\n");
	if (inside_an_array){
		arrlvl++;
	}
	inside_an_array = true;
	assigning_started = true;
	arrayopenings++;
//	printf("\n");
}

void arrayClosing()
{ 
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	decreaseNestDepth();
//	printf("\n\033[0;35mClosing array\033[0;32m\n");
	if(arrlvl > 0){
		arrlvl--;
	}
	else if(arrlvl == 0 && inside_an_array){
		inside_an_array = false;
	}
	if(assigning_started){
		assigning_started = false;
	}
	arrayclosings++;
}

void objectClosing()
{ 
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	decreaseNestDepth();
//	printf("\n\033[0;35mClosing object\033[0;32m\n");
	if(objlvl > 0){
		objlvl--;
	}
	else if(objlvl == 0 && inside_an_object){
		inside_an_object = false;
	}
	objectclosings++;
}

int check_what_is_what(char ch)
{ 
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	if (ch == JSON_comma_separator){
		return SEPARATOR;
	}
	else if (ch == JSON_new_line_char){
		return NEWLINE;
	}
	else if (ch == JSON_assign_value){
		return ASSIGNER;
	}
	else if (ch == JSON_escape_char){
		return ESCAPECHAR;
	}
	else if (ch == JSON_opening_array){
		return ARRAY_OPENING;
	}
	else if (ch == JSON_closing_array){
		return ARRAY_CLOSING;
	}
	else if (ch == JSON_opening_object){
		return OBJECT_OPENING;
	}
	else if (ch == JSON_closing_object){
		return OBJECT_CLOSING;
	}
	else if (ch == JSON_encapsulator){
		return ENCAPSULATOR;
	}
//	else if (ch == JSON_spacebar){
//		return SPACEBAR;
//	}
	else {
		return TAGFIELD;
	}
}

void shift()
{ 
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	printf("Shifting: %i %i %i %i %i\t -> \t", preprepreprevious_state, prepreprevious_state, preprevious_state, previous_state, current_state);
	preprepreprevious_state = prepreprevious_state;
	prepreprevious_state = preprevious_state;
	preprevious_state = previous_state;
	previous_state = current_state;
	printf("%i %i %i %i %i\n", preprepreprevious_state, prepreprevious_state, preprevious_state, previous_state, current_state);
}

void stats(char ch)
{ 
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	printf("%i, %c:\nOIN |\tAIN |\tENO |\tASS |\tALV |\tOLV |\tCST |\n%i\t%i\t%i\t%i\t%i\t%i\t%i\n->\t%s :: %s <=%i\n",current_state,ch, inside_an_object, inside_an_array, encap_on, assigning_started, arrlvl, objlvl, current_state, key_string, value_string, nest_depth);
}

void dosome(char *str)
{ 
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
//	printf("%s",str);
	for (size_t i = 0; i < strlen(str); ++i){
		current_state = check_what_is_what(str[i]);
		switch (current_state){
			case ARRAY_OPENING:
				arrayOpening();
				break;
			case ARRAY_CLOSING:
				arrayClosing();
				break;
			case OBJECT_OPENING:
				objectOpening();
				break;
			case OBJECT_CLOSING:
				objectClosing();
				break;
			case ASSIGNER:
				assigner();
				break;
			case SEPARATOR:
				separator();
				break;
			case NEWLINE:
				newLine();
				break;
			case ENCAPSULATOR:
				encapsulation();
				break;
			case TAGFIELD:
				send_to_buffer(str[i]);
				break;
	//		case SPACEBAR:
	//			spacebar();
	//			break;
			default:
			//	send_to_buffer(str[i]);
			//	stats(str[i]);
				printf("Doing nothing...\n");
				break;
		};
		appendPrevious(current_state);
	//	printPrevious();
	}
}


void parse_attempt(char **arg)
{ 
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	for ( long unsigned int i = 0; i <sizeof(*arg);++i){
		if (arg[i]==NULL){
			break;
		}
		dosome(arg[i]);
	}
}

int main(int argc, char **argv)
{ 
	if (DEBUG) printf("\n\033[2;35mCalling: %s\n\033[0;0m",__func__);
	++argv;
	(void)argc;
	initPreviousesArray();
	/*
	FILE * f = fopen("data3.json","r");
	if ( !f ) {
		printf("ERROR");
	}
	char *buffer[1];
	size_t l;
       	while ( (l = fread( buffer, sizeof(*buffer), ARRAYSIZE(buffer), f ) != 0) ){
		parse_attempt(buffer);
	}
	fclose(f);
	*/
        while (*argv!=NULL){
		parse_attempt(argv);
               ++argv;
        }
	if (DEBUG) printf("\nCommas found: %i\nNew lines found: %i\nAssingings found: %i\nEscapes found: %i\nArray::openings: %i\t::closings: %i\nObject::openings: %i\t::closings: %i\nEncapsulations: %i\nSpaceBars: %i\nEven encapsulations (1=true, 0=false): %i\nMax depth: %i\n\n\033[0;00m", 
			commas_found,
			newlines_found,
			assignings_found,
			escapechars_found,
			arrayopenings,
			arrayclosings,
			objectopenings,
			objectclosings,
			encapsulations,
			spacebars_found,
			((encapsulations % 2)==0)?true:false,
			max_nest
		);
	return 0;
}
