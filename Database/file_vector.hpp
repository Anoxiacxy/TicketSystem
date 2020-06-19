#ifndef __FILE_VECTOR_HPP__

#include "file_exceptions.hpp"
#include <climits>
#include <cstddef>
#include "find_blank.hpp"

namespace SJTU {
	template<typename T, const char *filename> class file_vector{
	private:
		FILE *f;
		size_t sz;
	public:
		file_vector() {
			buffer_read(&sz, 0, sizeof(size_t), filename);
		}
		
		~file_vector() {
			buffer_write(&sz, 0, sizeof(size_t), filename);
		}
		
		size_t size() const {
			return sz;
		}

		void init() {
			buffer_read(&sz, 0, sizeof(size_t), filename);
		}

		void clean() {
			FILE *f = fopen(filename, "rb+");
			if (!f) return;
			fclose(f);
			char ord[30];
			sprintf(ord, "rm %s", filename);
			system(ord);
		}

		T operator[](const size_t &index) const {
			if (index < 0 || index >= sz) throw index_out_of_bound();
			T result;
			buffer_read(&result, sizeof(size_t) + index * sizeof(T), sizeof(T), filename);
			return result;
		}

		void modify(const size_t &index, const T &value) const {
			if (index < 0 || index >= sz) throw index_out_of_bound();
			buffer_write(&value, sizeof(size_t) + index * sizeof(T), sizeof(T), filename);
		}

		void push_back(const T &value) {
			buffer_write(&value, sizeof(size_t) + sz * sizeof(T), sizeof(T), filename);
			++sz;
		}

		void pop_back() {
			--sz;
		}

		const T front() const {
			return operator[](0);
		}

		const T back() const {
			return operator[](sz - 1);
		}

		bool empty() const {
			return sz == 0;
		}

		void erase(const size_t &index) {
			for (int i = index; i < sz - 1; ++i) {
				T tmp = operator[](i + 1);
				modify(i, tmp);
			}
			--sz;
		}
	};
}

#endif
