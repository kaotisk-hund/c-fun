#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int c, char ** v){
	string a = "Welcome To our argumentor program, woooooooow\nPrint any arguments given from command line to a line per argument sorted as they were typed!!\n\n";
	vector<string> dd = {};
	for ( int i = 0; i < c; i++ ){
		string my (v[i]);
		dd.push_back(my);
	}
	cout << &a << endl;
	cout << a << endl;
	for ( long unsigned int i = 0; i < dd.size(); i++ )
		cout << dd[i] << endl;
	return 0;
}
