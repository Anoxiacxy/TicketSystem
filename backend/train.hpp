#ifndef SJTU_TRAIN_HPP
#define SJTU_TRAIN_HPP

#include "string.hpp"
#include "typedef.hpp"
#include "segement_tree.hpp"
namespace sjtu{
	int cmp_date(const string<5> &date1, const string<5> &date2){
		int Month1 = (date1[0] - '0') * 10 + date1[1] - '0';
		int Month2 = (date2[0] - '0') * 10 + date2[1] - '0';
		int day1 = (date1[3] - '0') * 10 + date1[4] - '0';
		int day2 = (date2[3] - '0') * 10 + date2[4] - '0';
		if(Month1 < Month2) return -1;
		else if(Month1 > Month2) return 1;
		else{
			if(day1 < day2) return -1;
			else if(day1 == day2) return 0;
			else return 1;
		}
	}
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
	pair<int, string<5> > add_time(const string<5> &time, int time_gap){
		int hour = (time[0] - '0') * 10 + time[1] - '0';
		int minute = (time[3] - '0') * 10 + time[4] - '0';
		int res = hour * 60 + minute + time_gap;
		int ans = 0;
		ans += res /1440;
		res = res % 1440;
		char a[6];
		a[0] = res / 600 + '0';
		a[1] = (res / 60) % 10 + '0';
		a[2] = ':';
		a[3] = (res % 60) / 10 + '0';
		a[4] = (res % 60) % 10 + '0';
		a[5] = '\0';
		return pair<int, string<5> >(ans, string<5>(a));
	}
	string<5> add_date(const string<5> &date, int date_gap){
		if(date_gap == 0) return date;
		int month[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		int Month = (date[0] - '0') * 10 + date[1] - '0';
		int day = (date[3] - '0') * 10 + date[4] - '0'+ date_gap;
		while(day > month[Month]){
			day -= month[Month];
			++Month;
		}
		char a[6];
		a[0] = Month / 10 + '0';
		a[1] = Month % 10 + '0';
		a[2] = '-';
		a[3] = day / 10 + '0';
		a[4] = day % 10 + '0';
		a[5] = '\0';
		return string<5>(a);
	}
	string<5> minus_date(const string<5> &date, int date_gap){
		int month[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		int Month = (date[0] - '0') * 10 + date[1] - '0';
		int day = (date[3] - '0') * 10 + date[4] - '0' - date_gap;
		while(day <= 0){
			day += month[Month - 1];
			--Month;
		}
		char a[6];
		a[0] = Month / 10 + '0';
		a[1] = Month % 10 + '0';
		a[2] = '-';
		a[3] = day / 10 + '0';
		a[4] = day % 10 + '0';
		a[5] = '\0';
		return string<5>(a);
	}
	class route{
	public:
		string<5> date;
		int leaving_station;
		int arriving_station;
		string<5> leaving_time;
		string<5> arriving_time;
		int date_gap;
		int date_fix;
		int date_fix1;
		int price = 0;
	public:
		route() = default;
	    route(const string<5> &Date, int Leaving_station, int Arriving_station, const string<5> Leaving_time, const string<5> Arriving_time, int Date_gap, int Date_fix, int Date_fix1, int Price):date(Date), leaving_station(Leaving_station), arriving_station(Arriving_station), leaving_time(Leaving_time), arriving_time(Arriving_time), date_gap(Date_gap), date_fix(Date_fix), date_fix1(Date_fix1), price(Price){}
		~route() = default; 
		bool operator == (const route &other) const{
			return date == other.date && leaving_station == other.leaving_station && arriving_station == other.arriving_station && leaving_time == other.leaving_time && arriving_time == other.arriving_time && date_gap == other.date_gap && date_fix == other.date_fix && date_fix1 == other.date_fix1 && price == other.price;
		}
	};
	class train{
	public: //除了trainID的其他信息 
		int stationNum = 0;
		string<20> *stations;
//		int *prices;
		string<5> startTime;
//		int *travelTimes;
//		int *stopoverTimes;
		string<5> saleDate_from;
		string<5> saleDate_to;  
		char type;
//		segement_tree *seat_num;
		bool release = false;
		int seatnum;
		int pos1;
		int pos2;
	public:
		train() = default;
	    train(int StationNum, string<20> *Stations, int *Prices, const string<5> &StartTime, int *TravelTimes, int *StopoverTimes, const string<5> &SaleDate_from, const string<5> &SaleDate_to, char Type, int SeatNum, int Pos1, int Pos2):stationNum(StationNum), startTime(StartTime), saleDate_from(SaleDate_from), saleDate_to(SaleDate_to), type(Type), seatnum(SeatNum), pos1(Pos1), pos2(Pos2){
//	    	stations = new string<20>[stationNum];
//	    	prices = new int[stationNum]; //空出首位 
//	    	travelTimes = new int[stationNum]; //空出数组首位置，从后一位开始记录
//			stopoverTimes = new int[stationNum]; //空出首位和末位 
//			int Date_interval = minus_date(saleDate_from, saleDate_to) + 1;
//			seat_num = new segement_tree[Date_interval];
//			int initial_data[stationNum];
//			for(int i = 1; i < stationNum; ++i) initial_data[i] = SeatNum;
//			segement_tree tmp(initial_data, stationNum - 1);
//			for(int i = 0; i < Date_interval; ++i){
//				seat_num[i] = tmp;
//			}
//	    	for(int i = 0; i < stationNum; ++i){
//	    		stations[i] = Stations[i];
//				travelTimes[i] = TravelTimes[i];
//				stopoverTimes[i] = StopoverTimes[i];
//				prices[i] = Prices[i];		
//			}
		}
		train(const train &other):stationNum(other.stationNum), startTime(other.startTime), saleDate_from(other.saleDate_from), saleDate_to(other.saleDate_to), type(other.type), release(other.release), seatnum(other.seatnum), pos1(other.pos1), pos2(other.pos2){
//			stations = new string<20>[stationNum];
//	    	prices = new int[stationNum]; 
//	    	travelTimes = new int[stationNum]; 
////			stopoverTimes = new int[stationNum]; 
	//		int Date_interval = minus_date(saleDate_from, saleDate_to) + 1;
	//		seat_num = new segement_tree[Date_interval];
	//		for(int i = 0; i < Date_interval; ++i){
	//			seat_num[i] = other.seat_num[i];
	//		}
	//    	for(int i = 0; i < stationNum; ++i){
	  //  		stations[i] = other.stations[i];
		//		travelTimes[i] = other.travelTimes[i];
		//		stopoverTimes[i] = other.stopoverTimes[i];
		//		prices[i] = other.prices[i];		
		//	}
		}
		~train(){
		//	if(stationNum != 0){
		//	    delete [] stations;
		//	    delete [] travelTimes;
		//	    delete [] stopoverTimes;
		//	    delete [] seat_num;
		//	    delete [] prices;
		  //  }
		}
		train &operator = (const train& other){
			new(this)train(other);
			return *this;
		} 
		inline bool isrelease(){
			return release;
		}
		inline void modify_release(){
			release = true;
		}
		inline string<5> &get_saleDate_from(){
			return saleDate_from;
		}
		inline string<5> &get_saleDate_to(){
			return saleDate_to;
		}
/*		void print(const string<5> &date){
			std::cout << type << std::endl;
			int date_gap = 0;
			int date_fix = 0;
			string<5> arriving_time = startTime;
			string<5> leaving_time = startTime;
			string<5> arriving_date = date;
			string<5> leaving_date = date;
			int price = 0;
			int date_interval = minus_date(saleDate_from, date);
			std::cout << stations[0] << " xx-xx" << " xx:xx" << " -> " << date << " " << leaving_time << " " << price << " " << seat_num[date_interval].query(1, 1, stationNum - 1, 1) << std::endl; 
			price += prices[1];
			pair<int, string<5> > tmpTime1 = add_time(leaving_time, travelTimes[1]);
			date_fix += tmpTime1.first;
			pair<int, string<5> > tmpTime3 = add_time(leaving_time, travelTimes[1] + stopoverTimes[1]);
			date_gap = tmpTime3.first - tmpTime1.first;
			arriving_date = add_date(leaving_date, tmpTime1.first);
			leaving_date = add_date(arriving_date, date_gap);
			arriving_time = tmpTime1.second;
			leaving_time = tmpTime3.second;
			for(int i = 1; i < stationNum - 1; ++i){
				std::cout << stations[i] << " " << arriving_date << " " << arriving_time << " -> " << leaving_date << " " << leaving_time << " " << price << " " << seat_num[date_interval].query(i + 1, 1, stationNum - 1, 1) << std::endl;
			    price += prices[i + 1];
			    pair<int, string<5> > tmpTime = add_time(leaving_time, travelTimes[i + 1]);
				date_fix += tmpTime.first;
				pair<int, string<5> > tmpTime2 = add_time(leaving_time, travelTimes[i + 1] + stopoverTimes[i + 1]);
				date_gap = tmpTime2.first - tmpTime.first;
				arriving_date = add_date(leaving_date, tmpTime.first);
				leaving_date = add_date(arriving_date, date_gap);
				arriving_time = tmpTime.second;
				leaving_time = tmpTime2.second;
			}
			std::cout << stations[stationNum - 1] << " " << arriving_date << " " << arriving_time << " -> xx-xx" << " xx:xx " << price <<  " x\n";
		} */
/*		route query_route(const string<5> &Date, const string<20> &Leaving_station, const string<20> &Arriving_station){
			int date_gap = 0;
			int date_fix = 0;
			string<5> arriving_time = startTime;
			string<5> leaving_time = startTime;
			int price = 0;
			int date_fix2 = 0;
			int date_gap1, date_fix1, price1;
			string<5> leaving_time1;
			int tmp1 = -1; 
			int tmp2;
			for(int i = 0; i < stationNum; ++i){
				if(stations[i] == Leaving_station){
					date_gap1 = date_gap;
					date_fix1 = date_fix2;
					price1 = price;
					leaving_time1 = leaving_time;
					tmp1 = i;
				}
				if(stations[i] == Arriving_station){
					tmp2 = i;
					if(tmp1 == -1){
						return route();
					}
					break;
				}
				if(i == stationNum - 1){
					return route();
				}
			    price += prices[i + 1];
			    pair<int, string<5> > tmpTime = add_time(leaving_time, travelTimes[i + 1]);
				date_fix += tmpTime.first;
				date_fix2 = date_fix;
				pair<int, string<5> > tmpTime2 = add_time(leaving_time, travelTimes[i + 1] + stopoverTimes[i + 1]);
				date_gap = tmpTime2.first - tmpTime.first;
				date_fix += date_gap;
				arriving_time = tmpTime.second;
				leaving_time = tmpTime2.second;
			}
			return route(Date, tmp1, tmp2, leaving_time1, arriving_time,  date_fix1 + date_gap1, date_fix2 - date_fix1 - date_gap1, date_fix1 + date_gap1, price - price1);
		}
		inline bool query_ticket(int num1, int num2, const string<5> &Date, int TicketNum){
			int date_interval = minus_date(saleDate_from, Date);
			int data = seat_num[date_interval].query(num1 + 1, num2, 1, stationNum - 1, 1);
			if(TicketNum <= data){
				seat_num[date_interval].update(num1 + 1, num2, 1, stationNum - 1, 1, -TicketNum);
				return true;
			}
			else{
				return false;
			}
		}
		inline void refund_ticket(int num1, int num2, const string<5> &Date, int TicketNum){
			int date_interval = minus_date(saleDate_from, Date);
			seat_num[date_interval].update(num1 + 1, num2, 1, stationNum - 1, 1, TicketNum);
		}
		
		inline int query_seat(int num1, int num2, const string<5> &Date){
			int date_interval = minus_date(saleDate_from, Date);
			return seat_num[date_interval].query(num1 + 1, num2, 1, stationNum - 1, 1);
		}*/
	};
}

#endif
