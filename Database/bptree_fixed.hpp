#include <iostream>
#include <cstdio>
#include <cstddef>
#include <cstring>
#include "find_blank.hpp"
#include "exceptions.hpp"
#include "utility.hpp"
#include "memory_pool.hpp"

#ifndef __B_PLUS_TREE__
#define __B_PLUS_TREE__

namespace sjtu
{
	template <class Key, class value_t, size_t K = 4096, class Compare = std::less<Key>>
	class bptree
	{
		typedef char* buffer_pointer;
		typedef char buffer[K];

		const size_t node_size;
		const size_t leaf_size;

		find_blank finder;
		off_t head, tail, root;
		char *filename, *index_file;

		Compare comparing = Compare();

		bool equal_key(const Key &a, const Key &b) const
		{
			return !comparing(a, b) && !comparing(b, a);
		}

		struct node
		{
			Key key;
			off_t prev, next, pos;
			size_t size;

			bool is_leaf;

			node(off_t __pos, off_t __prev = invalid_off, off_t __next = invalid_off, bool __is_leaf = true) :
				pos(__pos), prev(__prev), next(__next), is_leaf(__is_leaf), size(0)
			{
				key = Key();
			} // for leaf node

			node(off_t __pos, bool __is_leaf = false) : pos(__pos), is_leaf(__is_leaf)
			{
				key = Key();
			} // for inner node

			node(const node &other)
			{
				key = other.key;
				prev = other.prev;
				next = other.next;
				size = other.size;
				pos = other.pos;
				is_leaf = other.is_leaf;
			} // for copy a node
		};
	private:
		void save_node(const node &x)
		{
			buffer_write(&x, x.pos, sizeof(node), filename);
		}

		void delete_node(const node &x)
		{
			finder.m_delete(x.pos);
		}

		void save_info()
		{
			buffer_write(&head, 0, sizeof(off_t), filename);
			buffer_write(&tail, sizeof(off_t), sizeof(off_t), filename);
			buffer_write(&root, 2 * sizeof(off_t), sizeof(off_t), filename);
		}

		void init()
		{
			head = tail = root = invalid_off;
			save_info();
			finder.save_info();
		}

		//--------------transfer information between buffer and node------------------------

		void buffer_load_node(buffer_pointer b, const node &x) const
		{
			if (x.size == 0) return;
			buffer_read(b, x.pos + sizeof(node), x.size * (sizeof(off_t) + sizeof(Key)), filename);
		}

		void buffer_load_leaf(buffer_pointer b, const node &x) const {
			if (x.size == 0) return;
			buffer_read(b, x.pos + sizeof(node), x.size * (sizeof(value_t) + sizeof(Key)), filename);
		}

		void buffer_save_node(buffer_pointer b, const node &x) const {
			buffer_write(b, x.pos + sizeof(node), x.size * (sizeof(off_t) + sizeof(Key)), filename);
		}

		void buffer_save_leaf(buffer_pointer b, const node &x) const {
			buffer_write(b, x.pos + sizeof(node), x.size * (sizeof(value_t) + sizeof(Key)), filename);
		}

		//------------------get info from node---------------------
		
		Key *get_node_key(buffer_pointer b, size_t num) const {
			return (Key *)(b + num * (sizeof(Key) + sizeof(off_t)));
		}

		off_t *get_node_son(buffer_pointer b, size_t num) const {
			return (off_t *)(b + num * (sizeof(Key) + sizeof(off_t)) + sizeof(Key));
		}

		Key *get_leaf_key(buffer_pointer b, size_t num) const {
			return (Key *)(b + num * (sizeof(Key) + sizeof(value_t)));
		}

		value_t *get_leaf_value(buffer_pointer b, size_t num) const {
			return (value_t *)(b + num * (sizeof(Key) + sizeof(value_t)) + sizeof(Key));
		}

		node get_node(off_t t) const {
			node x(invalid_off, false);
			buffer_read(&x, t, sizeof(node), filename);
			return x;
		}

		size_t find_leaf_pos(buffer_pointer b, Key key, size_t n) const {
			size_t l = 0, r = n, mid;
			while (l < r) {
				mid = (l + r) / 2;
				if (comparing(*get_leaf_key(b, mid), key)) l = mid + 1;
				else r = mid;
			}
			return l;
		}

		size_t find_node_pos(buffer_pointer b, Key key, size_t n) const {
			size_t l = 0, r = n, mid;
			while (l < r) {
				mid = (l + r) / 2;
				if (comparing(*get_node_key(b, mid), key)) l = mid + 1;
				else r = mid;
			}
			return l;
		}

		//---------------------operation for insert--------------
		
		void adjust_node(buffer_pointer b, node &x, size_t t) {
			for (size_t i = x.size; i > t; --i) {
				*get_node_key(b, i) = *get_node_key(b, i - 1);
				*get_node_son(b, i) = *get_node_son(b, i - 1);
			}
		}

