#ifndef SJTU_TRAIN_HPP
#define SJTU_TRAIN_HPP

#include "string.hpp"
#include "typedef.hpp"
#include "segement_tree.hpp"
namespace sjtu{
	int minus_date(const string<5> &date1, const string<5> &date2){
		int month[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		int res = 0;
		int Month1 = (date1[0] - '0') * 10 + date1[1] - '0';
		int Month2 = (date2[0] - '0') * 10 + date2[1] - '0';
		int day1 = (date1[3] - '0') * 10 + date1[4] - '0';
		int day2 = (date2[3] - '0') * 10 + date2[4] - '0';
		for(int i = Month1; i < Month2; ++i) res += month[i];
		res += (day2 - day1);
		return res;
	}
	int minus_time(const string<5> &time1, const string<5> &time2){
		int res = 0;
		int hour1 = (time1[0] - '0') * 10 + time1[1] - '0';
		int hour2 = (time2[0] - '0') * 10 + time2[1] - '0';
		int minute1 = (time1[3] - '0') * 10 + time1[4] - '0';
		int minute2 = (time2[3] - '0') * 10 + time2[4] - '0';
		res += (hour2 - hour1) * 60;
		res += (minute2 - minute1);
		return res;
	}
	class train{
	private: //除了trainID的其他信息 
		int stationNum;
		string<20> *stations;
		int *prices;
		string<5> startTime;
		int *travelTimes;
		int *stopoverTimes;
		string<5> saleDate_from;
		string<5> saleDate_to;  
		char type;
	public:
	    train(int StationNum, string<20> *Stations, int *Prices, const string<5> &StartTime, int *TravelTimes, int *StopoverTimes, const string<5> &SaleDate_from, const string<5> &SaleDate_to, char Type):stationNum(StationNum), startTime(StartTime), saleDate_from(SaleDate_from), saleDate_to(SaleDate_to), type(Type){
	    	stations = new string<20>[stationNum];
	    	travelTimes = new int[stationNum]; //空出数组首位置，从后一位开始记录
			stopoverTimes = new int[stopoverTimes]; //空出首位和末位 
	    	for(int i = 0; i < stationNum; ++i){
	    		stations[i] = Stations[i];
				travelTimes[i] = TravelTimes[i];
				stopoverTimes[i] = StopoverTimes[i];		
			}
		}
		~train(){
			delete [] stations;
			delete [] travelTimes;
			delete [] stopoverTimes;
		} 
	};
	template <int num>
	class seat{  //与train用pair合起来作为信息 
	private:
		int date_interval;
		segement_tree<num> *seat_num;
		bool isrealse = false; 
	public:
	    seat(int Date_interval, int initial_data[num]):date_interval(Date_interval){
	    	seat_num = new segement_tree<num>[Date_interval];
			segement_tree<num> tmp(initial_data[num]);
			for(int i = 0; i < Date_interval; ++i){
				seat_num[i] = tmp;
			}
		}
		~seat(){
			delete [] seat_num;
		} 
	};
}

#endif
