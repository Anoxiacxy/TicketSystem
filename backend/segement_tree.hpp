#ifndef SJTU_SEGEMENT_TREE_HPP
#define SJTU_SEGEMENT_TREE_HPP

#include "exceptions.hpp"
#include <climits>
#include <cstddef>
#include <new>
namespace sjtu{
	template<int Size>
	class segement_tree{
	private:
		int tag[Size << 2];
		int Min[Size << 2];
		inline int ls(int x){return x << 1;}
		inline int rs(int x){return x << 1 | 1;} 
		inline void push_up(int p){Min[p] = std::min(Min[ls(p)], Min[rs(p)]);}
		inline void push_down(int p){
			if(tag[p]){
				tag[ls(p)] += tag[p];
			    Min[ls(p)] += tag[p];
			    tag[rs(p)] += tag[p];
			    Min[rs(p)] += tag[p];
			    tag[p] = 0;
			}
		}
	public:
		segement_tree() = default;
		segement_tree(const segement_tree &other){
			memcpy(tag, other.tag, sizeof(int)*4*Size);
			memcpy(Min, other.Min, sizeof(int)*4*Size);
		}
		segement_tree(int initial_data[Size]){
			build(1, 1, Size, initial_data[Size]);
		}
		~segement_tree(){};
		segement_tree &operator = (const segement_tree &other){
			new(this)segement_tree(other);
			return *this;
		}
		void build(int p, int l, int r,int initial_data[Size]){
			tag[p] = 0;
			if(l == r){
			    Min[p] = initial_data[l];
				return;	
			}
			int mid = (l + r) >> 1;
			build(ls(p), l, mid, initial_data[Size]);
			build(rs(p), mid + 1, r, initial_data[Size]);
			push_up(p);
		}
		inline void update(int L, int R, int l, int r, int p, int val){
			if(L <= l && R >= r){
				tag[p] += val;
				Min[p] += val;
				return;
			}
			push_down(p);
			int mid = (l + r) >> 1;
			if(L <= mid) update(L, R, l, mid, ls(p), val);
			if(R > mid) update(L, R, mid + 1, r, rs(p), val);
		}
		inline int query(int L, int R, int l, int r, int p){
			if(L <= l && R >= r){
				return Min[p];
			}
			push_down(p);
			int mid = (l + r) >> 1;
			if(r <= mid) return query(L, R, l, mid, ls(p));
			else if(l > mid) return query(L, R, mid + 1, r, rs(p));
			else{
				return std::min(query(L, R, l, mid, ls(p)), query(L, R, mid + 1, r, rs(p)));
			}
		}
	};
}

#endif
