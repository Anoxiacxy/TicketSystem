#ifndef SJTU_TICKET_USER_TRAIN_HPP
#define SJTU_TICKET_USER_TRAIN_HPP

#include "string.hpp"
namespace sjtu{
	class ticket_user_train{
	private:
		string<20> trainID;
		string<5> date;
		string<20> leaving_station;
		string<20> arriving_station;
		int ticket_num;
		string<5> leaving_time;
		string<5> arriving_time;
		int date_interval; 
		string<8> status;
	public:
		ticket_user_train() = default;
	};//username作为索引不包含在该类中 
} 

#endif
