#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include "priority_queue.hpp"
using namespace sjtu;
int main(){
	priority_queue<int> q;
	q.push(1);
	q.push(2);
	std::cout << q.top();
 	return 0;
}
