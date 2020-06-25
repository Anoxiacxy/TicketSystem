#ifndef SJTU_TICKET_USER_TRAIN_RELATED_HPP
#define SJTU_TICKET_USER_TRAIN_RELATED_HPP

#include "ticket_user_train.hpp"
#include "train_related.hpp"
#include "vector.hpp"
namespace sjtu{
	char queuelist[] = "queuelist";
    class ticket_user_train_related{
    	friend class train_related;
	    private:
		    bptree<user_order, ticket_user_train> ticket_user_train_tree;
			bptree<string<20>,int >order_num;
		    SJTU::file_vector<pair<queue_index, queue_value>, queuelist> queue_list;
	    public:
	        ticket_user_train_related(): ticket_user_train_tree("ticket_user_train_tree", "ticket_user_train_index"), order_num("order_num_tree", "order_num_index"), queue_list(){
			}
	        ~ticket_user_train_related(){
			}
			inline int add_ticket(const string<20> &username, const string<20> &trainID, const string<5> &date, const string<20> &leaving_station, const string<20> &arriving_station, int ticket_num, const string<5> &leaving_time, const string<5> &arriving_time, int date_gap, int date_fix, const string<8> &status, int price, int num1, int num2){
				int success1 = order_num.count(username);
				int data = 0;
				if(success1 == 0){
					order_num.insert(username, 1);
				}
				else{
					int tmp = order_num.at(username);
					data = tmp;
					++tmp;
					order_num.modify(username, tmp);
				}
				ticket_user_train tmp(trainID, date, leaving_station, arriving_station, ticket_num, leaving_time, arriving_time, date_gap, date_fix, status, price, num1, num2);
				ticket_user_train_tree.insert(user_order(data, username), tmp);
				return data;
			}
			inline void add_queue(const string<20> &username, int data, const string<20> &trainID, const string<5> &date, const string<20> &leaving_station, const string<20> &arriving_station, int ticket_num, const string<5> &leaving_time, const string<5> &arriving_time, int date_gap, int date_fix, const string<8> &status, int price, int num1, int num2){
				queue_list.push_back(pair<queue_index, queue_value>(queue_index(trainID, minus_date(date, date_fix), data), queue_value(num1, num2, ticket_num, username)));
			}
			inline int query_order(const string<20> &username){
				int TMP1 = order_num.at(username);
				if(TMP1 == 0){
					std::cout << 0 << std::endl;
				}
				else{
					std::cout << TMP1 << std::endl;
					auto TMP = ticket_user_train_tree.lower_bound(user_order(TMP1 - 1, username));
				    for(int i = 0; i < TMP1; ++i){
				    	ticket_user_train A = *(TMP);
				    	A.print();
					    --TMP;
				    }
				}
			}
			inline int query_order(const string<20> &username, std::ostream &out){
				int TMP1 = order_num.at(username);
				if(TMP1 == 0){
					out << 0 << std::endl;
				}
				else{
					out << TMP1 << std::endl;
					auto TMP = ticket_user_train_tree.lower_bound(user_order(TMP1 - 1, username));
				    for(int i = 0; i < TMP1; ++i){
				    	ticket_user_train A = *(TMP);
				    	A.print(out);
					    --TMP;
				    }
				}
			}
			inline int refund_ticket(const string<20> &username, int num, train_related &T){
				int TMP = order_num.at(username);
				if(num > TMP) return -1;
				else{
					ticket_user_train tmp = ticket_user_train_tree.at(user_order(TMP - num, username));
					int op = tmp.refund_ticket();
					if(op == 0){
						ticket_user_train_tree.modify(user_order(TMP - num, username), tmp);
						int i = 0;
						for(i = 0; i < queue_list.size(); ++i){
							if(queue_list[i].first.order_num == TMP - num && queue_list[i].second.username == username){
								queue_list.erase(i);
								break;
							}
						}
					}
					else if(op == 1){
						ticket_user_train_tree.modify(user_order(TMP - num, username), tmp);
						T.refund_ticket(tmp.get_trainID(), tmp.get_startdate(), tmp.get_ticketNum(), tmp.get_num1(), tmp.get_num2());
						bool flag = false;
						for(int i = 0; i < queue_list.size(); ++i){
							if(tmp.get_trainID() == queue_list[i].first.station && queue_list[i].first.startdate == tmp.get_startdate()){
								flag = T.query_num(queue_list[i].first.station, queue_list[i].first.startdate, queue_list[i].second.num3, queue_list[i].second.num1, queue_list[i].second.num2);
								if(flag){
								    ticket_user_train ttmp = ticket_user_train_tree.at(user_order(queue_list[i].first.order_num, queue_list[i].second.username));
									ttmp.success();
									ticket_user_train_tree.modify(user_order(queue_list[i].first.order_num, queue_list[i].second.username), ttmp);
									queue_list.erase(i);
									flag = false;
									--i;
									continue;
								}
							}
						}
					}
					else return -1;
					return 0; 
				}
			}
    };
}
#endif
