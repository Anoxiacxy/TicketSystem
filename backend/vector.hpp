#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"
#include <climits>
#include <cstddef>
#include <new>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector {
private:
    T *data;
	int currentsize;
	int maxsize;
	void doublespace(){
		T *tmp = data;
		maxsize *= 2;
		data = (T*)malloc(sizeof(T)*maxsize);
		for(int i = 0; i < currentsize; ++i){
			data[i] = tmp[i];
		}
		free(tmp);
	}
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	enum{space_capacity = 16};
	class const_iterator;
	class iterator {
		friend class vector;
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		vector *vec;
		int pos;
	public:
	    iterator(vector *Vec = NULL, int Pos = 0):vec(Vec),pos(Pos){}
		iterator(const iterator &other):vec(other.vec), pos(other.pos){}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		iterator operator+(const int &n) const {
			iterator tmp = *this;
			tmp.pos += n;
			return tmp;
			//TODO
		}
		iterator operator-(const int &n) const {
			iterator tmp = *this;
			tmp.pos -=n;
			return tmp;
			//TODO
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
			return pos-rhs.pos;
			//TODO
		}
		iterator& operator+=(const int &n) {
			pos += n;
			return *this;
			//TODO
		}
		iterator& operator-=(const int &n) {
			pos -= n;
			return *this;
			//TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator tmp = *this;
			++*this;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			++pos;
			return *this; 
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator tmp = *this;
			--*this;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			--pos;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return vec->data[pos];
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
			return pos == rhs.pos && vec == rhs.vec;
		}
		bool operator==(const const_iterator &rhs) const {
			return pos == rhs.pos && vec == rhs.vec;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(*this == rhs);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(*this == rhs);
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator {
	private:
	    vector *vec;
		int pos;
	public:
	    const_iterator(vector *Vec = NULL, int Pos = 0):vec(Vec),pos(Pos){}
		const_iterator(const const_iterator &other):vec(other.vec),pos(other.pos){}
		const_iterator operator+(const int &n) const {
			const_iterator tmp = *this;
			tmp.pos += n;
			return tmp;
			//TODO
		}
		const_iterator operator-(const int &n) const {
			const_iterator tmp = *this;
			tmp.pos -=n;
			return tmp;
			//TODO
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const {
			return pos-rhs.pos;
			//TODO
		}
		const_iterator& operator+=(const int &n) {
			pos += n;
			return *this;
			//TODO
		}
		const_iterator& operator-=(const int &n) {
			pos -= n;
			return *this;
			//TODO
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int) {
			const_iterator tmp = *this;
			++*this;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++() {
			++pos;
			return *this; 
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) {
			const_iterator tmp = *this;
			--*this;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--() {
			--pos;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return vec->data[pos];
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
			return pos == rhs.pos && vec == rhs.vec;
		}
		bool operator==(const const_iterator &rhs) const {
			return pos == rhs.pos && vec == rhs.vec;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(*this == rhs);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(*this == rhs);
		}
	};
	/**
	 * TODO Constructs
	 * Atleast three: default constructor, copy constructor and a constructor for std::vector
	 */
public:
	vector(int Maxsize = 16, int Currentsize = 0):maxsize(Maxsize),currentsize(Currentsize) {
		data=(T*)malloc(sizeof(T)*maxsize);
	}
	vector(const vector &other):maxsize(other.maxsize),currentsize(other.currentsize) {
		data=(T*)malloc(sizeof(T)*maxsize);
		for(int i = 0; i < currentsize; ++i){
			data[i] = other.data[i];
		} 
	}
	
	/**
	 * TODO Destructor
	 */
	~vector() {
		for(int i = 0; i < currentsize; ++i) data[i].~T();
		free(data);
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		for(int i = 0; i < currentsize; ++i) data[i].~T();
		free(data);
		maxsize = other.maxsize;
		currentsize = other.currentsize;
		data = (T*)malloc(sizeof(T)*maxsize);
		for(int i = 0; i < currentsize; ++i){
			data[i] = other.data[i];
		}
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */

	T & at(const size_t &pos) {
		if(pos < 0 || pos > currentsize) throw index_out_of_bound();
		return data[pos];
	}
	const T & at(const size_t &pos) const {
		if(pos < 0 || pos > currentsize) throw index_out_of_bound();
		return data[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {return at(pos);}
	const T & operator[](const size_t &pos) const {return at(pos);}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if(currentsize == 0) throw container_is_empty();
		return data[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if(currentsize == 0) throw container_is_empty();
		return data[currentsize-1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(this, 0);
	}
	const_iterator cbegin() const {
		return const_iterator(const_cast<vector*>(this), 0);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(this, currentsize);
	}
	const_iterator cend() const {
		return const_iterator(const_cast<vector*>(this),currentsize);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return currentsize == 0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return currentsize;
	}
	/**
	 * returns the number of elements that can be held in currently allocated storage.
	 */
	size_t capacity() const {
		return maxsize;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		currentsize = 0;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator Pos, const T &value) {
		if(currentsize == maxsize) doublespace();
		for(int i = currentsize; i > Pos.pos; --i){
			data[i] = data[i-1];
		}
		data[Pos.pos] = value;
		currentsize++;
		return iterator(this, Pos.pos);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value is true
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if(ind > currentsize) throw index_out_of_bound();
		if(currentsize == maxsize) doublespace();
		for(int i = currentsize; i > ind; --i){
			data[i] = data[i-1];
			data[i] = value;
			++currentsize;
		}
		return iterator(this, ind);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator Pos) {
		for(int i = Pos.pos; i < currentsize-1; ++i){
			data[i] = data[i+1];
		}
		currentsize--;
		return iterator(this, Pos.pos);
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if(ind >= currentsize) throw index_out_of_bound();
		for(int i = ind; i < currentsize-1; ++i){
			data[i] = data[i+1];
		}
		currentsize--;
		return iterator(this, ind);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		if(maxsize == currentsize) doublespace();
		new (&data[currentsize])T(value);
		currentsize++;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if(currentsize == 0) throw container_is_empty();
		currentsize--;
	}
};


}

#endif
