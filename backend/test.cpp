#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include "interpreter.hpp"
using namespace std;
	char time1[] = "station1";
bool cleaned = false;
/*class B{
	public:
		sjtu::bptree<int , int> ticket_user_train_tree;
	public:
		B(): ticket_user_train_tree("ticket_user_train_tree1", "ticket_user_train_index1"){}
};
class A{
	public:
		    sjtu::bptree<sjtu::user_order, sjtu::ticket_user_train> ticket_user_train_tree;
	public:
		A(): ticket_user_train_tree("ticket_user_train_tree", "ticket_user_train_index"){}
};
class C{
	public:
		B b;
		A a;
	public:
		C():a(), b(){} 
		~C(){
			std::cout << a.ticket_user_train_tree.finder.blank << std::endl;
			std::cout << b.ticket_user_train_tree.finder.blank << std::endl;
		}
};*/
int main(){
		std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    std::ofstream outf("out.txt");
//	std::ifstream in("1.txt");
	std::streambuf *default_buf=std::cout.rdbuf();
	std::cout.rdbuf( outf.rdbuf() ); 
	sjtu::bptree<int, int> T("A", "B");
	std::cerr << "Answer:" << T.count(1) << std::endl;
	T.insert(1, 1);
//	sjtu::bptree<int, int> T3("A2", "B2");
//	for(int i = 0; i <500000; ++i){
//		T.insert(i,i);
//		T3.insert(i,-i);
//	}
//	auto it = T.lower_bound(0);
//	for(; it != T.end(); ++it){
//		std::cout << *(it) << std::endl;
//	}
//	T.traverse();
//C T;
	std::cout.rdbuf( default_buf ); 
 	return 0;
}
