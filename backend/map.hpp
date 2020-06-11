#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP
#include <functional>
#include <cstddef>
#include <iostream>
#include "utility.hpp"
#include "exceptions.hpp"
namespace sjtu {
enum RBTreecolor{black, red};
template<class Key, class T, class Compare = std::less<Key>>
class map {
	friend class iterator;
	friend class const_iterator;
public:
    typedef pair<Key,T> value_type;
    struct RBTreeNode{
		value_type data;
		RBTreecolor color = black;
		RBTreeNode *left;
		RBTreeNode *right;
		RBTreeNode *parent;
		RBTreeNode(Key First, T Second, RBTreecolor Color, RBTreeNode *Left, RBTreeNode *Right, RBTreeNode *Parent):color(Color),left(Left),right(Right),parent(Parent){
			data = value_type(First, Second);
		}
		RBTreeNode(value_type Data, RBTreecolor Color, RBTreeNode *Left, RBTreeNode *Right, RBTreeNode *Parent):data(Data),color(Color),left(Left),right(Right),parent(Parent){}
		RBTreeNode(const RBTreeNode &other){
			data = other.data;
			left = other.left;
			right = other.right;
			parent = other.parent;
		}
	};
    class const_iterator;
    class iterator{
        friend class map;
        friend class const_iterator;
    private:
        const map *m;
        RBTreeNode *RBTreenode;
    public:
        iterator(){
            RBTreenode = NULL;
			m = NULL;
        }
        iterator(const map *ma, RBTreeNode *r):m(ma), RBTreenode(r){}
        iterator(const iterator &other){
            m = other.m;
            RBTreenode = other.RBTreenode;
        }
        iterator operator++(int){
		    iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        iterator &operator++(){
            if(*this == m->end()) {throw invalid_iterator();}
            if(RBTreenode->right != NULL){
                RBTreenode = RBTreenode->right;
                while (RBTreenode->left != NULL) RBTreenode = RBTreenode->left;
            } 
			else{
                RBTreeNode *tmp = RBTreenode;
                RBTreeNode *tmp1 = RBTreenode->parent;
                while(tmp1 != NULL && tmp == tmp1->right){
                    tmp = tmp1;
                    tmp1 = tmp1->parent;
                }
                RBTreenode = tmp1;
            }
            return *this;
        }
        iterator operator--(int){
            iterator tmp = *this;
            --(*this);    
            return tmp;
        }
        iterator &operator--(){
            if(*this == m->begin()){throw invalid_iterator();}
            else if(*this == m->end()){
                RBTreenode = m->root;
                while(RBTreenode->right != NULL) RBTreenode = RBTreenode->right;
            }
            else if(RBTreenode->left != NULL) {
                RBTreenode = RBTreenode->left;
                while(RBTreenode->right != NULL) RBTreenode = RBTreenode->right;
            } 
			else{
                RBTreeNode *tmp = RBTreenode;
                RBTreeNode *tmp1 = RBTreenode->parent;
                while (tmp1 != NULL && tmp == tmp1->left) {
                    tmp = tmp1;
                    tmp1 = tmp1->parent;
                }
                RBTreenode = tmp1;
            }
            return *this;
        }
        value_type &operator*() const{
        	if(*this == m->end()) throw invalid_iterator();
        	return RBTreenode->data;
		}
        bool operator==(const iterator &rhs) const{return m == rhs.m && RBTreenode == rhs.RBTreenode;}
        bool operator==(const const_iterator &rhs)const {return m == rhs.m && RBTreenode == rhs.RBTreenode;} 
        bool operator!= (const iterator &rhs) const{return !((*this) == rhs);}
        bool operator!= (const const_iterator &rhs)const {return !((*this) == rhs);}
        value_type* operator-> () const noexcept{return &(*(*this));}
    };
    class const_iterator:public iterator {
    public:
        const_iterator():iterator() {}
        const_iterator(map *ma, RBTreeNode *r):iterator(ma, r){}
        const_iterator (const const_iterator &other):iterator(other){}
        const_iterator (const iterator &other):iterator(other){}
    };
private:
    int Size = 0;
    RBTreeNode *root;
    RBTreeNode *create (RBTreeNode *otheroot){
    	if(otheroot == NULL) return NULL;
        RBTreeNode *tmp1 = create(otheroot->left);
		RBTreeNode *tmp2 = create(otheroot->right);
		RBTreeNode *tmp = new RBTreeNode(otheroot->data, otheroot->color, tmp1, tmp2, NULL);
		if(tmp1 != NULL) tmp1->parent = tmp;
		if(tmp2 != NULL) tmp2->parent = tmp;
		return tmp;
	}
    void Clear(RBTreeNode *&Root){
		if(Root == NULL) return;
		if(Root != NULL){
			Clear(Root->left);
			Clear(Root->right);
			delete Root;
			Root = NULL;
		}
	}
	void Colorflip(RBTreeNode *Root){
		if(Root->color == black) Root->color = red;
		else if(Root->color == red) Root->color = black;
	}
	void colorflip(RBTreeNode *Root){
		Colorflip(Root->left);
		Colorflip(Root);
		Colorflip(Root->right);
	}
	RBTreeNode *rotateright(RBTreeNode *root){
		RBTreeNode *tmp = root->left;
		root->left = tmp->right;
		if(root->left != NULL){
			root->left->parent = root;
		}
		tmp->right = root;
		if(root->parent != NULL){
			if(root->parent->left == root){
				root->parent->left = tmp;
			}
			if(root->parent->right == root){
				root->parent->right = tmp;
			}
		}
		tmp->parent = root->parent;
		root->parent = tmp;
		tmp->color = root->color;
		root->color = red;
		return tmp;
	}
	RBTreeNode *rotateleft(RBTreeNode *root){
		RBTreeNode *tmp = root->right;
		root->right = tmp->left;
		if(root->right != NULL){
			root->right->parent = root;
		}
		tmp->left = root;
		if(root->parent != NULL){
			if(root->parent->left == root){
				root->parent->left = tmp;
			}
			if(root->parent->right == root){
				root->parent->right = tmp;
			}
		}
		tmp->parent = root->parent;
		root->parent = tmp;
		tmp->color = root->color;
		root->color = red;
		return tmp;
	}
    RBTreeNode *fixup(RBTreeNode *root){
        if(root == NULL){return root;}
        if(isred(root->right) && !isred(root->left)){root = rotateleft(root);}
        if(isred(root->left) && isred(root->left->left)){root = rotateright(root);}
        if(isred(root->left) && isred(root->right)){colorflip(root);}
        return root;
    }
    RBTreeNode *balance(RBTreeNode *root){
    	if(root == NULL){return root;}
        if(isred(root->right)){root = rotateleft(root);}
        if(isred(root->left) && isred(root->left->left)){root = rotateright(root);}
        if(isred(root->left) && isred(root->right)){colorflip(root);}
        return root;
	}
    RBTreeNode *moveredleft(RBTreeNode *root) {
        colorflip(root);
        if (root->right != NULL && isred(root->right->left)){
            root->right = rotateright(root->right);
            root = rotateleft(root);
            colorflip(root);
        }
        return root;
    }
    RBTreeNode *moveredright(RBTreeNode *root) {
        colorflip(root);
        if(root->left != NULL && isred(root->left->left)){
            root = rotateright(root);
            colorflip(root);
        }
        return root;
    }
    RBTreeNode *deleteMin(RBTreeNode *root, RBTreeNode *&tmp){
        if(root == NULL){return NULL;}
        if(root->left == NULL){
            RBTreeNode *tmp1 = root->right;
            tmp = root;
            return tmp1;
        }
        if (!isred(root->left) && !isred(root->left->left)){root = moveredleft(root);}
        root->left = deleteMin(root->left, tmp);
        if(root->left != NULL) {
            root->left->parent = root;
        }
        root = balance(root);
        return root;
    }
    RBTreeNode *erase(RBTreeNode *root , const Key &key){
        if(root == NULL) {throw invalid_iterator();}
        if(Compare()(key, root->data.first)){ 
            if(root->left != NULL && !isred(root->left) && !isred(root->left->left)){root = moveredleft(root);}
            root->left = erase(root->left , key);
        } 
		else {
            if(isred(root->left)){root = rotateright(root);}
            if(!Compare()(key, root->data.first) && !Compare()(root->data.first, key) && root->right == NULL) {
                delete root;
                return NULL;
            }
            if(!isred(root->right) && !isred(root->right->left)){root = moveredright(root);}
            if(!Compare()(key, root->data.first) && !Compare()(root->data.first, key)){
                RBTreeNode *tmp = (++iterator(this, root)).RBTreenode;
                RBTreeNode *tmp1;
                root->right = deleteMin(root->right ,tmp1);
                if(root->parent != NULL && root->parent->left == root){
                    root->parent->left = tmp1;
                } 
				else if(root->parent != NULL){
                    root->parent->right = tmp1;
                }
                if(root->left != NULL){
                    root->left->parent = tmp1;
                }
                if (root->right != NULL){
                    root->right->parent = tmp1;
                }
                tmp1->color=root->color;
                tmp1->parent=root->parent;
                tmp1->right=root->right;
                tmp1->left=root->left;
                delete root;
                root = tmp1;
                } 
			else{root->right = erase(root->right, key);}
        }
        root = balance(root);
        return root;
    }
    RBTreeNode *at (RBTreeNode *root , const Key &key)const{
        if (root == NULL) { throw index_out_of_bound();}
        if (Compare()(root->data.first , key)){return at(root->right , key);} 
		else if (Compare()(key, root->data.first)){return at(root->left, key);}
		else{return root;}
    }
    bool isred (RBTreeNode *root){return root != NULL && root->color == red;}
     RBTreeNode *insert(RBTreeNode *root , const value_type &value , RBTreeNode *&tmp){
        if(root == NULL){
            tmp = new RBTreeNode(value, red, NULL, NULL, NULL);
            return tmp;
        }
        if(Compare()(root->data.first, value.first)){
            root->right = insert(root->right, value, tmp);
            root->right->parent = root;
        } 
		else if(Compare()(value.first, root->data.first)){
            root->left = insert(root->left, value, tmp);
            root->left->parent = root;
        } 
		else{
            tmp = root;
            throw invalid_iterator();
        }
        root = fixup(root);
        return root;
    }
public:
    map(){root = NULL;}
    map(const map &other){
    	Size = other.Size;
        root = create(other.root);
    }
    map &operator=(const map &other){
        if(this == &other) {return *this;}
        Clear(root);
        root = create(other.root);
        Size = other.Size;
        return *this;
    }
    ~map(){Clear(root);} 
public:
    T & at(const Key &key){
	    return at(root, key)->data.second;
	}
	const T & at(const Key &key) const{return at(root, key)->data.second;}
    T & operator[] (const Key &key){
        try{return at(key);}catch(index_out_of_bound){
            RBTreeNode *tmp;
            root=(insert(root, value_type(key, T()), tmp));
            Size++;
            return at(key);
        }
    }
    const T &operator[] (const Key &key) const{return at(key);}
    iterator begin() const {
		if(root == NULL) return iterator(this, root);
		RBTreeNode *tmp = root;
		while(tmp->left != NULL){
			tmp = tmp->left;
		}
		return iterator(this, tmp); 
	}
	const_iterator cbegin() const{return const_iterator(begin());}
    iterator end() const{return iterator(this, NULL);}
	const_iterator cend() const{return const_iterator(end());}
    bool empty () const{return Size == 0;}
    size_t size () const{return Size;}
    void clear (){
        Clear(root);
        Size = 0;
    }
public:
    pair<iterator, bool>insert(const value_type &value){
        RBTreeNode *tmp;
        if (root == NULL){
            root = new RBTreeNode(value, black, NULL, NULL, NULL);
            Size++;
            return pair<iterator, bool>(iterator(this, root),true);
        }
        try{
            root = insert(root, value, tmp);
            root->color = black;
            Size++;
            return pair<iterator, bool>(iterator(this, tmp) , true);
        }catch(invalid_iterator){return pair<iterator, bool>(iterator(this, tmp) , false);}
    }
    void erase(iterator iter){
        if(iter == end() || iter.m!=this){throw invalid_iterator();}
        if (!isred(root->left) && !isred(root->right)){
            root->color = red;
        }
        root = erase(root, iter->first);
        Size--;
        if(root != NULL){root->color = black;}
    }
    size_t count(const Key &key) const{
        try{
            at(key);
            return 1;
        }catch(index_out_of_bound){return 0;}
    }
    iterator find(const Key &key){
        try{
            RBTreeNode *tmp = at(root, key);
            return iterator(this, tmp);
        }catch(index_out_of_bound){return end();}
    }
    const_iterator find(const Key &key) const {
        try{
            RBTreeNode *tmp = at(root, key);
            return const_iterator(iterator(this, tmp));
        }catch(index_out_of_bound){return cend();}
    }
};
}
#endif
