#include <iostream>
#include <cstdio>
#include "bptree_fixed.hpp"
#include "structs.hpp"

struct Key {
	int a, b;
	bool operator < (const Key &other) const {
		return (a < other.a) || (a == other.a && b < other.b);
	}
	Key() = default;
	Key(const int &_a, const int &_b) : a(_a), b(_b) { }
	Key(const Key &other) {
		a = other.a;
		b = other.b;
	}
	void print() {
		printf("(a, b) = (%d, %d)\n", a, b);
	}
};

struct Val {
	int p;
	Val(const int &_p) : p(_p) { }
	Val(const Val &other) : p(other.p) { }
	void print() {
		printf("p = %d\n", p);
	}
};

bool cleaned = false;

int main() {
	// std::cerr << "fuck" << std::endl;
	sjtu::bptree<str<char, 20>, Val> tr("fuck", "fucking");
	// std::cerr << "fuck" << std::endl;
	str<char, 20> pp;
	char mmp[20];
	pp = mmp;
	tr.insert(pp, 100);
	printf("%d\n", tr.count(pp));
	tr.clear();
}