		void adjust_leaf(buffer_pointer b, node &x, size_t t) {
			for (size_t i = x.size; i > t; --i) {
				*get_leaf_key(b, i) = *get_leaf_key(b, i - 1);
				*get_leaf_value(b, i) = *get_leaf_value(b, i - 1);
			}
		}

		node split_node(buffer_pointer b, node &x) {
			size_t sz = x.size / 2, new_sz = x.size - sz;
			node new_node(finder.m_alloc(), false);
			new_node.key = *get_node_key(b, sz);
			new_node.size = new_sz;
			x.size = sz;
			save_node(x);
			buffer_save_node(b, x);
			save_node(new_node);
			buffer_save_node(b + sz * (sizeof(Key) + sizeof(off_t)), new_node);
			return new_node;
		}

		node split_leaf(buffer_pointer b, node &x) {
			size_t sz = x.size / 2, new_sz = x.size - sz;
			off_t nxt = x.next;
			node new_leaf(finder.m_alloc(), x.pos, x.next, true);
			x.next = new_leaf.pos;
			x.size = sz;
			new_leaf.size = new_sz;
			new_leaf.key = *get_leaf_key(b, sz);
			if (new_leaf.next == invalid_off) {
				tail = new_leaf.pos;
				save_info();
			} else {
				node new_leaf_next = get_node(nxt);
				new_leaf_next.prev = new_leaf.pos;
				save_node(new_leaf_next);
			}
			save_node(x);
			buffer_save_leaf(b, x);
			save_node(new_leaf);
			buffer_save_leaf(b + sz * (sizeof(Key) + sizeof(value_t)), new_leaf);
			return new_leaf;
		}

		node buffer_insert_node(buffer_pointer b, node &x, Key key, off_t son, off_t t) {
			adjust_node(b, x, t);
			*get_node_key(b, t) = key;
			*get_node_son(b, t) = son;
			x.key = *get_node_key(b, 0);
			++x.size;
			save_node(x);
			if (x.size > node_size) {
				node y = split_node(b, x);
				return y;
			} else {
				buffer_save_node(b, x);
				return x;
			}
		}

		node buffer_insert_leaf(buffer_pointer b, node &x, Key key, value_t value, bool &is_change) {
			size_t t = find_leaf_pos(b, key, x.size);
			if (t < x.size && equal_key(key, *get_leaf_key(b, t))) {
				*get_leaf_value(b, t) = value;
				is_change = true;
				buffer_save_leaf(b, x);
				return x;
			}
			adjust_leaf(b, x, t);
			*get_leaf_key(b, t) = key;
			*get_leaf_value(b, t) = value;
			x.key = *get_leaf_key(b, 0);
			++x.size;
			save_node(x);
			is_change = false;
			if (x.size > leaf_size) {
				node y = split_leaf(b, x);
				return y;
			} else {
				buffer_save_leaf(b, x);
				return x;
			}
		}

		node insert(node &x, Key key, value_t value, bool &is_change) {
			if (x.is_leaf) {
				buffer b;
				buffer_load_leaf(b, x);
				node new_node = buffer_insert_leaf(b, x, key, value, is_change);
				return new_node;
			} else {
				buffer b;
				buffer_load_node(b, x);
				size_t pos = find_node_pos(b, key, x.size);
				if (!equal_key(key, *get_node_key(b, pos)) && pos > 0) --pos;
				if (pos == x.size) --pos;
				off_t pre_son = *get_node_son(b, pos);
				node pre_son_node = get_node(pre_son);
				node new_node = insert(pre_son_node, key, value, is_change);
				if (is_change) return x;
				if (new_node.pos != pre_son_node.pos) {
					node alter_node = buffer_insert_node(b, x, new_node.key, new_node.pos, pos + 1);
					return alter_node;
				} else {
					*get_node_key(b, pos) = pre_son_node.key;
					x.key = *get_node_key(b, 0);
					save_node(x);
					buffer_save_node(b, x);
					return x;
				}
			}
		}

		//--------------------------operation for erase--------------------------
		
		void merge_left_node(buffer_pointer bx, node &x, node &y) {
			buffer by;
			buffer_load_node(by, y);
			for (size_t i = 0; i < x.size; ++i) {
				*get_node_son(by, i + y.size) = *get_node_son(bx, i);
				*get_node_key(by, i + y.size) = *get_node_key(bx, i);
			}
			y.size += x.size;
			delete_node(x);
			save_node(y);
			buffer_save_node(by, y);
		}

		void merge_right_node(buffer_pointer bx, node &x, node &y) {
			buffer by;
			buffer_load_node(by, y);
			for (size_t i = 0; i < y.size; ++i) {
				*get_node_son(bx, i + x.size) = *get_node_son(by, i);
				*get_node_key(bx, i + x.size) = *get_node_key(by, i);
			}
			x.size += y.size;
			delete_node(y);
			save_node(x);
			buffer_save_node(bx, x);
		}

