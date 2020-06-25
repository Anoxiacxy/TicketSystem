#ifndef SJTU_TICKET_USER_TRAIN_HPP
#define SJTU_TICKET_USER_TRAIN_HPP

#include "string.hpp"
#include "train.hpp"
#include "typedef.hpp"
namespace sjtu{
	class user_order{
	public:
		int order;
		USERNAME username;
	public:
		user_order() = default;
		user_order(int O, const USERNAME &U):order(O), username(U){}
		user_order(const user_order &other):order(other.order), username(other.username){}
		~user_order() = default; 
		bool operator < (const user_order &other) const{
			if(username < other.username) return true;
			else if(username > other.username) return false;
			else{
				if(order < other.order) return true;
				else return false;
			}
		}
	};
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
		void print(){
			std::cout << "[" << status << "] " << trainID << " " << leaving_station << " " << date << " " << leaving_time << " -> " << arriving_station << " " << add_date(date, date_gap) << " " << arriving_time << " " << price << " " << ticket_num << std::endl;
		}
		void print(std::ostream &out){
			out << "[" << status << "] " << trainID << " " << leaving_station << " " << date << " " << leaving_time << " -> " << arriving_station << " " << add_date(date, date_gap) << " " << arriving_time << " " << price << " " << ticket_num << std::endl;
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
			else if(status == string<8>("pending")){
				new(&status)string<8>("refunded");
				return 0;
			}
			else return -1;
		}
		inline void success(){
			new(&status)string<8>("success");
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
	};//username��Ϊ�����������ڸ����� 
	class queue_index{
	public:
		string<20> station; // trainID
		string<5> startdate;
		int order_num;
	//	int order_num2;
	public:
		queue_index() = default;
		queue_index(const string<20> &Station, const string<5> &Startdate, int Order_num):station(Station),startdate(Startdate),order_num(Order_num){}
		queue_index(const queue_index &other):station(other.station), startdate(other.startdate),order_num(other.order_num){}
		bool operator < (const queue_index &other) const{
			if(station < other.station) return true;
			else if(station > other.station) return false;
			else{
				if(startdate < other.startdate) return true;
				else if(startdate > other.startdate) return false;
				else{
					if(order_num < other.order_num) return true;
	//				else if(order_num > other.order_num) return false;
	//				else{
	//					if(order_num2 < other.order_num2) return true;
	//					else return false;
	//				}
	            else return false;
				}
			}
		}
		queue_index &operator =(const queue_index &other){
			new(this)queue_index(other);
			return *this;
		}
		bool operator == (const queue_index &other) const{
			return station == other.station && startdate == other.startdate && order_num == other.order_num; 
		}
	};
	class queue_value{
	public:
		int num1;
		int num2;
		int num3;
	//	int num4;
		USERNAME username;
	public:
		queue_value() = default;
		queue_value(int Num1, int Num2, int Num3, const USERNAME &Username):num1(Num1), num2(Num2), num3(Num3), username(Username){}
		queue_value(const queue_value &other):num1(other.num1), num2(other.num2), num3(other.num3), username(other.username){}
		queue_value &operator =(const queue_value &other){
			new(this)queue_value(other);
			return *this;
		}
		bool operator == (const queue_value &other) const{
			return num1 == other.num1 && num2 == other.num2 && num3 == other.num3 && username == other.username;
		}
	};
} 

#endif
