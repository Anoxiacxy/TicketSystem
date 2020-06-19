#include "file_vector.hpp"
#include <cstdio>
#include <cstdlib>
#include "file_exceptions.hpp"


bool cleaned = false;
char test_vector_name[] = "test_vector_name";

int main() {
	SJTU::file_vector<int, test_vector_name> vec;
//	 vec._init();
	for (int i = 0; i < 1000000; ++i) vec.push_back(i);
	for (int i = 0; i < vec.size(); ++i) printf("%d ", vec[i]);
//	puts("");
//	vec.modify(0, 200);
//	for (int i = 0; i < 20; ++i) printf("%d ", vec[i]);
//	puts("");

//	printf("%d %d\n", vec.front(), vec.back());

//	printf("%d\n", vec.size());

//	vec.clean();
//	vec.init();

//	printf("%d\n", vec.size());

//	vec.clean();
}
