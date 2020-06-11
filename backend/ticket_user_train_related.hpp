#ifndef SJTU_TICKET_USER_TRAIN_RELATED_HPP
#define SJTU_TICKET_USER_TRAIN_RELATED_HPP

#include "ticket_user_train.hpp"
#include "train_related.hpp"
#include "vector.hpp"
namespace sjtu{
    class ticket_user_train_related{
    	friend class train_related;
	    private:
		    bptree<string<20>, vector<ticket_user_train> > ticket_user_train_tree;
		    vector<pair<string<20>, ticket_user_train> > queue_list;
	    public:
	        ticket_user_train_related(): ticket_user_train_tree("ticket_user_train_tree", "ticket_user_train_index"), queue_list(){}
	        inline void add_ticket(const string<20> &username, const string<20> &trainID, const string<5> &date, const string<20> &leaving_station, const string<20> &arriving_station, int ticket_num, const string<5> &leaving_time, const string<5> &arriving_time, int date_gap, int date_fix, const string<8> &status, int price, int num1, int num2){
				int success = ticket_user_train_tree.count(username);
				ticket_user_train tmp(trainID, date, leaving_station, arriving_station, ticket_num, leaving_time, arriving_time, date_gap, date_fix, status, price, num1, num2);
				if(success == 0){
					vector<ticket_user_train> TMP;
				    TMP.push_back(tmp);
					ticket_user_train_tree.insert(username, TMP);   
				}     	
				else{
					vector<ticket_user_train> TMP = ticket_user_train_tree.at(username);
					TMP.push_back(tmp);
					ticket_user_train_tree.modify(username, TMP);   
				}
			}
			inline void add_queue(const string<20> &username, const string<20> &trainID, const string<5> &date, const string<20> &leaving_station, const string<20> &arriving_station, int ticket_num, const string<5> &leaving_time, const string<5> &arriving_time, int date_gap, int date_fix, const string<8> &status, int price, int num1, int num2){
				ticket_user_train tmp(trainID, date, leaving_station, arriving_station, ticket_num, leaving_time, arriving_time, date_gap, date_fix,status, price, num1, num2);
				queue_list.push_back(pair<string<20>, ticket_user_train>(username, tmp));
			}
			inline int query_order(const string<20> &username){
				vector<ticket_user_train> TMP = ticket_user_train_tree.at(username);
				if(TMP == vector<ticket_user_train>()) return -1;
				for(int i = 0; i < TMP.size(); ++i){
					std::cout << TMP[i] << std::endl;
				}
				return 0;
			}
			inline int refund_ticket(const string<20> &username, int num, train_related &T){
				vector<ticket_user_train> TMP = ticket_user_train_tree.at(username);
				if(TMP == vector<ticket_user_train>()) return -1;
				else if(num > TMP.size()) return -1;
				else{
					int op = TMP[TMP.size() - num].refund_ticket();
					if(op == 0){
						int i = 0;
						for(i = 0; i < queue_list.size(); ++i){
							if(queue_list[i].first == username  && queue_list[i].second == TMP[TMP.size() - num]){
								queue_list.erase(i);
								break;//处理不了一样的订单 
							}
						}
					}
					else{
						T.refund_ticket(TMP[TMP.size() - num].get_trainID(), TMP[TMP.size() - num].get_startdate(), TMP[TMP.size() - num].get_ticketNum(), TMP[TMP.size() - num].get_num1(), TMP[TMP.size() - num].get_num2());
						vector<int> cmp_train;
						bool flag = false;
						for(int i = 0; i < queue_list.size(); ++i){
							if(TMP[TMP.size() - num].get_trainID() == queue_list[i].second.get_trainID()){
								flag = T.query_num(queue_list[i].second.get_trainID(), queue_list[i].second.get_startdate(), queue_list[i].second.get_ticketNum(), queue_list[i].second.get_num1(), queue_list[i].second.get_num2());
								if(flag){
									queue_list.erase(i);
									flag = false;
									continue;
								}
							}
						}
					}
					return 0; 
				}
			}
    };
}
#endif
