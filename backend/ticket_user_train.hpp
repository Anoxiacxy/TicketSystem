#ifndef SJTU_TICKET_USER_TRAIN_HPP
#define SJTU_TICKET_USER_TRAIN_HPP

#include "string.hpp"
#include "train.hpp"
namespace sjtu{
	class ticket_user_train{
	private:
		string<20> trainID;
		string<5> date;
		string<20> leaving_station;
		string<20> arriving_station;
		int ticket_num;
		int price;
		string<5> leaving_time;
		string<5> arriving_time;
		int date_gap;
		int date_fix; 
		string<8> status;
		int leaving_num;
		int arriving_num;
	public:
		ticket_user_train() = default;
		ticket_user_train(const string<20> &TrainID, const string<5> &Date, const string<20> &Leaving_station, const string<20> &Arriving_station, int Ticket_num, const string<5> &Leaving_time, const string<5> &Arriving_time, int Date_gap, int Date_fix, const string<8> &Status, int Price, int Leaving_num, int Arriving_num):trainID(TrainID), date(Date), leaving_station(Leaving_station), arriving_station(Arriving_station), ticket_num(Ticket_num), leaving_time(Leaving_time), arriving_time(Arriving_time), date_gap(Date_gap), date_fix(Date_fix), status(Status), price(Price), leaving_num(Leaving_num), arriving_num(Arriving_num){}
		~ticket_user_train() = default; 
		friend inline std::ostream &operator <<(std::ostream &os, const ticket_user_train &tut){
			os << "[" << tut.status << "] " << tut.trainID << " " << tut.leaving_station << " " << tut.date << " " << tut.leaving_time << " -> " << tut.arriving_station << " " << add_date(tut.date, tut.date_gap) << " " << tut.arriving_time << " " << tut.price << " " << tut.ticket_num;
			return os;
		}
		ticket_user_train &operator = (const ticket_user_train &other){
			trainID = other.trainID;
			date = other.date;
			leaving_station = other.leaving_station;
			arriving_station = other.arriving_station;
			ticket_num = other.ticket_num;
			price = other.price;
			leaving_time = other.leaving_time;
			arriving_time = other.arriving_time;
			date_gap = other.date_gap;
			date_fix = other.date_fix;
			status = other.status;
			leaving_num = other.leaving_num;
			arriving_num = other.arriving_num;
		}
		bool operator == (const ticket_user_train &other) const{
			return trainID == other.trainID && date == other.date && leaving_station == other.leaving_station && arriving_station == other.arriving_station && ticket_num == other.ticket_num && price == other.price && leaving_time == other.leaving_time && arriving_time == other.arriving_time && date_gap == other.date_gap && date_fix == other.date_fix && status == other.status && leaving_num == other.leaving_num && arriving_num == other.arriving_num;
		}
		bool operator != (const ticket_user_train &other) const{
			return !((*this) == other);
		}
		inline int refund_ticket(){
			if(status == string<8>("success")) {
				new(&status)string<8>("refunded");
				return 1;
			}
			else if(status == string<8>("queue")){
				new(&status)string<8>("refunded");
				return 0;
			}
			else return -1;
		}
		inline int get_ticketNum(){
			return ticket_num;
		}
		inline int get_num1() const{
			return leaving_num;
		}
		inline int get_num2() const{
			return arriving_num;
		}
		inline string<20> &get_trainID(){
		    return trainID;
		}
		inline string<5> get_startdate(){
			return minus_date(date, date_fix);
		}
	};//username作为索引不包含在该类中 
} 

#endif
