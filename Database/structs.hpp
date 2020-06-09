#include <cstring>
#include <cstdio>
#include <iostream>
#include <iomanip>

using namespace std;

#ifndef __STRUCTS_FOR_TEST__
#define __STRUCTS_FOR_TEST__

template <class T, int N> struct str {
	T data[N];

	str() = default;
	str(int x) {
		if (x != 0) throw 1;
		for (int i = 0; i < N; ++i)
			data[i] = 0;
	}

	str(T _data[]) {
		memcpy(data, _data, N * sizeof(T));
	}

	str(const str &other) {
		memcpy(data, other.data, N * sizeof(T));
	}

	void cpy(T _data[]) {
		memcpy(data, _data, N * sizeof(T));
	}

	bool operator < (const str &other) const {
		for (int i = 0; i < N; ++i) {
			if (data[i] < other.data[i]) return true;
			if (data[i] > other.data[i]) return false;
		}
		return false;
	}

	bool operator > (const str &other) const {
		for (int i = 0; i < N; ++i) {
			if (data[i] > other.data[i]) return true;
			if (data[i] < other.data[i]) return false;
		}
		return false;
	}

	bool operator == (const str &other) const {
		for (int i = 0; i < N; ++i) {
			if (data[i] != other.data[i])
				return false;
		}
		return true;
	}

	template <T, int M>
	str<T, N + M> operator + (const str<T, M> other) {
		str<T, N + M> ans;
		for (int i = 0; i < N; ++i) {
			ans.data[i] = data[i];
		}
		for (int i = N; i < N + M; ++i) {
			ans.data[i] = other.data[i - N];
		}
		return ans;
	}
};

#endif