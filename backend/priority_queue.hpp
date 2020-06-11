#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

template <typename T> void swap(T &a, T &b) {
	T x = a;
	a = b;
	b = x;
}

/**
 * a container like std::priority_queue which is a heap internal.
 */
template<typename T, class Compare = std::less<T>>
class priority_queue {
	size_t sz;

	struct node {
		T value;
		node *left, *right;

		node(const T &_value) : value(_value) {
			left = right = NULL;
		}
	};

	node *merge(node *x, node *y) {
		if (x == NULL) return y;
		if (y == NULL) return x;
		if (Compare()(x->value, y->value)) swap(x, y);
		swap(x->left, x->right);
		x->left = merge(x->left, y);
		return x;
	}

	void clear(node *x) {
		if (x != NULL) {
			clear(x->left);
			clear(x->right);
		}
		delete x;
	}

	node *copy(node *y) {
		if (y == NULL) return NULL;
		node *cur = new node(y->value);
		cur->left = copy(y->left);
		cur->right = copy(y->right);
		return cur;
	}

	void remove() {
		--sz;
		node *l = root->left;
		node *r = root->right;
		delete root;
		root = merge(l, r);
	}

	void clear_all() {
		clear(root);
		sz = 0;
		root = NULL;
	}

	void clear_link() {
		sz = 0;
		root = NULL;
	}

	node *root;
public:
	/**
	 * TODO constructors
	 */
	priority_queue() {
		sz = 0;
		root = NULL;
	}
	priority_queue(const priority_queue &other) {
		// if (this == &other) return *this;
		// clear(root);
		sz = other.sz;
		root = copy(other.root);
	}
	/**
	 * TODO deconstructor
	 */
	~priority_queue() {
		clear_all();
	}
	/**
	 * TODO Assignment operator
	 */
	priority_queue &operator=(const priority_queue &other) {
		if (this == &other) return *this;
		clear_all();
		sz = other.sz;
		root = copy(other.root);
		return *this;
	}
	/**
	 * get the top of the queue.
	 * @return a reference of the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	const T & top() const {
		// std::cerr << sz << "\n";
		if (sz == 0) {
			throw container_is_empty();
		}
		// std::cerr << sz << "\n";
		return root->value;
	}
	/**
	 * TODO
	 * push new element to the priority queue.
	 */
	void push(const T &e) {
		node *x = new node(e);
		++sz;
		root = merge(root, x);
		// std::cerr << root->value << std::endl;
		// std::cerr << sz << "\n";
	}
	/**
	 * TODO
	 * delete the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	void pop() {
		// std::cerr << sz << "\n";
		if (sz == 0) {
			throw container_is_empty();
		}
		remove();
	}
	/**
	 * return the number of the elements.
	 */
	size_t size() const {
		return sz;
	}
	/**
	 * check if the container has at least an element.
	 * @return true if it is empty, false if it has at least an element.
	 */
	bool empty() const {
		return sz == 0;
	}
	/**
	 * return a merged priority_queue with at least O(logn) complexity.
	 */
	void merge(priority_queue &other) {
		sz += other.sz;
		root = merge(root, other.root);
		other.clear_link();
	}
};

}

#endif
