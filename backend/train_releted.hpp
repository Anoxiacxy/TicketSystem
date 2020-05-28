#ifndef SJTU_TRAIN_RELATED_HPP
#define SJTU_TRAIN_RELATED_HPP

#include "train.hpp"

namespace sjtu{
	class train_related{
	private:
		bptree<string<20>, pair<train, seat>> traintree;
	public:
	    int add_train(){
		} 
		int release_train(){
		}
	};
}
