#ifndef SJTU_ARRAY_HPP
#define SJTU_ARRAY_HPP

#include "exceptions.hpp"
#include <climits>
#include <cstddef>
#include <new>

namespace sjtu{
	template<typename T, int Size>
	class array{
	private:
		int currentlength;
		T data[Size];
	public:
	class const_iterator;
	class iterator{
		friend class const_iterator;
		friend class array;
	private:
		array *arr;
		int pos;
	public:
	    iterator(array *ARR, int POS):arr(ARR),pos(POS){}
	    iterator(const iterator &other):arr(other.arr),pos(other.pos){}
	    inline iterator operator+(const int &n) const{
	    	iterator tmp = *this;
	    	tmp.pos += n;
	    	return tmp;
		}
		inline iterator operator-(const int &n) const{
	    	iterator tmp = *this;
	    	tmp.pos -= n;
	    	return tmp;
		}
		inline int operator-(const iterator &rhs)const{
			return pos - rhs.pos;
		}
		inline iterator operator +=(const int &n)const{
			pos += n;
			return *this;
		}
		inline iterator operator -=(const int &n)const{
			pos -= n;
			return *this;
		}
		inline iterator operator++ (int){
			iterator tmp = *this;
			++(*this);
			return tmp;
		}
		inline iterator &operator++(){
			++pos;
			return *this;
		}
			inline iterator operator-- (int){
			iterator tmp = *this;
			--(*this);
			return tmp;
		}
		inline iterator &operator--(){
			--pos;
			return *this;
		}
		inline T &operator*() const{
			return arr->data[pos];
		}
		inline bool operator==(const iterator &rhs)const{
			return arr == rhs.arr && pos == rhs.pos;
		}
		inline bool operator==(const const_iterator &rhs)const{
			return arr == rhs.arr && pos == rhs.pos;
		}
		inline bool operator!=(const iterator &rhs)const{
			return !(*this == rhs);
		}
		inline bool operator!=(const const_iterator &rhs)const{
			return !(*this == rhs);
		}
	};
	class const_iterator:public iterator{
		friend class iterator;
		friend class array;
	public:
		const_iterator(array *ARR, int POS):iterator(ARR, POS){}
		const_iterator(const const_iterator &other):iterator(other){};
		const_iterator(const iterator &other):iterator(other){}; 
	};
	array(){
		currentlength = 0;
	}
	array(const array &other){
	    currentlength = other.currentlength;
		for(int i = 0; i < currentlength; ++i) data[i] = other.data[i];	
	}
	~array(){}
	array &operator= (const array &other){
		currentlength = other.currentlength;
		for(int i = 0; i < currentlength; ++i) data[i] = other.data[i];
	}
	inline T & at(const size_t &pos) {return data[pos];}
	inline const T & at(const size_t &pos) const {return data[pos];}
	inline T & operator[](const size_t &pos) {return at(pos);}
	inline const T & operator[](const size_t &pos) const {return at(pos);}
	inline const T & front() const {return data[0];}
	inline const T & back() const {return data[size()-1];}
	inline iterator begin() const{return iterator(this, 0);}
	inline const_iterator cbegin() const{return const_iterator(begin());}
	inline iterator end() const{return iterator(this, size());}
	inline const_iterator cend() const{return const_iterator(end());}
	inline bool empty() const {return currentlength == 0;}
	inline size_t size() const {return currentlength;}
	inline size_t capacity() const {return Size;}
	inline void clear(){currentlength = 0;}
	inline void push_back(const T &value){
		data[currentlength] = value;
		++currentlength;
	}
	inline T pop_back(){
		--currentlength;
		return data[currentlength];
	}
    };
}
#endif
