#ifndef SJTU_TRAIN_RELATED_HPP
#define SJTU_TRAIN_RELATED_HPP

#include "train.hpp"
#include "../uti.hpp"
namespace sjtu{
	class train_related{
	private:
		bptree<string<20>, train> traintree;
	public:
		train_related():traintree("traintree", "trainindex"){} 
	    int add_train(string<20> TrainID, int StationNum, string<20> *Stations, int *Prices, const string<5> &StartTime, int *TravelTimes, int *StopoverTimes, const string<5> &SaleDate_from, const string<5> &SaleDate_to, char Type, int SeatNum){
		    int success = traintree.count(TrainID);
			if(success == 1) return -1;
			else{
				traintree.insert(TrainID, train(StationNum, Stations, Prices, StartTime, TravelTimes, StopoverTimes, SaleDate_from, SaleDate_to, Type, SeatNum));
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
			if(tmp.stationNum == 0) return pair<int, route>(-1, route());
			else{
				route Route = tmp.query_route(Date, Leaving_station, Arriving_station);
				if(Route.price == -1) return pair<int, route>(-1, route());
				else{
					string<5> tmpDate = minus_date(Date, Route.date_fix);
					bool flag = tmp.query_ticket(Route.leaving_station, Route.arriving_station, tmpDate, TicketNum);
					if(flag) return pair<int, route>(1, Route);
					else return pair<int, route>(0, Route);
				}
			}
		}
		inline void refund_ticket(const string<20> &TrainID, const string<5> &date, int ticketNum, int num1, int num2){
			train tmp = traintree.at(TrainID);
			tmp.refund_ticket(num1, num2, date, ticketNum);
		}
		inline bool query_num(const string<20> &TrainID, const string<5> &date, int ticketNum, int num1, int num2){
			train tmp = traintree.at(TrainID);
			return tmp.query_ticket(num1, num2, date, ticketNum);
		}
		
		inline int query_seat(const string<20> &TrainID, const string<5> &date, int num1, int num2){
			train tmp = traintree.at(TrainID);
			return tmp.query_seat(num1, num2, date);
		}
		
		inline train at(const string<20> &TrainID){
			return traintree.at(TrainID);
		}
	};
}

#endif
