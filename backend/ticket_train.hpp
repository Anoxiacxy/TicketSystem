#ifndef SJTU_TICKET_TRAIN_HPP
#define SJTU_TICKET_TRAIN_HPP

#include "train.hpp"
namespace sjtu{
	class station_train{
	private:
		string<20> station;
		string<20> trainID;
	public:
		station_train() = default;
	    station_train(const string<20> &Station, const string<20> &TrainID):station(Station), trainID(TrainID){}
	    station_train(const station_train &other){
	    	station = other.station;
	    	trainID = other.trainID;
		}
	    ~station_train() = default;
	    station_train operator = (const station_train &other){
	    	new(this)station_train(other);
	    	return *this;
		}
		inline string<20> &get_station(){
			return station;
		}
		inline string<20> &get_trainID(){
			return trainID;
		}
		bool operator < (const station_train &other) const{
			if(station < other.station) return true;
			else if(station > other.station) return false;
			else{
				if(trainID < other.trainID) return true;
				else return false;
			}
		}
	};
	class time_cost{
	public:
		string<5> saleDate_from;
		string<5> saleDate_to;
		int date_gap;
		int date_fix;
		string<5> arriving_time;
		string<5> leaving_time;
		int station_num;
		int price;
	public:
		time_cost() = default;
		time_cost(const string<5> &SaleDate_from, const string<5> &SaleDate_to, int Date_gap, int Date_fix, const string<5> &Arriving_time, const string<5> &Leaving_time, int Station_num, int Price):saleDate_from(SaleDate_from), saleDate_to(SaleDate_to), date_gap(Date_gap), date_fix(Date_fix), arriving_time(Arriving_time), leaving_time(Leaving_time), station_num(Station_num), price(Price){}
		~time_cost() = default;
	};
	class ticket_train{
	private:
		string<20> trainID;
		string<20> leaving_station;
		string<20> arriving_station;
		string<5> start_date;
		string<5> leaving_date;
		string<5> arriving_date;
		string<5> leaving_time;
		string<5> arriving_time;
		int num1;
		int num2;
		int price;
	public:
		ticket_train() = default;
		ticket_train(const string<20> &TrainID, const string<20> &Leaving_station, const string<20> &Arriving_station, const string<5> &Start_date, const string<5> &Leaving_date, const string<5> &Arriving_date, const string<5> &Leaving_time, const string<5> &Arriving_time, int Price, int Num1, int Num2):trainID(TrainID), leaving_station(Leaving_station), arriving_station(Arriving_station), start_date(Start_date), leaving_date(Leaving_date), arriving_date(Arriving_date), leaving_time(Leaving_time), arriving_time(Arriving_time), price(Price), num1(Num1), num2(Num2){}
		~ticket_train() = default;
		inline const string<5> &get_leaving_date() const{return leaving_date;}
		inline const string<5> &get_arriving_date() const{return arriving_date;}
		inline const string<5> &get_start_date() const{return start_date;}
		inline const string<5> &get_leaving_time() const{return leaving_time;}
		inline const string<5> &get_arriving_time() const{return arriving_time;}
		inline int get_price() const{return price;}
		inline int get_num1() const{return num1;}
		inline int get_num2() const{return num2;}
		inline const string<20> &get_trainID() const{return trainID;}
		inline void print(){
			std::cout << trainID << " " << leaving_station << " " << leaving_date << " " << leaving_time << " -> " << arriving_station << " " << arriving_date << " " << arriving_time << " " << price << " ";
		}
	};
	struct cmp_time{
		bool operator() (const ticket_train &tmp1, const ticket_train &tmp2){
			int t1 = minus_date(tmp1.get_leaving_date(), tmp1.get_arriving_date()) * 1440 + minus_time(tmp1.get_leaving_time(), tmp1.get_arriving_time());
			int t2 = minus_date(tmp2.get_leaving_date(), tmp2.get_arriving_date()) * 1440 + minus_time(tmp2.get_leaving_time(), tmp2.get_arriving_time());
			return t1 < t2;
		}
		bool operator() (pair<ticket_train, ticket_train> tmp1, pair<ticket_train, ticket_train> tmp2){
			int t11 = minus_date(tmp1.first.get_leaving_date(), tmp1.first.get_arriving_date()) * 1440 + minus_time(tmp1.first.get_leaving_time(), tmp1.first.get_arriving_time());
			int t12 = minus_date(tmp1.second.get_leaving_date(), tmp1.second.get_arriving_date()) * 1440 + minus_time(tmp1.second.get_leaving_time(), tmp1.second.get_arriving_time());
			int t21 = minus_date(tmp2.first.get_leaving_date(), tmp2.first.get_arriving_date()) * 1440 + minus_time(tmp2.first.get_leaving_time(), tmp2.first.get_arriving_time());
			int t22 = minus_date(tmp2.second.get_leaving_date(), tmp2.second.get_arriving_date()) * 1440 + minus_time(tmp2.second.get_leaving_time(), tmp2.second.get_arriving_time());
			return t11 + t12 < t21 + t22;
		}
	};
	struct cmp_price{
		bool operator() (const ticket_train &tmp1, const ticket_train &tmp2){
			return tmp1.get_price() < tmp2.get_price();
		} 
		bool operator() (pair<ticket_train, ticket_train> tmp1, pair<ticket_train, ticket_train> tmp2){
			return tmp1.first.get_price() + tmp1.second.get_price() < tmp2.first.get_price() + tmp2.second.get_price();
		}
	};
} 

#endif
