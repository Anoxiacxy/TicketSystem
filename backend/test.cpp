#include <iostream>
#include "string.hpp"
using namespace std;
int main(){
	sjtu::string<5> name;
	cin >> name;
	int a = (name[0]-'0')*10 + name[1]-'0';
	cout << a;
	return 0;
} 
