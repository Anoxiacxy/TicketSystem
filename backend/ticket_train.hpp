#ifndef SJTU_TICKET_TRAIN_HPP
#define SJTU_TICKET_TRAIN_HPP

#include "string.hpp"
namespace sjtu{
	class ticket_train{
	private:
		string<20> trainID;
		string<20> leaving_station;
		string<20> arriving_station;
		string<5> leaving_date;
		string<5> arriving_date;
		string<5> leaving_time;
		string<5> arriving_time;
		int price;
		int seat;
	};
	class station_train{
	private:
		string<20> station;
		string<20> trainID;
	public:
		station_train = default;
	    station_train(const string<20> &Station, const string<20> &TrainID):station(Station), trainID(TrainID){}
	    station_train(const station_train &other){
	    	station = other.station;
	    	trianID = other.trainID;
		}
	    ~station_train = default;
	    station_train operator = (const station_train &other){
	    	new(this)station_train(other);
	    	return *this;
		}
		bool operator < (const station_train &other){
			if(station < other.station) return true;
			else if(station > other.station) return false;
			else{
				if(trainID < other.trainID) return true;
				else return false;
			}
		}
	};
	class time_cost{
	private:
		string<5> saleDate_from;
		string<5> saleDate_to;
		int date_gap;
		string<5> arriving_time;
		string<5> leaving_time;
		int station_num;
		int price;//从首发站算起的票价 
	};
} 

#endif;
