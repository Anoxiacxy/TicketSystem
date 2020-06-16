#include "interpreter.hpp"
using namespace sjtu;
char a[90000000]; 
int main(){
	std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    std::ofstream outf("out.txt");
	std::ifstream in("1.txt");
	std::streambuf *default_buf=std::cout.rdbuf();
	std::cout.rdbuf( outf.rdbuf() ); 
	if(in.is_open()){
		while(!in.eof()){
			in.getline(a, 90000000, '&');
		}
	}
	Interpreter I;
	I(a);
	std::cout.rdbuf( default_buf ); 
	return 0; 
}