		void merge_left_leaf(buffer_pointer bx, node &x, node &y) {
			buffer by;
			buffer_load_leaf(by, y);
			for (size_t i = 0; i < x.size; ++i) {
				*get_leaf_value(by, i + y.size) = *get_leaf_value(bx, i);
				*get_leaf_key(by, i + y.size) = *get_leaf_key(bx, i);
			}
			y.size += x.size;
			y.next = x.next;
			if (x.next != invalid_off) {
				node nextx = get_node(x.next);
				nextx.prev = y.pos;
				save_node(nextx);
			}
			delete_node(x);
			save_node(y);
			buffer_save_node(by, y);
		}

		void merge_right_leaf(buffer_pointer bx, node &x, node &y) {
			buffer by;
			buffer_load_leaf(by, y);
			for (size_t i = 0; i < y.size; ++i) {
				*get_leaf_value(bx, i + x.size) = *get_leaf_value(by, i);
				*get_leaf_key(bx, i + x.size) = *get_leaf_key(by, i);
			}
			x.size += y.size;
			x.next = y.next;
			if (y.next != invalid_off) {
				node nexty = get_node(y.next);
				nexty.prev = x.pos;
				save_node(nexty);
			}
			delete_node(y);
			save_node(x);
			buffer_save_node(bx, x);
		}

		void buffer_erase_node(buffer_pointer b, node &x, Key key) {
			size_t t = find_node_pos(b, key, x.size);
			if (!(t < x.size && equal_key(key, *get_node_key(b, t)))) throw "erase node error!";
			for (size_t i = t; i < x.size - 1; ++i) {
				*get_node_key(b, i) = *get_node_key(b, i + 1);
				*get_node_son(b, i) = *get_node_son(b, i + 1);
			}
			x.key = *get_node_key(b, 0);
			--x.size;
		}

		void buffer_erase_leaf(buffer_pointer b, node &x, Key key) {
			size_t t = find_leaf_pos(b, key, x.size);
			if (!(t < x.size && equal_key(key, *get_node_key(b, t)))) throw "erase leaf error!";
			for (size_t i = t; i < x.size - 1; ++i) {
				*get_leaf_value(b, i) = *get_leaf_value(b, i + 1);
				*get_leaf_key(b, i) = *get_leaf_key(b, i + 1);
			}
			x.key = *get_leaf_key(b, 0);
			--x.size;
		}

		// -1 : not merged and balanced with next
		// 0  : not merged and balanced with prev
		// 1  : merged with prev
		// 2  : merged with next
		// 3  : deleted

		pair<int, Key> leaf_balance(buffer_pointer b, node &x, off_t prev, off_t next) {
			if (prev == invalid_off && next == invalid_off) {
				if (!x.size) {
					delete_node(x);
					head = tail = root = invalid_off;
					save_info();
					save_node(x);
					buffer_save_leaf(b, x);
					return pair<int, Key> (3, Key());
				}
				return pair<int, Key> (0, Key());
			}
			if (prev == invalid_off) {
				node n = get_node(next);
				if (n.size >= leaf_size / 2 + 1) {
					buffer bn;
					buffer_load_leaf(bn, n);
					*get_leaf_value(b, x.size) = *get_leaf_value(bn, 0);
					*get_leaf_key(b, x.size) = *get_leaf_key(bn, 0);
					for (size_t i = 0; i < n.size - 1; ++i) {
						*get_leaf_value(bn, i) = *get_leaf_value(bn, i + 1);
						*get_leaf_key(bn, i) = *get_leaf_key(bn, i + 1);
					}
					n.key = *get_leaf_key(bn, 0);
					++x.size;
					--n.size;
					save_node(x);
					buffer_save_leaf(b, x);
					save_node(n);
					buffer_save_leaf(bn, n);
					return pair<int, Key> (-1, n.key);	
				} else {
					merge_right_leaf(b, x, n);
					return pair<int, Key> (2, Key());
				}
			}
			if (next == invalid_off) {
				node p = get_node(prev);
				if (p.size >= leaf_size / 2 + 1) {
					buffer bp;
					buffer_load_leaf(bp, p);
					for (size_t i = x.size; i > 0; --i) {
						*get_leaf_value(b, i) = *get_leaf_value(b, i - 1);
						*get_leaf_key(b, i) = *get_leaf_key(b, i - 1);
					}
					*get_leaf_value(b, 0) = *get_leaf_value(bp, p.size - 1);
					*get_leaf_key(b, 0) = *get_leaf_key(bp, p.size - 1);
					x.key = *get_leaf_key(b, 0);
					++x.size;
					--p.size;
					save_node(x);
					buffer_save_leaf(b, x);
					save_node(p);
					buffer_save_leaf(bp, p);
					return pair<int, Key> (0, x.key);
				} else {
					merge_left_leaf(b, x, p);
					return pair<int, Key> (1, Key());
				}
			}
			node p = get_node(prev), n = get_node(next);
			if (p.size >= leaf_size / 2 + 1) {
				buffer bp;
				buffer_load_leaf(bp, p);
				for (size_t i = x.size; i > 0; --i) {
					*get_leaf_value(b, i) = *get_leaf_value(b, i - 1);
					*get_leaf_key(b, i) = *get_leaf_key(b, i - 1);
				}
				*get_leaf_value(b, 0) = *get_leaf_value(bp, p.size - 1);
				*get_leaf_key(b, 0) = *get_leaf_key(bp, p.size - 1);
				x.key = *get_leaf_key(b, 0);
				++x.size;
				--p.size;
				save_node(x);
				buffer_save_leaf(b, x);
				save_node(p);
				buffer_save_leaf(bp, p);
				return pair<int, Key> (0, x.key);
			} else {
				if (n.size >= leaf_size / 2 + 1) {
					buffer bn;
					buffer_load_leaf(bn, n);
					*get_leaf_value(b, x.size) = *get_leaf_value(bn, 0);
					*get_leaf_key(b, x.size) = *get_leaf_key(bn, 0);
					for (size_t i = 0; i < n.size - 1; ++i) {
						*get_leaf_value(bn, i) = *get_leaf_value(bn, i + 1);
						*get_leaf_key(bn, i) = *get_leaf_key(bn, i + 1);
					}
					n.key = *get_leaf_key(bn, 0);
					++x.size;
					--n.size;
					save_node(x);
					buffer_save_leaf(b, x);
					save_node(n);
					buffer_save_leaf(bn, n);
					return pair<int, Key> (-1, n.key);	
				} else {
					merge_right_leaf(b, x, n);
					return pair<int, Key> (2, Key());
				}
			}
		}

