#ifndef SJTU_TICKET_TRAIN_RELATED_HPP
#define SJTU_TICKET_TRAIN_RELATED_HPP

#include "ticket_train.hpp"
#include "vector.hpp"
#include "train_related.hpp"
#include "priority_queue.hpp"
#include "map.hpp"
namespace sjtu{
	class ticket_train_related{
	    friend class train_related;
	private:
	    bptree<station_train, time_cost> ticket_train_tree;
	public:
		struct key{
			string<20> station;
			string<5> timearrive;
			string<5> timeleave;
			int date_gap;
			int date_fix1; 
			int date_fix2; //和发车日期比较 
			int price; 
			string<5> saledate_from;
			string<5> saledate_to;
			int num1; 
			int num2;
		}; 
		struct station_compare{
			bool operator()(const key &key1, const key &key2){
				if(key1.station != key2.station) return key1.station < key2.station;
				else if(key1.timearrive != key2.timearrive) return key1.timearrive < key2.timearrive;
				else if(key1.timeleave != key2.timeleave) return key1.timeleave < key2.timeleave;
				else if(key1.date_gap != key2.date_gap) return key1.date_gap < key2.date_gap;
				else if(key1.date_fix1 != key2.date_fix1) return key1.date_fix1 < key2.date_fix1;
				else if(key1.date_fix2 != key2.date_fix2) return key1.date_fix2 < key2.date_fix2;
			}
		}; 
		ticket_train_related():ticket_train_tree("ticket_train_tree", "ticket_train_index"){}
		void add(const train &tmp, const string<20> &trainID){
			int date_gap = 0;
			int date_fix = 0;
			int date_fix1 = 0;
			int price = 0;
			string<5> arriving_time = tmp.startTime;
			string<5> leaving_time = tmp.startTime;
			for(int i = 0; i < tmp.stationNum; ++i){
				std::cout << i << "WWW" << std::endl;
				add_ticket_train(tmp.stations[i], trainID, tmp.saleDate_from, tmp.saleDate_to, date_gap, date_fix1, leaving_time, arriving_time, i, price);
				if(i != tmp.stationNum - 1){
					pair<int, string<5> > tmpTime = add_time(leaving_time, tmp.travelTimes[i + 1]);
					date_fix += tmpTime.first;
					date_fix1 = date_fix;
					price += tmp.prices[i + 1];
					pair<int, string<5> > tmpTime2 = add_time(leaving_time, tmp.travelTimes[i + 1] + tmp.stopoverTimes[i + 1]);
					date_gap = tmpTime2.first - tmpTime.first;
					date_fix += date_gap;
					arriving_time = tmpTime.second;
					leaving_time = tmpTime2.second;
					}
				}
		}
		void add_ticket_train(const string<20> &Station, const string<20> &TrainID, const string<5> &SaleDate_from, const string<5> &SaleDate_to, int Date_gap, int Date_fix, const string<5> &Leaving_time, const string<5> &Arriving_time, int Station_num, int Price){
			ticket_train_tree.insert(station_train(Station, TrainID), time_cost(SaleDate_from, SaleDate_to, Date_gap, Date_fix, Arriving_time, Leaving_time, Station_num, Price)); 
		}
		priority_queue<ticket_train, cmp_time> query_ticket_time(const string<20> &station1, const string<20> &station2, const string<5> &date){
			priority_queue<ticket_train, cmp_time> result;
			char a[2];
			a[0] = 0x80;
			string<20> tmpID = string<20>(a);
			auto iter1 = ticket_train_tree.lower_bound(station_train(station1, tmpID));
			auto iter2 = ticket_train_tree.lower_bound(station_train(station2, tmpID));
			if(iter1 == ticket_train_tree.end() || iter2 == ticket_train_tree.end()) return result;	
	        string<20> tmpstation1 = iter1.get_key().get_station();
	        string<20> tmpstation2 = iter2.get_key().get_station();
	        string<20> tmpID1 = iter1.get_key().get_trainID();
	        string<20> tmpID2 = iter2.get_key().get_trainID();
			while(tmpstation1 == station1 && tmpstation2 == station2){
				if(tmpID1 < tmpID2){
					++iter1;
					if(iter1 == ticket_train_tree.end()) return result;	
					tmpID1 = iter1.get_key().get_trainID();
					tmpstation1 = iter1.get_key().get_station();
				}
				else if(tmpID2 < tmpID1){
					++iter2;
					if(iter2 == ticket_train_tree.end()) return result;
					tmpID2 = iter2.get_key().get_trainID();
					tmpstation2 = iter2.get_key().get_station();
				}
				else{
					if(iter1->station_num >= iter2->station_num){
						++iter1;
					    ++iter2;
					    if(iter1 == ticket_train_tree.end() || iter2 == ticket_train_tree.end()) return result;
					    tmpID1 = iter1.get_key().get_trainID();
					    tmpstation1 = iter1.get_key().get_station();
					    tmpID2 = iter2.get_key().get_trainID();
					    tmpstation2 = iter2.get_key().get_station();
					    continue;
					}
					else{
						time_cost TMP1 = *(iter1);
						time_cost TMP2 = *(iter2);
						string<5> tmpdate = minus_date(date, TMP1.date_fix + TMP1.date_gap);
						string<5> tmpdate1 = TMP1.saleDate_from;
						string<5> tmpdate2 = TMP2.saleDate_to;
						if(tmpdate1 <= tmpdate && tmpdate2 >= tmpdate){
							string<5> arrivingtime = TMP2.arriving_time;
							result.push(ticket_train(iter1.get_key().get_trainID(), station1, station2, tmpdate, date, add_date(date, TMP2.date_fix - TMP1.date_fix - TMP1.date_gap), TMP1.leaving_time, arrivingtime, TMP2.price - TMP1.price, TMP1.station_num, TMP2.station_num));
						}
						++iter1;
						++iter2;
						if(iter1 == ticket_train_tree.end() || iter2 == ticket_train_tree.end()) return result;
						tmpID1 = iter1.get_key().get_trainID();
					    tmpstation1 = iter1.get_key().get_station();
					    tmpID2 = iter2.get_key().get_trainID();
					    tmpstation2 = iter2.get_key().get_station();
					}
				}
			}
			return result;
		}
		priority_queue<ticket_train, cmp_price> query_ticket_price(const string<20> &station1, const string<20> &station2, const string<5> &date){
			priority_queue<ticket_train, cmp_price> result;
			char a[2];
			a[0] = 0x80;
			string<20> tmpID = string<20>(a);
			auto iter1 = ticket_train_tree.lower_bound(station_train(station1, tmpID));
			auto iter2 = ticket_train_tree.lower_bound(station_train(station2, tmpID));	
			if(iter1 == ticket_train_tree.end() || iter2 == ticket_train_tree.end()) return result;	
	        string<20> tmpstation1 = iter1.get_key().get_station();
	        string<20> tmpstation2 = iter2.get_key().get_station();
	        string<20> tmpID1 = iter1.get_key().get_trainID();
	        string<20> tmpID2 = iter2.get_key().get_trainID();
			while(tmpstation1 == station1 && tmpstation2 == station2){
				if(tmpID1 < tmpID2){
					++iter1;
					if(iter1 == ticket_train_tree.end()) return result;	
					tmpID1 = iter1.get_key().get_trainID();
					tmpstation1 = iter1.get_key().get_station();
				}
				else if(tmpID2 < tmpID1){
					++iter2;
					if(iter2 == ticket_train_tree.end()) return result;	
					tmpID2 = iter2.get_key().get_trainID();
					tmpstation2 = iter2.get_key().get_station();
				}
				else{
					if(iter1->station_num >= iter2->station_num){
						++iter1;
					    ++iter2;
					    if(iter1 == ticket_train_tree.end() || iter2 == ticket_train_tree.end()) return result;	
					    tmpID1 = iter1.get_key().get_trainID();
					    tmpstation1 = iter1.get_key().get_station();
					    tmpID2 = iter2.get_key().get_trainID();
					    tmpstation2 = iter2.get_key().get_station();
					    continue;
					}
					else{
						time_cost TMP1 = *(iter1);
						time_cost TMP2 = *(iter2);
						string<5> tmpdate = minus_date(date, TMP1.date_fix + TMP1.date_gap);
						string<5> tmpdate1 = TMP1.saleDate_from;
						string<5> tmpdate2 = TMP2.saleDate_to;
						if(tmpdate1 <= tmpdate && tmpdate2 >= tmpdate){
							string<5> arrivingtime = iter2->arriving_time;
							result.push(ticket_train(iter1.get_key().get_trainID(), station1, station2, tmpdate, date, add_date(date, TMP2.date_fix - TMP1.date_fix - TMP1.date_gap), TMP1.leaving_time, arrivingtime, TMP2.price - TMP1.price, TMP1.station_num, TMP2.station_num));
						}
						++iter1;
						++iter2;
						if(iter1 == ticket_train_tree.end() || iter2 == ticket_train_tree.end()) return result;	
						tmpID1 = iter1.get_key().get_trainID();
					    tmpstation1 = iter1.get_key().get_station();
					    tmpID2 = iter2.get_key().get_trainID();
					    tmpstation2 = iter2.get_key().get_station();
					}
				}
			}
			return result;
		}
		pair<ticket_train, ticket_train> query_transfer_time(const string<20> &station1, const string<20> &station2, const string<5> &date, train_related &T){
			map<key, string<20>, station_compare> set1;
			map<key, string<20>, station_compare> set2;
			pair<ticket_train, ticket_train> result;
			char a[2];
			a[0] = 0x80;
			string<20> tmpID = string<20>(a);
			auto iter1 = ticket_train_tree.lower_bound(station_train(station1, tmpID));
			auto iter2 = ticket_train_tree.lower_bound(station_train(station2, tmpID));	
			if(iter1 == ticket_train_tree.end() || iter2 == ticket_train_tree.end()) return result;	
			while(iter1.get_key().get_station() == station1){
				string<20> TrainID = iter1.get_key().get_trainID();
				train tmp = T.at(TrainID);
				if(!tmp.isrelease()){
					++iter1;
					if(iter1 == ticket_train_tree.end()) break;	
					continue;
				}
				else{
					bool flag = false;
					int date_gap = 0;
			        int date_fix = 0;
			        int date_fix1 = 0;
			        int date_fix2 = 0;
			        int price1 = 0;
			        int num;
			        string<5> arriving_time = tmp.startTime;
			        string<5> leaving_time = tmp.startTime;
			        int price = 0;
			        string<5> leaving_time1;
			        for(int i = 0; i < tmp.stationNum; ++i){
			        	if(flag){
			        		set1.insert(pair<key, string<20> >(key{tmp.stations[i], arriving_time, leaving_time1, date_gap, date_fix2 - date_fix1, date_fix2, price - price1, tmp.saleDate_from, tmp.saleDate_to, num, i}, TrainID));
						}
						if(station1 == tmp.stations[i]){
							flag = true;
							date_fix1 = date_fix2 + date_gap;
							price1 = price;
							num = i;
							leaving_time1 = leaving_time;
						}
						if(i == tmp.stationNum - 1) break;
			            price += tmp.prices[i + 1];
			            pair<int, string<5> > tmpTime = add_time(leaving_time, tmp.travelTimes[i + 1]);
				        date_fix += tmpTime.first;
				        date_fix2 = date_fix;
				        pair<int, string<5> > tmpTime2 = add_time(leaving_time, tmp.travelTimes[i + 1] + tmp.stopoverTimes[i + 1]);
				        date_gap = tmpTime2.first - tmpTime.first;
				        date_fix += date_gap;
				        arriving_time = tmpTime.second;
				        leaving_time = tmpTime2.second;
			        }
			        ++iter1;
			        if(iter1 == ticket_train_tree.end()) break;	
				}
			}
			while(iter2.get_key().get_station() == station2){
				string<20> TrainID = iter2.get_key().get_trainID();
				train tmp = T.at(TrainID);
				if(!tmp.isrelease()){
					++iter2;
					if(iter2 == ticket_train_tree.end()) break;	
					continue;
				}
				else{
					int date_gap = 0;
					int date_gap1;
			        int date_fix = 0;
			        int date_fix1 = 0;
			        int date_fix2 = 0;
			        int price1 = 0;
			        int num;
			        string<5> arriving_time = tmp.startTime;
			        string<5> leaving_time = tmp.startTime;
			        string<5> arriving_time1;
			        int price = 0;
			        for(int i = 0; i < tmp.stationNum; ++i){
						if(station2 == tmp.stations[i]){
							date_fix1 = date_fix2;
							price1 = price;
							num = i;
							arriving_time1 = arriving_time;
							date_gap1 = date_gap;
							break;
						}
			            price += tmp.prices[i + 1];
			            pair<int, string<5> > tmpTime = add_time(leaving_time, tmp.travelTimes[i + 1]);
				        date_fix += tmpTime.first;
				        date_fix2 = date_fix;
				        pair<int, string<5> > tmpTime2 = add_time(leaving_time, tmp.travelTimes[i + 1] + tmp.stopoverTimes[i + 1]);
				        date_gap = tmpTime2.first - tmpTime.first;
				        date_fix += date_gap;
				        arriving_time = tmpTime.second;
				        leaving_time = tmpTime2.second;
			        }
			        date_gap = 0;
			        date_fix = 0;
			        date_fix2 = 0;
			        arriving_time = tmp.startTime;
			        leaving_time = tmp.startTime;
			        price = 0;
			        for(int i = 0; i < tmp.stationNum; ++i){
						if(station2 == tmp.stations[i]){
							break;
						}
						set2.insert(pair<key, string<20> >(key{tmp.stations[i], arriving_time1, leaving_time, date_gap, date_fix1 - date_fix2 - date_gap, date_fix2, price1 - price, tmp.saleDate_from, tmp.saleDate_to, i, num}, TrainID));
			            price += tmp.prices[i + 1];
			            pair<int, string<5> > tmpTime = add_time(leaving_time, tmp.travelTimes[i + 1]);
				        date_fix += tmpTime.first;
				        date_fix2 = date_fix;
				        pair<int, string<5> > tmpTime2 = add_time(leaving_time, tmp.travelTimes[i + 1] + tmp.stopoverTimes[i + 1]);
				        date_gap = tmpTime2.first - tmpTime.first;
				        date_fix += date_gap;
				        arriving_time = tmpTime.second;
				        leaving_time = tmpTime2.second;
			        }
			        ++iter2;
			        if(iter2 == ticket_train_tree.end()) break;	
				}
			}
			auto it1 = set1.begin();
			auto it2 = set2.begin();
			int min = 100000;
			ticket_train ans1;
			ticket_train ans2;
			for(; it1 != set1.end(); ++it1){
				for(it2 = set2.begin(); it2 != set2.end(); ++it2){
					if(it1->first.station != it2->first.station){
						continue;
					}
					if(minus_date(date, it1->first.date_fix2 - it1->first.date_fix1) < it1->first.saledate_from || minus_date(date, it1->first.date_fix2 - it1->first.date_fix1) > it1->first.saledate_to) continue;
					if(add_date(date, it1->first.date_fix1) < add_date(it2->first.saledate_to, it2->first.date_fix2) || (add_date(date, it1->first.date_fix1) == add_date(it2->first.saledate_to, it2->first.date_fix2) && it1->first.timearrive <= it2->first.timeleave)){
						string<5> tmpdate;
						if(add_date(date, it1->first.date_fix1) < add_date(it2->first.saledate_from, it2->first.date_fix2 + it2->first.date_gap)) tmpdate = add_date(it2->first.saledate_from, it2->first.date_fix2 + it2->first.date_gap);
						else{
							tmpdate = add_date(date, it1->first.date_fix1);
							if(it1->first.timearrive > it2->first.timeleave) tmpdate = add_date(tmpdate, 1);
						}
						if(minus_time(it1->first.timeleave, it2->first.timearrive) + 1440 * minus_date(date, add_date(tmpdate, it2->first.date_fix1)) < min || minus_time(it1->first.timeleave, it2->first.timearrive) + 1440 * minus_date(date, add_date(tmpdate, it2->first.date_fix1)) == min && (minus_time(it1->first.timeleave, it1->first.timearrive) + 1440 * it1->first.date_fix1) < (minus_time(ans1.leaving_time, ans1.arriving_time) + 1440 * minus_date(ans1.leaving_date, ans1.arriving_date))){
						   min = minus_time(it1->first.timeleave, it2->first.timearrive) + 1440 * minus_date(date, add_date(tmpdate, it2->first.date_fix1));
						   ans1 = ticket_train(it1->second, station1, it1->first.station, minus_date(date, it1->first.date_fix2 - it1->first.date_fix1), date, add_date(date, it1->first.date_fix1), it1->first.timeleave, it1->first.timearrive, it1->first.price, it1->first.num1, it1->first.num2);
						   ans2 = ticket_train(it2->second, it2->first.station, station2, minus_date(tmpdate, it2->first.date_fix2 + it2->first.date_gap), tmpdate, add_date(tmpdate, it2->first.date_fix1), it2->first.timeleave, it2->first.timearrive, it2->first.price, it2->first.num1, it2->first.num2);
						}
					}
				}
			}
			result = pair<ticket_train, ticket_train>(ans1, ans2);
			return result;
		}
		pair<ticket_train, ticket_train> query_transfer_price(const string<20> &station1, const string<20> &station2, const string<5> &date, train_related &T){

			map<key, string<20>, station_compare> set1;
			map<key, string<20>, station_compare> set2;
			pair<ticket_train, ticket_train> result;
			char a[2];
			a[0] = 0x80;
			string<20> tmpID = string<20>(a);
			auto iter1 = ticket_train_tree.lower_bound(station_train(station1, tmpID));
			auto iter2 = ticket_train_tree.lower_bound(station_train(station2, tmpID));	
			if(iter1 == ticket_train_tree.end() || iter2 == ticket_train_tree.end()) return result;	
			while(iter1.get_key().get_station() == station1){
				string<20> TrainID = iter1.get_key().get_trainID();
				train tmp = T.at(TrainID);
				if(!tmp.isrelease()){
					++iter1;
					if(iter1 == ticket_train_tree.end()) break;	
					continue;
				}
				else{
					bool flag = false;
					int date_gap = 0;
			        int date_fix = 0;
			        int date_fix1 = 0;
			        int date_fix2 = 0;
			        int price1 = 0;
			        int num;
			        string<5> arriving_time = tmp.startTime;
			        string<5> leaving_time = tmp.startTime;
			        int price = 0;
			        string<5> leaving_time1;
			        for(int i = 0; i < tmp.stationNum; ++i){
			        	if(flag){
			        		set1.insert(pair<key, string<20> >(key{tmp.stations[i], arriving_time, leaving_time1, date_gap, date_fix2 - date_fix1, date_fix2, price - price1, tmp.saleDate_from, tmp.saleDate_to, num, i}, TrainID));
						}
						if(station1 == tmp.stations[i]){
							flag = true;
							date_fix1 = date_fix2 + date_gap;
							price1 = price;
							num = i;
							leaving_time1 = leaving_time;
						}
						if(i == tmp.stationNum - 1) break;
			            price += tmp.prices[i + 1];
			            pair<int, string<5> > tmpTime = add_time(leaving_time, tmp.travelTimes[i + 1]);
				        date_fix += tmpTime.first;
				        date_fix2 = date_fix;
				        pair<int, string<5> > tmpTime2 = add_time(leaving_time, tmp.travelTimes[i + 1] + tmp.stopoverTimes[i + 1]);
				        date_gap = tmpTime2.first - tmpTime.first;
				        date_fix += date_gap;
				        arriving_time = tmpTime.second;
				        leaving_time = tmpTime2.second;
			        }
			        ++iter1;
			        if(iter1 == ticket_train_tree.end()) break;	
				}
			}
			while(iter2.get_key().get_station() == station2){
				string<20> TrainID = iter2.get_key().get_trainID();
				train tmp = T.at(TrainID);
				if(!tmp.isrelease()){
					++iter2;
					if(iter2 == ticket_train_tree.end()) break;	
					continue;
				}
				else{
					int date_gap = 0;
					int date_gap1;
			        int date_fix = 0;
			        int date_fix1 = 0;
			        int date_fix2 = 0;
			        int price1 = 0;
			        int num;
			        string<5> arriving_time = tmp.startTime;
			        string<5> leaving_time = tmp.startTime;
			        string<5> arriving_time1;
			        int price = 0;
			        for(int i = 0; i < tmp.stationNum; ++i){
						if(station2 == tmp.stations[i]){
							date_fix1 = date_fix2;
							price1 = price;
							num = i;
							arriving_time1 = arriving_time;
							date_gap1 = date_gap;
							break;
						}
			            price += tmp.prices[i + 1];
			            pair<int, string<5> > tmpTime = add_time(leaving_time, tmp.travelTimes[i + 1]);
				        date_fix += tmpTime.first;
				        date_fix2 = date_fix;
				        pair<int, string<5> > tmpTime2 = add_time(leaving_time, tmp.travelTimes[i + 1] + tmp.stopoverTimes[i + 1]);
				        date_gap = tmpTime2.first - tmpTime.first;
				        date_fix += date_gap;
				        arriving_time = tmpTime.second;
				        leaving_time = tmpTime2.second;
			        }
			        date_gap = 0;
			        date_fix = 0;
			        date_fix2 = 0;
			        arriving_time = tmp.startTime;
			        leaving_time = tmp.startTime;
			        price = 0;
			        for(int i = 0; i < tmp.stationNum; ++i){
						if(station2 == tmp.stations[i]){
							break;
						}
						set2.insert(pair<key, string<20> >(key{tmp.stations[i], arriving_time1, leaving_time, date_gap, date_fix1 - date_fix2 - date_gap, date_fix2, price1 - price, tmp.saleDate_from, tmp.saleDate_to, i, num}, TrainID));
			            price += tmp.prices[i + 1];
			            pair<int, string<5> > tmpTime = add_time(leaving_time, tmp.travelTimes[i + 1]);
				        date_fix += tmpTime.first;
				        date_fix2 = date_fix;
				        pair<int, string<5> > tmpTime2 = add_time(leaving_time, tmp.travelTimes[i + 1] + tmp.stopoverTimes[i + 1]);
				        date_gap = tmpTime2.first - tmpTime.first;
				        date_fix += date_gap;
				        arriving_time = tmpTime.second;
				        leaving_time = tmpTime2.second;
			        }
			        ++iter2;
			        if(iter2 == ticket_train_tree.end()) break;	
				}
			}
		    auto it1 = set1.begin();
			auto it2 = set2.begin();
			int min = 100000;
			ticket_train ans1;
			ticket_train ans2;
			for(; it1 != set1.end(); ++it1){
				for(it2 = set2.begin(); it2 != set2.end(); ++it2){
					if(it1->first.station != it2->first.station){
						continue;
					}
					if(it1->second == it2->second){
						continue;
					}
					if(minus_date(date, it1->first.date_fix2 - it1->first.date_fix1) < it1->first.saledate_from || minus_date(date, it1->first.date_fix2 - it1->first.date_fix1) > it1->first.saledate_to) continue;
					if(add_date(date, it1->first.date_fix1) < add_date(it2->first.saledate_to, it2->first.date_fix2 + it2->first.date_gap) || (add_date(date, it1->first.date_fix1) == add_date(it2->first.saledate_to, it2->first.date_fix2 + it2->first.date_gap) && it1->first.timearrive <= it2->first.timeleave)){
						if(it1->first.price + it2->first.price < min || (it1->first.price + it2->first.price == min) && (minus_time(it1->first.timeleave, it1->first.timearrive) + 1440 * it1->first.date_fix1) < (minus_time(ans1.leaving_time, ans1.arriving_time) + 1440 * minus_date(ans1.leaving_date, ans1.arriving_date))){
							min = it1->first.price + it2->first.price;
							string<5> tmpdate;
							if(add_date(date, it1->first.date_fix1) < add_date(it2->first.saledate_from, it2->first.date_fix2 +it2->first.date_gap)) tmpdate = add_date(it2->first.saledate_from, it2->first.date_fix2);
							else{
								tmpdate = add_date(date, it1->first.date_fix1);
								if(it1->first.timearrive > it2->first.timeleave) tmpdate = add_date(tmpdate, 1);
							}
							ans1 = ticket_train(it1->second, station1, it1->first.station, minus_date(date, it1->first.date_fix2 - it1->first.date_fix1), date, add_date(date, it1->first.date_fix1), it1->first.timeleave, it1->first.timearrive, it1->first.price, it1->first.num1, it1->first.num2);
						    ans2 = ticket_train(it2->second, it2->first.station, station2, minus_date(tmpdate, it2->first.date_fix2 + it2->first.date_gap), tmpdate, add_date(tmpdate, it2->first.date_fix1), it2->first.timeleave, it2->first.timearrive, it2->first.price, it2->first.num1, it2->first.num2);
						}
					}
				}
			}
			pair<ticket_train, ticket_train> ans(ans1, ans2);
			return ans;
		}
    };
}

#endif
