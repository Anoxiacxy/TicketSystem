#include "interpreter.hpp"

using namespace sjtu;

int main(){
	char a[100000];
	std::ifstream in("1.in");
	if(in.is_open()){
		while(!in.eof()){
			in.getline(a, 100000, '&');
		}
	}
	Interpreter I;
	I(a);
	return 0; 
}
