#ifndef SJTU_TRAIN_RELATED_HPP
#define SJTU_TRAIN_RELATED_HPP

#include "train.hpp"
#include "../uti.hpp"
namespace sjtu{
	char time1[] = "station1";
	char time2[] = "time1";
	char time3[] = "time2";
	char price1[] = "price1";
	char seat1[] = "seat1";
	class train_related{
	public:
		bptree<string<20>, train> traintree;
		SJTU::file_vector<string<20>, time1> Station1;
		SJTU::file_vector<int, time2> Time1;
		SJTU::file_vector<int, time3> Time2;
		SJTU::file_vector<int, price1> Price1;
		SJTU::file_vector<int, seat1> Seat1;
	public:
		train_related():traintree("traintree", "trainindex"){} 
	    int add_train(string<20> TrainID, int StationNum, string<20> *Stations, int *Prices, const string<5> &StartTime, int *TravelTimes, int *StopoverTimes, const string<5> &SaleDate_from, const string<5> &SaleDate_to, char Type, int SeatNum){
		    int success = traintree.count(TrainID);
			if(success == 1) return -1;
			else{
				int pos1, pos2;
				pos1 = Station1.size();
				pos2 = Seat1.size();
			//	std::cout << pos1 << " " << pos2 << " WWWW" << std::endl; 
				for(int i = 0; i < StationNum; ++i){
					Station1.push_back(Stations[i]);
					Time1.push_back(TravelTimes[i]);
					Time2.push_back(StopoverTimes[i]);
					Price1.push_back(Prices[i]);
				}
				for(int i = 0; i < minus_date(SaleDate_from, SaleDate_to) + 1; ++i){
					for(int j = 0; j < StationNum - 1; ++j){
						Seat1.push_back(SeatNum);
					}
				}
			//	std::cout << Station1.size() << " " << Seat1.size() << " WWWW" << std::endl;
				traintree.insert(TrainID, train(StationNum, Stations, Prices, StartTime, TravelTimes, StopoverTimes, SaleDate_from, SaleDate_to, Type, SeatNum, pos1, pos2));
			    return 0;
			}	    	
		} 
		int release_train(const string<20> &TrainID){
			train tmp = traintree.at(TrainID);
			if(tmp.stationNum == 0) return -1;
			else{
				if(tmp.isrelease()) return -1;
				else{
					tmp.modify_release();
					traintree.modify(TrainID, tmp);
					return 0;
				}
			}
		}
		bool release(const string<20> &TrainID){	
			train tmp = traintree.at(TrainID);
			if(tmp.stationNum == 0) return false;
			else{
				if(tmp.isrelease()) return true;
				else return false;
			}
		}
		pair<bool, pair<string<20>, train> > query_train(const string<20> &TrainID){
			train tmp = traintree.at(TrainID);
			if(tmp.stationNum == 0) return pair<bool, pair<string<20>, train> >(false, pair<string<20>, train>(TrainID, tmp));
			else{
				return pair<bool, pair<string<20>, train> >(true, pair<string<20>, train>(TrainID, tmp));
			}
		}
		int delete_train(const string<20> &TrainID){
			train tmp = traintree.at(TrainID);
			if(tmp.stationNum == 0 || tmp.isrelease()) return -1;
			else{
				traintree.erase(TrainID);
				return 0; 
			}
		}
		pair<int, route> query_ticket(const string<20> &TrainID, const string<5> &Date, const string<20> &Leaving_station, const string<20> &Arriving_station, int TicketNum){
			train tmp = traintree.at(TrainID);
		//	std::cout << tmp.pos1 << "WWW" << tmp.pos2 << std::endl;
			if(tmp.stationNum == 0) return pair<int, route>(-1, route());
			else if(tmp.seatnum < TicketNum) return pair<int, route>(-1, route());
			else{
				//route Route = tmp.query_route(Date, Leaving_station, Arriving_station);
				//if(Route == route()) return pair<int, route>(-1, route());
				bool flag = true;
				int date_gap = 0;
			    int date_fix = 0;
			    string<5> arriving_time = tmp.startTime;
			    string<5> leaving_time = tmp.startTime;
			    int price = 0;
			    int date_fix2 = 0;
			    int date_gap1, date_fix1, price1;
			    string<5> leaving_time1;
			    int tmp1 = -1; 
			    int tmp2;
			    for(int i = 0; i < tmp.stationNum; ++i){
	//		    	std::cout << Station1[tmp.pos1 + i] << std::endl;
	//		    	std::cout << Price1[tmp.pos1 + i + 1] << std::endl;
	//		    	std::cout << Time1[tmp.pos1 + i + 1] << std::endl;
	//		    	std::cout << Time2[tmp.pos1 + i + 1] << std::endl;
				    if(Station1[tmp.pos1 + i] == Leaving_station){
					    date_gap1 = date_gap;
					    date_fix1 = date_fix2;
					    price1 = price;
					    leaving_time1 = leaving_time;
					    tmp1 = i;
				    }
				    if(Station1[tmp.pos1 + i] == Arriving_station){
					    tmp2 = i;
					    if(tmp1 == -1){
						    flag = false;
					    }
					    break;
				    }
				    if(i == tmp.stationNum - 1){
					    flag = false;
				    }
			        price += Price1[tmp.pos1 + i + 1];
			        pair<int, string<5> > tmpTime = add_time(leaving_time, Time1[tmp.pos1 + i + 1]);
				    date_fix += tmpTime.first;
				    date_fix2 = date_fix;
				    pair<int, string<5> > tmpTime2 = add_time(leaving_time, Time1[tmp.pos1 + i + 1] + Time2[tmp.pos1 + i + 1]);
				    date_gap = tmpTime2.first - tmpTime.first;
				    date_fix += date_gap;
				    arriving_time = tmpTime.second;
				    leaving_time = tmpTime2.second;
				}
				if(!flag) return pair<int, route>(-1, route());
				else{
					route Route(Date, tmp1, tmp2, leaving_time1, arriving_time,  date_fix1 + date_gap1, date_fix2 - date_fix1 - date_gap1, date_fix1 + date_gap1, price - price1);
					string<5> tmpDate = minus_date(Date, Route.date_fix1);
					if(cmp_date(tmp.get_saleDate_from(), tmpDate) == 1 || cmp_date(tmp.get_saleDate_to(), tmpDate) == -1) return pair<int, route>(-1, route());
				//	bool flag1 = tmp.query_ticket(Route.leaving_station, Route.arriving_station, tmpDate, TicketNum);
					int min = 1000000;
	//				std::cout << tmp.pos2 + minus_date(tmp.saleDate_from, tmpDate) * (tmp.stationNum - 1) + Route.leaving_station << "2333"<<std::endl;
					for(int i = tmp.pos2 + minus_date(tmp.saleDate_from, tmpDate) * (tmp.stationNum - 1) + Route.leaving_station; i < tmp.pos2 + minus_date(tmp.saleDate_from, tmpDate) * (tmp.stationNum - 1) + Route.arriving_station; ++i){
				        if(min > Seat1[i]) min = Seat1[i];
			        }
			        bool res;
			        if(min < TicketNum) res = false;
			        else{
				        res = true;
				        for(int i = tmp.pos2 + minus_date(tmp.saleDate_from, tmpDate) * (tmp.stationNum - 1) + Route.leaving_station; i < tmp.pos2 + minus_date(tmp.saleDate_from, tmpDate) * (tmp.stationNum - 1) + Route.arriving_station; ++i){
				            Seat1.modify(i, Seat1[i] - TicketNum);
			            }
			        }
					if(res){
					//	traintree.modify(TrainID, tmp);
						return pair<int, route>(1, Route);
					} 
					else return pair<int, route>(0, Route);
				}
			}
		}
		inline void refund_ticket(const string<20> &TrainID, const string<5> &date, int ticketNum, int num1, int num2){
			train tmp = traintree.at(TrainID);
		//	tmp.refund_ticket(num1, num2, date, ticketNum);
			for(int i = tmp.pos2 + minus_date(tmp.saleDate_from, date) * (tmp.stationNum - 1) + num1; i < tmp.pos2 + minus_date(tmp.saleDate_from, date) * (tmp.stationNum - 1) + num2; ++i){
				Seat1.modify(i, Seat1[i] + ticketNum);
			}
		//	traintree.modify(TrainID, tmp);
		}
		inline bool query_num(const string<20> &TrainID, const string<5> &date, int ticketNum, int num1, int num2){
			train tmp = traintree.at(TrainID);
		//	bool res = tmp.query_ticket(num1, num2, date, ticketNum);
		//	traintree.modify(TrainID, tmp);
		    int min = 1000000;
			for(int i = tmp.pos2 + minus_date(tmp.saleDate_from, date) * (tmp.stationNum - 1) + num1; i < tmp.pos2 + minus_date(tmp.saleDate_from, date) * (tmp.stationNum - 1) + num2; ++i){
				if(min > Seat1[i]) min = Seat1[i];
			}
			bool res;
			if(min < ticketNum) res = false;
			else{
				res = true;
				for(int i = tmp.pos2 + minus_date(tmp.saleDate_from, date) * (tmp.stationNum - 1) + num1; i < tmp.pos2 + minus_date(tmp.saleDate_from, date) * (tmp.stationNum - 1) + num2; ++i){
				    Seat1.modify(i, Seat1[i] - ticketNum);
			    }
			}
			return res;
		}
		
		inline int query_seat(const string<20> &TrainID, const string<5> &date, int num1, int num2){
			train tmp = traintree.at(TrainID);
			int min = 1000000;
	//		std::cout << tmp.pos2 + minus_date(tmp.saleDate_from, date) * (tmp.stationNum - 1) + num1 << "www" << std::endl;
	//		std::cout << tmp.pos2 + minus_date(tmp.saleDate_from, date) * (tmp.stationNum - 1) + num2 << "www" << std::endl;
			for(int i = tmp.pos2 + minus_date(tmp.saleDate_from, date) * (tmp.stationNum - 1) + num1; i < tmp.pos2 +minus_date(tmp.saleDate_from, date) * (tmp.stationNum - 1) + num2; ++i){
				if(min > Seat1[i]) min = Seat1[i];
			}
			return min; 
			//return tmp.query_seat(num1, num2, date);
		}
		
		inline train at(const string<20> &TrainID){
			return traintree.at(TrainID);
		}
	};
}

#endif
