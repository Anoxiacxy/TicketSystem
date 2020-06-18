#include <iostream>
#include "memory_pool.hpp"
#include "exceptions.hpp"

const off_t invalid_off = 0xdeadbeef;

class find_blank {
private:
	const off_t K = 4096;
	int num;
	off_t size;
	off_t *blank;
	char *index_file;
public:
	find_blank() {}
	find_blank(const char *in_file) {
		index_file = new char[strlen(in_file) + 1];
		strcpy(index_file, in_file);
		FILE *file = fopen(index_file, "rb+");
		if (!file) {
			num = 0;
			size = 3 * sizeof(off_t);
			blank = nullptr;
			save_info();
		} else {
			fclose(file);
			save_info();
		}
	}

	~find_blank() {
		std::cerr << "testing" << std::endl;
		if (blank) delete blank;
		std::cerr << "testing" << std::endl;
		if (index_file) delete index_file;
		std::cerr << "testing" << std::endl;
	}

	void init(const char *in_file) {
		index_file = new char[strlen(in_file) + 1];
		strcpy(index_file, in_file);
		FILE *file = fopen(index_file, "rb+");
		// std::cerr << "fucking" << std::endl;
		if (!file) {
			num = 0;
			size = 3 * sizeof(off_t);
			blank = nullptr;

			// std::cerr << "fucking" << std::endl;
			save_info();
			// std::cerr << "fucking" << std::endl;
		}
		else {
			fclose(file);
			
			// std::cerr << "fucking" << std::endl;
			save_info();
			// std::cerr << "fucking" << std::endl;
		}
	}

	void save_info() {
		buffer_write(&num, 0, sizeof(int), index_file);
		buffer_write(&size, sizeof(int), sizeof(off_t), index_file);
		if (num) {
			for (int i = 0; i < num; ++i)
				buffer_write(&blank[i], sizeof(int) + sizeof(off_t) * i, sizeof(off_t), index_file);
		}
	}

	void load_info() {
		buffer_read(&num, 0, sizeof(int), index_file);
		buffer_read(&size, sizeof(int), sizeof(off_t), index_file);
		if (num) {
			blank = new off_t[num + 1];
			for (int i = 0; i < num; ++i)
				buffer_read(&blank[i], sizeof(int) + sizeof(off_t) * i, sizeof(off_t), index_file);
		}
	}

	off_t m_alloc() {
		load_info();
		if (num == 0) {
			off_t tmp = size;
			size += K;
			save_info();
			return tmp;
		} else {
			--num;
			save_info();
			return blank[num];
		}
	}

	void m_delete(off_t pos) {
		load_info();
		if (num == 0) {
			num = 1;
			blank = new off_t[1];
			blank[0] = pos;
		} else {
			blank[num++] = pos;
		}
		save_info();
		num = 0;
		size = invalid_off;
		delete blank;
		blank = nullptr;
	}
};