		pair<int, Key> node_balance(buffer_pointer b, node &x, off_t prev, off_t next) {
			if (prev == invalid_off && next == invalid_off) {
				if (!x.size) {
					delete_node(x);
					head = tail = root = invalid_off;
					save_info();
					save_node(x);
					buffer_save_node(b, x);
					return pair<int, Key> (3, Key());
				}
				return pair<int, Key> (0, Key());
			}
			if (prev == invalid_off) {
				node n = get_node(next);
				if (n.size >= node_size / 2 + 1) {
					buffer bn;
					buffer_load_node(bn, n);
					*get_node_son(b, x.size) = *get_node_son(bn, 0);
					*get_node_key(b, x.size) = *get_node_key(bn, 0);
					for (size_t i = 0; i < n.size - 1; ++i) {
						*get_node_son(bn, i) = *get_node_son(bn, i + 1);
						*get_node_key(bn, i) = *get_node_key(bn, i + 1);
					}
					n.key = *get_node_key(bn, 0);
					++x.size;
					--n.size;
					save_node(x);
					buffer_save_node(b, x);
					save_node(n);
					buffer_save_node(bn, n);
					return pair<int, Key> (-1, n.key);	
				} else {
					merge_right_node(b, x, n);
					return pair<int, Key> (2, Key());
				}
			}
			if (next == invalid_off) {
				node p = get_node(prev);
				if (p.size >= node_size / 2 + 1) {
					buffer bp;
					buffer_load_node(bp, p);
					for (size_t i = x.size; i > 0; --i) {
						*get_node_son(b, i) = *get_node_son(b, i - 1);
						*get_node_key(b, i) = *get_node_key(b, i - 1);
					}
					*get_node_son(b, 0) = *get_node_son(bp, p.size - 1);
					*get_node_key(b, 0) = *get_node_key(bp, p.size - 1);
					x.key = *get_node_key(b, 0);
					++x.size;
					--p.size;
					save_node(x);
					buffer_save_node(b, x);
					save_node(p);
					buffer_save_node(bp, p);
					return pair<int, Key> (0, x.key);
				} else {
					merge_left_node(b, x, p);
					return pair<int, Key> (1, Key());
				}
			}
			node p = get_node(prev), n = get_node(next);
			if (p.size >= node_size / 2 + 1) {
				buffer bp;
				buffer_load_node(bp, p);
				for (size_t i = x.size; i > 0; --i) {
					*get_node_son(b, i) = *get_node_son(b, i - 1);
					*get_node_key(b, i) = *get_node_key(b, i - 1);
				}
				*get_node_son(b, 0) = *get_node_son(bp, p.size - 1);
				*get_node_key(b, 0) = *get_node_key(bp, p.size - 1);
				x.key = *get_node_key(b, 0);
				++x.size;
				--p.size;
				save_node(x);
				buffer_save_node(b, x);
				save_node(p);
				buffer_save_node(bp, p);
				return pair<int, Key> (0, x.key);
			} else {
				if (n.size >= node_size / 2 + 1) {
					buffer bn;
					buffer_load_node(bn, n);
					*get_node_son(b, x.size) = *get_node_son(bn, 0);
					*get_node_key(b, x.size) = *get_node_key(bn, 0);
					for (size_t i = 0; i < n.size - 1; ++i) {
						*get_node_son(bn, i) = *get_node_son(bn, i + 1);
						*get_node_key(bn, i) = *get_node_key(bn, i + 1);
					}
					n.key = *get_node_key(bn, 0);
					++x.size;
					--n.size;
					save_node(x);
					buffer_save_node(b, x);
					save_node(n);
					buffer_save_node(bn, n);
					return pair<int, Key> (-1, n.key);	
				} else {
					merge_right_node(b, x, n);
					return pair<int, Key> (2, Key());
				}
			}
		}

