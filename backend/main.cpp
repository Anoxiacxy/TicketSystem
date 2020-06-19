#include "interpreter1.hpp"
using namespace sjtu;
bool cleaned = false;
int main(){
	std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
//    std::ifstream in("1.txt");
//    std::streambuf *backinbuf = std::cin.rdbuf();
//    std::cin.rdbuf(in.rdbuf()); 
//    std::ofstream out("out.txt");
//	std::streambuf *default_buf=std::cout.rdbuf();
//	std::cout.rdbuf( out.rdbuf() ); 
	Interpreter I;
	I();
//	std::cout.rdbuf( default_buf );
//	std::cin.rdbuf( backinbuf );
	return 0; 
}
