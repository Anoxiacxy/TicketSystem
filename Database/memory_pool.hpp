// This file is for memory pool.

#include <cstdio>
#include <cstring>
#include <cstdlib>

#ifndef __MEMORY_POOL__
#define __MEMORY_POOL__

using ull = unsigned long long;

const ull hash_seed = 233;

const size_t bitsize = (1 << 12);

struct FILE_KEY {
	char *filename;
	size_t pos, len;

	FILE_KEY() = default;
	FILE_KEY(const FILE_KEY &other) {
		len = other.len;
		pos = other.pos;
		filename = new char[len + 1];
		memcpy(filename, other.filename, len + 1);
	}

	FILE_KEY(const char *fname, int _pos) {
		len = strlen(fname);
		pos = _pos;
		filename = new char[len + 1];
		memcpy(filename, fname, len + 1);
	}

	~FILE_KEY() {
		delete [] filename;
	}

	bool operator == (const FILE_KEY &other) const {
		if (len != other.len || pos != other.pos) return 0;
		for (size_t i = 0; i < len; ++i)
			if (filename[i] != other.filename[i]) return 0;
		return 1;
	}

	ull hash_key() {
		ull result = pos;
		for (int i = 0; i < len; ++i)
			result = result * hash_seed + filename[i];
		return result;
	}
};

struct unit {
	unit *prev, *next, *qprev, *qnext;
	char mem[bitsize];
	FILE_KEY key;

	unit(FILE_KEY _key, unit *_next, unit *_qnext) :
		key(_key), next(_next), prev(nullptr), qnext(_qnext), qprev(_qprev)
	{
		FILE *f = fopen(_key.filename, "rb+");
		if (!f) f = fopen(_key.filename, "wb+");
		fseek(f, _key.pos * bitsize, SEEK_SET);
		fread(mem, 1, bitsize, f);
		fclose(f);
	}

	void release() {
		FILE *f = fopen(key.filename, "rb+");
		if (!f) f = fopen(key.filename, "wb+");
		fseek(f, _key.pos * bitsize, SEEK_SET);
		fwrite(mem, 1, bitsize, f);
		fflush(f);
		fclose(f);
	}
};

const size_t magic = 33331, K = 4096;

class buffer_pool {
private:
	unit *head, *tail, *mem[magic];
	int size;

	unit *find(FILE_KEY key) {
		int pos = key.hash_key() % magic;
		for (unit *iter = mem[pos]; iter; iter = iter->next)
			if (key == iter->key) return iter;
		return nullptr;
	}

public:
	buffer_pool() {
		size = 0;
		head = tail = nullptr;
		memset(mem, 0, sizeof(mem));
	}

	~buffer_pool() {
		if (cleaned) return;
		for (unit *iter = head; iter; ) {
			unit *nxt = iter->qnext;
			iter->release();
			delete iter;
			iter = nxt;
		}
	}

	void remove(unit *x) {
		if (x->qprev) x->qprev->qnext = x->qnext;
		else head = x->qnext;
		if (x->qnext) x->qnext->qprev = x->qprev;
		else tail = x->qprev;
		x->qprev = x->qnext = nullptr;
	}

	unit *insert(FILE_KEY key) {
		unit *result = find(key);
		if (result) {
			remove(result);
			if (head) {
				head->qprev = result;
				result->qnext = head;
			} else tail = result;
			head = result;
			return result;
		}
		ull hash;
		int pos;
		if (size == K) {
			unit *now = tail;
			now->release();
			remove(now);
			hash = now->key.hash_key();
			pos = hash % magic;
			if (now->next) now->next->prev = now->prev;
			if (now->prev) now->prev->next = now->next;
			else mem[pos] = now->next;
			delete now;
			--size;
		}
		hash = key.hash_key();
		pos = hash % magic;
		result = new unit(key, mem[pos], head);
		if (mem[pos]) mem[pos]->prev = result;
		mem[pos] = result;
		if (head == nullptr) tail = head;
		else head->qprev = result;
		head = result;
		++size;
		return result;
	}

	void clean() {
		size = 0;
		for (unit *iter = head; iter; ) {
			unit *nxt = iter->qnext;
			iter->release();
			delete iter;
			iter = nxt;
		}
		head = tail = nullptr;
	}
};

buffer_pool mem_pool;

void buffer_read(void *value, size_t pos, size_t size, const char *filename) {
	if (cleaned) return;
	size_t block_l = pos / bitsize, block_r = (pos + size - 1) / bitsize, index = 0;
	pos %= block_l;
	for (size_t i = block_l; i <= block_r; ++i) {
		int len = size > bitsize - pos ? bitsize - pos : size;
		unit *now = mem_pool.insert(FILE_KEY(filename, i));
		memcpy(value + index, now->mem + pos, len);
		pos = 0;
		index += len;
		size -= len;
	}
}

void buffer_write(const void *value, size_t pos, size_t size, const char *filename) {
	if (cleaned) return;
	size_t block_l = pos / bitsize, block_r = (pos + size - 1) / bitsize, index = 0;
	pos %= block_l;
	for (size_t i = block_l; i <= block_r; ++i) {
		int len = size > bitsize - pos ? bitsize - pos : size;
		unit *now = mem_pool.insert(FILE_KEY(filename, i));
		memcpy(now->mem + pos, value + index, len);
		pos = 0;
		index += len;
		size -= len;
	}
}

#endif