		pair<int, Key> erase(node &x, const Key &key, node &parent) {
			buffer b;
			if (x.is_leaf) {
				buffer_load_leaf(b, x);
				buffer_erase_leaf(b, x, key);
				if (x.size == 0 && x.pos == root) {
					root = head = tail = invalid_off;
					save_info();
					return pair<int, Key> (3, x.key);
				}
				if (x.size < leaf_size / 2 && x.pos != root) {
					buffer bp;
					buffer_load_node(bp, parent);
					size_t t = find_node_pos(bp, x.key, parent.size);
					if (!equal_key(x.key, *get_node_key(bp, t)) && t > 0) --t;
					if (t == parent.size) --t;
					off_t prev = (t > 0) ? (*get_node_son(bp, t - 1)) : invalid_off;
					off_t next = (t < parent.size - 1) ? (*get_node_son(bp, t + 1)) : invalid_off;
					return leaf_balance(b, x, prev, next);
				}
				save_node(x);
				buffer_save_leaf(b, x);
				return pair<int, Key> (0, x.key);
			} else {
				buffer_load_node(b, x);
				size_t tx = find_node_pos(b, key, x.size);
				if (!equal_key(key, *get_node_key(b, tx)) && tx > 0) --tx;
				node x_son = get_node(*get_node_son(b, tx));
				pair<int, Key> tmp = erase(x_son, key, x);
				switch (tmp.first) {
					case -1: {
						*get_node_key(b, tx) = x_son.key;
						*get_node_key(b, tx + 1) = tmp.second;
						if (!tx) x.key = x_son.key;
						save_node(x);
						buffer_save_node(b, x);
						break;
					}
					case 0: {
						*get_node_key(b, tx) = tmp.second;
						save_node(x);
						buffer_save_node(b, x);
						break;
					}
					case 1: {
						for (size_t i = tx; i < x.size - 1; ++i) {
							*get_node_son(b, i) = *get_node_son(b, i + 1);
							*get_node_key(b, i) = *get_node_key(b, i + 1);
						}
						--x.size;
						save_node(x);
						buffer_save_node(b, x);
						break;
					}
					case 2: {
						for (size_t i = tx + 1; i < x.size - 1; ++i) {
							*get_node_son(b, i) = *get_node_son(b, i + 1);
							*get_node_key(b, i) = *get_node_key(b, i + 1);
						}
						if (!tx) x.key = x_son.key;
						--x.size;
						save_node(x);
						buffer_save_node(b, x);
						break;
					}
					default: {
						delete_node(x);
						head = tail = root = invalid_off;
						save_info();
						return pair<int, Key> (3, Key());
					}
				}
				if (x.pos == root && x.size == 1) {
					root = *get_node_son(b, 0);
					save_info();
					return pair<int, Key> (0, Key());
				}
				if (x.size < node_size / 2) {
					if (root == x.pos) return node_balance(b, x, invalid_off, invalid_off);
					buffer bp;
					buffer_load_node(bp, parent);
					size_t t = find_node_pos(bp, x.key, parent.size);
					if (!equal_key(x.key, *get_node_key(bp, t)) && t > 0) --t;
					off_t prev = (t > 0) ? (*get_node_son(bp, t - 1)) : invalid_off;
					off_t next = (t < parent.size - 1) ? (*get_node_son(bp, t + 1)) : invalid_off;
					return node_balance(b, x, prev, next);
				}
				x.key = *get_node_key(b, 0);
				save_node(x);
				buffer_save_node(b, x);
				return pair<int, Key> (0, x.key);
			}
		}

