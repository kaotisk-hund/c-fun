#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "casting.h"
#include "config.h"

sint ch2int(char *str, long int size) {
	if (DEBUG) printf("[ch2int] Size of str: %li\n", size);
	int length_output = size-1;
	if (DEBUG) printf("[ch2int] Expected length_output: %li\n", size);
	assert( length_output <= 11);
	sint output = 0;
	for (int i = 0; i < size; i++){
		if (str[i]!='\0'){
			switch (str[i]){
				case '0':
					output += 0 * (int)pow(10.0,(double)length_output-i-1);
					break;
				case '1':
					output += 1 * (int)pow(10.0,(double)length_output-i-1);
					break;
				case '2':
					output += 2 * (int)pow(10.0,(double)length_output-i-1);
					break;
				case '3':
					output += 3 * (int)pow(10.0,(double)length_output-i-1);
					break;
				case '4':
					output += 4 * (int)pow(10.0,(double)length_output-i-1);
					break;
				case '5':
					output += 5 * (int)pow(10.0,(double)length_output-i-1);
					break;
				case '6':
					output += 6 * (int)pow(10.0,(double)length_output-i-1);
					break;
				case '7':
					output += 7 * (int)pow(10.0,(double)length_output-i-1);
					break;
				case '8':
					output += 8 * (int)pow(10.0,(double)length_output-i-1);
					break;
				case '9':
					output += 9 * (int)pow(10.0,(double)length_output-i-1);
					break;
				default:
					break;
			}
			if (DEBUG) printf("[ch2int] %llu %c -> %f\n", output,str[i], pow(10.0,(double)length_output-i-1));
		}
	}
	return output;
}

char* int2ch(sint s)
{
	if (DEBUG) printf("\nCONVERTING INTEGER TO CHAR*\n");
	assert ( s < 9999999999 && s > 1000000000);
	char *result = malloc(11);
	sint temp = 0;
	int counter = 0;
	for ( sint i = 1000000000; i > 0; i=i/10){
		temp = (s/i);
		s = s - (s/i)*i;
		if (DEBUG) printf("%lli \t", temp);
		switch(temp){
			case 0:
				result[counter] = '0';
				break;
			case 1:
				result[counter] = '1';
				break;
			case 2:
				result[counter] = '2';
				break;
			case 3:
				result[counter] = '3';
				break;
			case 4:
				result[counter] = '4';
				break;
			case 5:
				result[counter] = '5';
				break;
			case 6:
				result[counter] = '6';
				break;
			case 7:
				result[counter] = '7';
				break;
			case 8:
				result[counter] = '8';
				break;
			case 9:
				result[counter] = '9';
				break;

		}
		counter++;
	}
	result[10] = '\0';
	if (DEBUG) printf("%lli",s);
	if (DEBUG) printf("\n--------------------------\n");
	return result;
}
