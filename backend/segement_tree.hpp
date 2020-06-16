#ifndef SJTU_SEGEMENT_TREE_HPP
#define SJTU_SEGEMENT_TREE_HPP

#include "exceptions.hpp"
#include <climits>
#include <cstddef>
#include <new>
namespace sjtu{
	class segement_tree{
	private:
        int Size = 0;
		int *tag;
		int *Min; 
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
		segement_tree(int size):Size(size){
			tag = new int[Size << 2];
			Min = new int[Size << 2];
		};
		segement_tree(const segement_tree &other):Size(other.Size){
			if(Size != 0){
				tag = new int[Size << 2];
			    Min = new int[Size << 2];
			    memcpy(tag, other.tag, sizeof(int)*4*Size);
			    memcpy(Min, other.Min, sizeof(int)*4*Size);
			}
		}
		segement_tree(int initial_data[], int size):Size(size){
			tag = new int[Size << 2];
			Min = new int[Size << 2];
			build(1, 1, Size, initial_data);
		}
		~segement_tree(){
			if(Size != 0){
			    delete [] tag;
			    delete [] Min;
		    }
		};
		segement_tree &operator = (const segement_tree &other){
			new(this)segement_tree(other);
			return *this;
		}
		void build(int p, int l, int r,int initial_data[]){
			tag[p] = 0;
			if(l == r){
			    Min[p] = initial_data[l];
				return;	
			}
			int mid = (l + r) >> 1;
			build(ls(p), l, mid, initial_data);
			build(rs(p), mid + 1, r, initial_data);
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
			push_up(p);
		}
		inline int query(int L, int R, int l, int r, int p){
			if(L <= l && R >= r){
				return Min[p];
			}
			push_down(p);
			int mid = (l + r) >> 1;
			if(R <= mid) return query(L, R, l, mid, ls(p));
			else if(L > mid) return query(L, R, mid + 1, r, rs(p));
            else return std::min(query(L, R, l, mid, ls(p)), query(L, R, mid + 1, r, rs(p)));
		}
		inline int query(int num, int l, int r, int p){
			if(l == r){
				return Min[p];
			}
			push_down(p);
			int mid = (l + r) >> 1;
			if(num <= mid) return query(num, l, mid, ls(p));
			else if(num > mid) return query(num, mid + 1, r, rs(p));
		}
	};
}

#endif