	public:
		bptree(const char *fname, const char *in_file) : 
			leaf_size((K - sizeof(node)) / (sizeof(Key) + sizeof(value_t)) - 1),
			node_size((K - sizeof(node)) / (sizeof(Key) + sizeof(value_t)) - 1)
			// leaf_size(50), node_size(50)
		{
			// std::cerr << "fucking" << std::endl;
			filename = new char[strlen(fname) + 1];
			strcpy(filename, fname);
			index_file = new char[strlen(in_file) + 1];
			strcpy(index_file, in_file);

			// std::cerr << "fucking" << std::endl;
			finder.init(in_file);
			
			// std::cerr << "fucking" << std::endl;
			FILE *file = fopen(filename, "rb+");
			// std::cerr << "fucking" << std::endl;
			if (!file) init();
			else {
				fclose(file);
				buffer_read(&head, 0, sizeof(off_t), filename);
				buffer_read(&tail, sizeof(off_t), sizeof(off_t), filename);
				buffer_read(&root, 2 * sizeof(off_t), sizeof(off_t), filename);
			}
		}

		~bptree() {
			finder.save_info();
			save_info();
			if (filename) delete filename;
			if (index_file) delete index_file;
		}

		void clear() {
			char deleted[40] = "rm ";
			strncpy(deleted + 3, filename, strlen(filename));
			system(deleted);
			char deletef[40] = "rm ";
			strncpy(deletef + 3, index_file, strlen(index_file));
			system(deletef);
		}

		class const_iterator;
		class iterator {
			friend class const_iterator;
			typedef value_t & reference;
			typedef value_t * pointer;
		private:
			node leaf;
			size_t pos;
			const bptree *tree;
		public:
			iterator() : leaf(invalid_off, invalid_off, invalid_off, true), pos(0), tree(nullptr) {}
			iterator(node _leaf, size_t _pos, const bptree *_tree) : leaf(_leaf), pos(_pos) {
				tree = _tree;
			}

			iterator(const iterator &other) : leaf(other.leaf), pos(other.pos) {
				tree = other.tree;
			}

			operator const_iterator() {
				return const_iterator(*this);
			}

			const iterator operator++(int) {
				iterator bakup(*this);
				++(*this);
				return bakup;
			}

			iterator & operator++() {
				if (pos == leaf.size - 1) {
					if (leaf.pos != tree->tail) {
						leaf = tree->get_node(leaf.next);
						pos = 0;
					} else {
						node tmp(invalid_off, invalid_off, invalid_off, true);
						leaf = tmp;
						pos = 0;
					}
				} else ++pos;
				return *this;
			}

			const iterator operator--(int) {
				iterator bakup(*this);
				--(*this);
				return bakup;
			}

			iterator & operator--() {
				if (pos == 0) {
					if (leaf.pos != tree->head) {
						leaf = tree->get_node(leaf.prev);
						pos = leaf.size - 1;
					} else {
						node tmp(invalid_off, invalid_off, invalid_off, true);
						leaf = tmp;
						pos = 0;
					}
				} else --pos;
				return *this;
			}

			reference operator *() {
				buffer b;
				tree->buffer_load_leaf(b, leaf);
				return *tree->get_leaf_value(b, pos);
			}

			pointer operator -> () {
				buffer b;
				tree->buffer_load_leaf(b, leaf);
				return tree->get_leaf_value(b, pos);
			}

			Key & get_key() {
				buffer b;
				tree->buffer_load_leaf(b, leaf);
				return *tree->get_leaf_key(b, pos);
			}

			bool operator == (const iterator &other) const {
				return leaf.pos == other.leaf.pos && pos == other.pos;
			}

			bool operator != (const iterator &other) const {
				return !(*this == other);
			}

			bool operator == (const const_iterator &other) const {
				return leaf.pos == other.leaf.pos && pos == other.pos;
			}

			bool operator != (const const_iterator &other) const {
				return !(*this == other);
			}
		};

		class const_iterator {
			friend class iterator;
			typedef value_t& reference;
			typedef value_t* pointer;
		private:
			node leaf;
			size_t pos;
			const bptree *tree;
		public:
			const_iterator() : leaf(invalid_off, invalid_off, invalid_off, true), pos(0), tree(nullptr) {}
			const_iterator(node _leaf, size_t _pos, const bptree *_tree) : leaf(_leaf), pos(_pos) {
				tree = _tree;
			}

			const_iterator(const iterator &other) : leaf(other.leaf), pos(other.pos) {
				tree = other.tree;
			}

			const_iterator(const const_iterator &other) : leaf(other.leaf), pos(other.pos) {
				tree = other.tree;
			}

			operator iterator() {
				return iterator(*this);
			}

			const const_iterator operator++(int) {
				const_iterator bakup(*this);
				++(*this);
				return bakup;
			}

			const_iterator & operator++() {
				if (pos == leaf.size - 1) {
					if (leaf.pos != tree->tail) {
						leaf = tree->get_node(leaf.next);
						pos = 0;
					} else {
						node tmp(invalid_off, invalid_off, invalid_off, true);
						leaf = tmp;
						pos = 0;
					}
				} else ++pos;
				return *this;
			}

			const const_iterator & operator--(int) {
				const_iterator bakup(*this);
				--(*this);
				return bakup;
			}

			const_iterator & operator--() {
				if (pos == 0) {
					if (leaf.pos != tree->head) {
						leaf = tree->get_node(leaf.prev);
						pos = leaf.size - 1;
					} else {
						node tmp(invalid_off, invalid_off, invalid_off, true);
						leaf = tmp;
						pos = 0;
					}
				} else --pos;
				return *this;
			}

			reference operator *() {
				buffer b;
				tree->buffer_load_leaf(b, leaf);
				return *tree->get_leaf_value(b, pos);
			}

			pointer operator -> () {
				buffer b;
				tree->buffer_load_leaf(b, leaf);
				return tree->get_leaf_value(b, pos);
			}

			Key & get_key() {
				buffer b;
				tree->buffer_load_leaf(b, leaf);
				return *tree->get_leaf_key(b, pos);
			}

			bool operator == (const iterator &other) const {
				return leaf.pos == other.leaf.pos && pos == other.pos;
			}

			bool operator != (const iterator &other) const {
				return !(*this == other);
			}

			bool operator == (const const_iterator &other) const {
				return leaf.pos == other.leaf.pos && pos == other.pos;
			}

			bool operator != (const const_iterator &other) const {
				return !(*this == other);
			}
		};

		iterator begin() {
			return iterator(get_node(head), 0, this);
		}

		iterator end() {
			return iterator(node(invalid_off, invalid_off, invalid_off, true), 0, this);
		}

		const_iterator cbegin() {
			return const_iterator(get_node(head), 0, this);
		}

		const_iterator cend() {
			return const_iterator(node(invalid_off, invalid_off, invalid_off, true), 0, this);
		}

		inline bool empty() const {
			return root == invalid_off;
		}

		int count(const Key &key, node &x) const {
			if (comparing(key, x.key)) return 0;
			if (x.is_leaf) {
				buffer b;
				buffer_load_leaf(b, x);
				size_t t = find_leaf_pos(b, key, x.size);
				if (t < x.size && equal_key(key, *get_leaf_key(b, t))) return 1;
				else return 0;
			}
			buffer b;
			buffer_load_node(b, x);
			size_t t = find_node_pos(b, key, x.size);
			if (t > 0 && !equal_key(key, *get_node_key(b, t))) --t;
			if (t == x.size) --t;
			node y = get_node(*get_node_son(b, t));
			return count(key, y);
		}

		int count(const Key &key) const {
			if (empty()) return 0;
			node x = get_node(root);
			return count(key, x);
		}

		void insert(const Key &key, const value_t &value) {
			bool is_change;
			if (empty()) {
				node x(finder.m_alloc(), invalid_off, invalid_off, true);
				buffer b;
				buffer_insert_leaf(b, x, key, value, is_change);
				head = tail = root = x.pos;
				save_node(x);
				buffer_save_leaf(b, x);
				save_info();
				return;
			}
			node x = get_node(root);
			node result = insert(x, key, value, is_change);
			if (result.pos != x.pos) {
				buffer b;
				node new_root(finder.m_alloc(), invalid_off, invalid_off, false);
				// buffer_load_node(b, new_root);
				root = new_root.pos;
				*get_node_key(b, 0) = x.key;
				*get_node_son(b, 0) = x.pos;
				*get_node_key(b, 1) = result.key;
				*get_node_son(b, 1) = result.pos;
				new_root.size = 2;
				save_node(new_root);
				buffer_save_node(b, new_root);
				save_info();
				return;
			} else {
				save_node(x);
			}
		}

		void erase(const Key &key) {
			node rt = get_node(root);
			erase(rt, key, rt);
		}

		value_t at(const Key &key, node &x) {
			if (comparing(key, x.key)) {
				value_t value = value_t();
				return value;
			}
			if (x.is_leaf) {
				buffer b;
				buffer_load_leaf(b, x);
				size_t t = find_leaf_pos(b, key, x.size);
				if (t < x.size && equal_key(key, *get_leaf_key(b, t))) return *get_leaf_value(b, t);
				else {
					value_t value = value_t();
					return value;
				}
			}
			buffer b;
			buffer_load_node(b, x);
			size_t t = find_node_pos(b, key, x.size);
			if (t > 0 && !equal_key(key, *get_node_key(b, t))) --t;
			if (t == x.size) --t;
			node y = get_node(*get_node_son(b, t));
			return at(key, y);
		}

		value_t at(const Key &key) {
			if (empty()) return value_t();
			node rt = get_node(root);
			return at(key, rt);
		}

		const value_t at(const Key &key, node &x) const {
			if (comparing(key, x.key)) {
				value_t value = value_t();
				return value;
			}
			if (x.is_leaf) {
				buffer b;
				buffer_load_leaf(b, x);
				size_t t = find_leaf_pos(b, key, x.size);
				if (t < x.size && equal_key(key, *get_leaf_key(b, t))) return *get_leaf_value(b, t);
				else {
					value_t value = value_t();
					return value;
				}
			}
			buffer b;
			buffer_load_node(b, x);
			size_t t = find_node_pos(b, key, x.size);
			if (t > 0 && !equal_key(key, *get_node_key(b, t))) --t;
			if (t == x.size) --t;
			node y = get_node(*get_node_son(b, t));
			return at(key, y);
		}

		const value_t at(const Key &key) const {
			if (empty()) return value_t();
			node rt = get_node(root);
			return at(key, rt);
		}

		const_iterator clower_bound(const Key &key, node &x) const {
			if (x.is_leaf) {
				buffer b;
				buffer_load_leaf(b, x);
				size_t t = find_leaf_pos(b, key, x.size);
				if (t < x.size && equal_key(key, *get_leaf_key(b, t))) return const_iterator(x, t, this);
				else {
					if (t == x.size) {
						if (x.pos != tail) return ++const_iterator(x, t - 1, this);
						else return const_iterator(node(invalid_off, invalid_off, invalid_off, true), 0, this);
					}
					else return const_iterator(x, t, this);
				}
			}
			buffer b;
			buffer_load_node(b, x);
			size_t t = find_node_pos(b, key, x.size);
			if (t > 0 && !equal_key(key, *get_node_key(b, t))) --t;
			if (t == x.size) --t;
			node y = get_node(*get_node_son(b, t));
			return clower_bound(key, y);
		}

		const_iterator clower_bound(const Key &key) const {
			if (empty())
				return const_iterator(node(invalid_off, invalid_off, invalid_off, true), 0, this);
			node rt = get_node(root);
			return clower_bound(key, rt);
		}

		iterator lower_bound(const Key &key, node &x) const {
			if (x.is_leaf) {
				buffer b;
				buffer_load_leaf(b, x);
				size_t t = find_leaf_pos(b, key, x.size);
				if (t < x.size && equal_key(key, *get_leaf_key(b, t))) return iterator(x, t, this);
				else {
					if (t == x.size) {
						if (x.pos != tail) return ++iterator(x, t - 1, this);
						else return iterator(node(invalid_off, invalid_off, invalid_off, true), 0, this);
					}
					else return iterator(x, t, this);
				}
			}
			buffer b;
			buffer_load_node(b, x);
			size_t t = find_node_pos(b, key, x.size);
			if (t > 0 && !equal_key(key, *get_node_key(b, t))) --t;
			if (t == x.size) --t;
			node y = get_node(*get_node_son(b, t));
			return lower_bound(key, y);
		}

		iterator lower_bound(const Key &key) const {
			if (empty())
				return iterator(node(invalid_off, invalid_off, invalid_off, true), 0, this);
			node rt = get_node(root);
			return lower_bound(key, rt);
		}

		bool modify(const Key &key, const value_t &value) {
			bool is_change;
			if (empty()) {
				node x(finder.m_alloc(), invalid_off, invalid_off, true);
				buffer b;
				buffer_insert_leaf(b, x, key, value, is_change);
				head = tail = root = x.pos;
				save_node(x);
				buffer_save_leaf(b, x);
				save_info();
				return is_change;
			}
			node x = get_node(root);
			node result = insert(x, key, value, is_change);
			if (result.pos != x.pos) {
				buffer b;
				node new_root(finder.m_alloc(), invalid_off, invalid_off, false);
				// buffer_load_node(b, new_root);
				root = new_root.pos;
				*get_node_key(b, 0) = x.key;
				*get_node_son(b, 0) = x.pos;
				*get_node_key(b, 1) = result.key;
				*get_node_son(b, 1) = result.pos;
				new_root.size = 2;
				save_node(new_root);
				buffer_save_node(b, new_root);
				save_info();
				return is_change;
			} else {
				save_node(x);
				return is_change;
			}
		}

		void traverse(node &x) {
			buffer b;
			if (x.is_leaf) {
				buffer_load_leaf(b, x);
				std::cout << "Leaf " << x.pos << "---" << x.size << "datas\n";
				for (size_t i = 0; i < x.size; ++i) {
					std::cout << "(" << *get_leaf_key(b, i) << "," << *get_leaf_value(b, i) << ")\n";
				}
			} else {
				buffer_load_node(b, x);
				std::cout << "Node " << x.pos << "---" << x.size << "datas\n";
				for (size_t i = 0; i < x.size; ++i) {
					std::cout << "(" << *get_node_key(b, i) << "," << *get_node_son(b, i) << ")\n";
				}
				std::cout << "\n";

				for (size_t i = 0; i < x.size; ++i) {
					node son = get_node(*get_node_son(b, i));
					traverse(son);
				}
				std::cout << "\n";
			}
		}

		void traverse() {
			node rt = get_node(root);
			traverse(rt);
		}
	};
}

#endif