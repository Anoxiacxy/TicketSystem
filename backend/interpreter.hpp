#pragma once

#include "user_related.hpp"
#include "train_related.hpp"
#include "ticket_train_related.hpp"
#include "ticket_user_train_related.hpp"
#include <iostream> 
#include <sstream>
#include <cstring>
#include <fstream> 

namespace sjtu{
class Interpreter{
    /*
        take buff as user's command
        and replace it with the result from back end
    */
    private:
    	user_related U;
    	train_related T;
    	ticket_user_train_related TUT;
    	ticket_train_related TT;
    public:
    Interpreter(){}
    void operator() (char *buff) {
    	std::ofstream outfile("data.txt");
    	std::ifstream infile;
    	outfile << buff;
    	outfile.close();
    	infile.open("data.txt");
    	string<15>tmp;
		std::stringstream sout;
    	while(infile >> tmp){
    	    if(tmp == string<10>("add_user")){
    	    	string<20> username1;
    	    	string<20> username2;
    	    	string<30> password;
    	    	string<10> name;
    	    	string<30> mailAddr;
    	    	int privilege;
    	    	while(infile.get() != 13 && !infile.eof()){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-c")){
    	    			infile >> username1;
					}
					else if(op == string<3>("-u")){
    	    			infile >> username2;
					}
					else if(op == string<3>("-p")){
    	    			infile >> password;
					}
					else if(op == string<3>("-n")){
    	    			infile >> name;
					}
					else if(op == string<3>("-m")){
    	    			infile >> mailAddr;
					}
					else if(op == string<3>("-g")){
    	    			infile >> privilege;
					}
				}
				if(username1 == string<20>("cur")){
					sout << U.add_user(username2, password, name, mailAddr, privilege) << std::endl;
				}
				else{
					sout << U.add_user(username1, username2, password, name, mailAddr, privilege) << std::endl;
				}
		    } 
		    else if(tmp == string<10>("login")){
		    	string<20> username;
    	    	string<30> password;
    	    	while(infile.get() != 13 && !infile.eof()){
    	    		string<3> op;
    	    		infile >> op;
					if(op == string<3>("-u")){
    	    			infile >> username;
					}
					else if(op == string<3>("-p")){
    	    			infile >> password;
					}
				}
				sout << U.login(username, password) << std::endl;
		    }
		    else if(tmp == string<10>("logout")){
		    	string<20> username;
    	    	while(infile.get() != 13 && !infile.eof()){
    	    		string<3> op;
    	    		infile >> op;
					if(op == string<3>("-u")){
    	    			infile >> username;
					}
				}
				sout << U.logout(username) << std::endl;
		    }
		    else if(tmp == string<15>("query_profile")){
		    	string<20> username1;
    	    	string<20> username2;
    	    	while(infile.get() != 13 && !infile.eof()){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-c")){
    	    			infile >> username1;
					}
					else if(op == string<3>("-u")){
    	    			infile >> username2;
					}
				}
				pair<bool, pair<USERNAME, user> > Tmp = U.query_profile(username1, username2);
				if(Tmp.first == false) sout << -1 << std::endl;
				else{
					sout << username2 << " " << Tmp.second.second << std::endl;
				}
		    }
		    else if(tmp == string<15>("modify_profile")){
		    	string<20> username1;
    	    	string<20> username2;
    	    	string<30> password = invalid_password;
    	    	string<10> name = invalid_name;
    	    	string<30> mailAddr = invalid_mailAddr;
    	    	int privilege = invalid_privilege;
    	    	while(infile.get() != 13 && !infile.eof()){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-c")){
    	    			infile >> username1;
					}
					else if(op == string<3>("-u")){
    	    			infile >> username2;
					}
					else if(op == string<3>("-p")){
    	    			infile >> password;
					}
					else if(op == string<3>("-n")){
    	    			infile >> name;
					}
					else if(op == string<3>("-m")){
    	    			infile >> mailAddr;
					}
					else if(op == string<3>("-g")){
    	    			infile >> privilege;
					}
				}
				pair<bool, pair<USERNAME, user> > Tmp = U.modify_profile(username1, username2, password, name, mailAddr, privilege);
				if(Tmp.first == false) sout << -1 << std::endl;
				else{
					sout << username2 << " " << Tmp.second.second << std::endl;
				}
		    }
		    else if(tmp == string<10>("add_train")){
		    	string<20> trainID;
		    	int stationNum;
		    	int seatNum;
		    	while(infile.get() != 13 && !infile.eof()){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-i")){
    	    			infile >> trainID;
					}
					else if(op == string<3>("-n")){
    	    			infile >> stationNum;
					}
					else if(op == string<3>("-m")){
    	    			infile >> seatNum;
    	    			break;
					}
				}
		    	string<20> stations[stationNum];
		    	int prices[stationNum];
		    	string<5> startTime;
		    	int travelTimes[stationNum];
		    	int stopoverTimes[stationNum];
		    	string<5> saleDate_from;
		    	string<5> saleDate_to;
		    	char type;
		    	while(infile.get() != 13 && !infile.eof()){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-s")){
    	    			infile.get();
    	    			for(int i = 0; i < stationNum - 1; ++i){
    	    				char Tmp[33];
    	    				infile.getline(Tmp, 33, '|');
    	    				stations[i] = string<20>(Tmp);
						}
						infile >> stations[stationNum - 1];
					}
					else if(op == string<3>("-p")){
						prices[0] = -1;
						for(int i = 1; i < stationNum - 1; ++i){
							infile >> prices[i];
							infile.get();
						}
						infile >> prices[stationNum - 1];
					}
					else if(op == string<3>("-x")){
						infile >> startTime;
					}
					else if(op == string<3>("-t")){
						travelTimes[0] = -1;
						for(int i = 1; i < stationNum - 1; ++i){
							infile >> travelTimes[i];
							infile.get();
						}
						infile >> travelTimes[stationNum - 1];
					}
					else if(op == string<3>("-o")){
						if(stationNum == 2) infile.get();
						stopoverTimes[0] = -1;
						for(int i = 1; i < stationNum - 1; ++i){
							infile >> stopoverTimes[i];
							if(i != stationNum - 2) infile.get();
						}
						stopoverTimes[stationNum - 1] = -1;
					}
					else if(op == string<3>("-d")){
						char Tmp[6];
						infile.get();
    	    			infile.getline(Tmp, 6, '|');
						saleDate_from = string<5>(Tmp);
						infile >> saleDate_to;
					}
					else if(op == string<3>("-y")){
						infile.get();
						infile >> type;
					}
				}
				int tmp = T.add_train(trainID, stationNum, stations, prices, startTime, travelTimes, stopoverTimes, saleDate_from, saleDate_to, type, seatNum);
				sout << tmp << std::endl;
		    } 
		    else if(tmp == string<15>("release_train")){
		    	string<20> trainID;
		    	while(infile.get() != 13 && !infile.eof()){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-i")){
    	    			infile >> trainID;
					}
				}
				int data = T.release_train(trainID);
				if(data == 0){
					train tmp = T.at(trainID);
					TT.add(tmp, trainID, T);
				}
				sout << data << std::endl;
		    }
		    else if(tmp == string<15>("query_train")){
		    	string<20> trainID;
		    	string<5> date;
		    	while(infile.get() != 13 && !infile.eof()){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-i")){
    	    			infile >> trainID;
					}
					else if(op == string<3>("-d")){
						infile >> date;
					}
				}
				pair<bool, pair<string<20>, train> > Tmp = T.query_train(trainID);
				if(Tmp.first == false) sout << -1 << std::endl;
				else{
					if(cmp_date(Tmp.second.second.get_saleDate_from(), date) == 1 || cmp_date(Tmp.second.second.get_saleDate_to(), date) == -1) sout << -1 << std::endl;
					else{
						sout << trainID << " ";
		                sout << Tmp.second.second.type << std::endl;
			            int date_gap = 0;
			            int date_fix = 0;
			            string<5> arriving_time = Tmp.second.second.startTime;
			            string<5> leaving_time = Tmp.second.second.startTime;
			            string<5> arriving_date = date;
			            string<5> leaving_date = date;
			            int price = 0;
			            int date_interval = minus_date(Tmp.second.second.saleDate_from, date);
			            sout << T.Station1[Tmp.second.second.pos1] << " xx-xx" << " xx:xx" << " -> " << date << " " << leaving_time << " " << price << " " << T.Seat1[Tmp.second.second.pos2 + date_interval * (Tmp.second.second.stationNum - 1)] << std::endl; 
			            price += T.Price1[Tmp.second.second.pos1 + 1];
			            pair<int, string<5> > tmpTime1 = add_time(leaving_time, T.Time1[Tmp.second.second.pos1 + 1]);
			            date_fix += tmpTime1.first;
			            pair<int, string<5> > tmpTime3 = add_time(leaving_time, T.Time1[Tmp.second.second.pos1 + 1] + T.Time2[Tmp.second.second.pos1 + 1]);
			            date_gap = tmpTime3.first - tmpTime1.first;
			            arriving_date = add_date(leaving_date, tmpTime1.first);
			            leaving_date = add_date(arriving_date, date_gap);
			            arriving_time = tmpTime1.second;
			            leaving_time = tmpTime3.second;
			            for(int i = 1; i < Tmp.second.second.stationNum - 1; ++i){
				            sout << T.Station1[Tmp.second.second.pos1 + i] << " " << arriving_date << " " << arriving_time << " -> " << leaving_date << " " << leaving_time << " " << price << " " << T.Seat1[Tmp.second.second.pos2 + date_interval * (Tmp.second.second.stationNum - 1) + i] << std::endl;
			                price += T.Price1[Tmp.second.second.pos1 + i + 1];
			                pair<int, string<5> > tmpTime = add_time(leaving_time, T.Time1[Tmp.second.second.pos1 + i + 1]);
				            date_fix += tmpTime.first;
				            pair<int, string<5> > tmpTime2 = add_time(leaving_time, T.Time1[Tmp.second.second.pos1 + i + 1] + T.Time2[Tmp.second.second.pos1 + i + 1]);
				            date_gap = tmpTime2.first - tmpTime.first;
				            arriving_date = add_date(leaving_date, tmpTime.first);
				            leaving_date = add_date(arriving_date, date_gap);
				            arriving_time = tmpTime.second;
				            leaving_time = tmpTime2.second;
			            }
			            sout << T.Station1[Tmp.second.second.pos1 + Tmp.second.second.stationNum - 1] << " " << arriving_date << " " << arriving_time << " -> xx-xx" << " xx:xx " << price <<  " x\n";
					}
				}
		    }
		    else if(tmp == string<15>("delete_train")){
		    	string<20> trainID;
		    	while(infile.get() != 13 && !infile.eof()){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-i")){
    	    			infile >> trainID;
					}
				}
				sout << T.delete_train(trainID) << std::endl;
		    }
		    else if(tmp == string<15>("query_ticket")){
		    	string<5> date;
		    	string<20> station1;
		    	string<20> station2;
		    	string<5> cmp;
		    	while(infile.get() != 13 && !infile.eof()){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-d")){
    	    			infile >> date;
					}
					else if(op == string<3>("-s")){
						infile >> station1;
					}
					else if(op == string<3>("-t")){
						infile >> station2;
					} 
					else if(op == string<3>("-p")){
						infile >> cmp;
					}
				}
				if(cmp == string<5>("time")){
					priority_queue<ticket_train, cmp_time> res = TT.query_ticket_time(station1, station2, date);
					int num = res.size();
					sout << num << std::endl;
					for(int i = 0; i < num; ++i){
						ticket_train ans = res.top();
						res.pop();
						bool flag = T.release(ans.get_trainID());
						if(flag){
							ans.print(sout);
						    int seat = T.query_seat(ans.get_trainID(), ans.get_start_date(), ans.get_num1(), ans.get_num2()); 
						    sout << seat << std::endl;
						}
					}
				}
				else if(cmp == string<5>("cost")){
					priority_queue<ticket_train, cmp_price> res = TT.query_ticket_price(station1, station2, date);
					int num = res.size();
					sout << num << std::endl;
					for(int i = 0; i < num; ++i){
						ticket_train ans = res.top();
						res.pop();
						bool flag = T.release(ans.get_trainID());
						if(flag){
							ans.print(sout);
						    int seat = T.query_seat(ans.get_trainID(), ans.get_start_date(), ans.get_num1(), ans.get_num2());
						    sout << seat << std::endl;
						}
					}
				}
	 	    }
		    else if(tmp == string<15>("query_transfer")){
		    	string<5> date;
		    	string<20> station1;
		    	string<20> station2;
		    	string<5> cmp;
		    	while(infile.get() != 13 && !infile.eof()){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-d")){
    	    			infile >> date;
					}
					else if(op == string<3>("-s")){
						infile >> station1;
					}
					else if(op == string<3>("-t")){
						infile >> station2;
					}
					else if(op == string<3>("-p")){
						infile >> cmp;
					}
				}
				if(cmp == string<5>("time")){
					pair<ticket_train, ticket_train> ans = TT.query_transfer_time(station1, station2, date, T); 
					if (ans.first == ticket_train()){
						sout << 0 << std::endl;
						continue;
					}
					ans.first.print(sout);
					int seat1 = T.query_seat(ans.first.get_trainID(), ans.first.get_start_date(), ans.first.get_num1(), ans.first.get_num2());
					sout << seat1 << std::endl;
					ans.second.print(sout);
					int seat2 = T.query_seat(ans.second.get_trainID(), ans.second.get_start_date(), ans.second.get_num1(), ans.second.get_num2());
					sout << seat2 << std::endl;
				}
				else if(cmp == string<5>("cost")){
					pair<ticket_train, ticket_train> ans = TT.query_transfer_price(station1, station2, date, T); 
					if (ans.first == ticket_train()){
						sout << 0 << std::endl;
						continue;
					}
					ans.first.print(sout);
					int seat1 = T.query_seat(ans.first.get_trainID(), ans.first.get_start_date(), ans.first.get_num1(), ans.first.get_num2());
					sout << seat1 << std::endl;
					ans.second.print(sout);
					int seat2 = T.query_seat(ans.second.get_trainID(), ans.second.get_start_date(), ans.second.get_num1(), ans.second.get_num2());
					sout << seat2 << std::endl;
				} 
		    }
		    else if(tmp == string<15>("buy_ticket")){
		    	string<20> username;
		    	string<20> trainID;
		    	string<5> date;
		    	string<20> leaving_station;
		    	string<20> arriving_station;
		    	int ticketNum;
		    	string<5> queue("false");
		    	while(infile.get() != 13 && !infile.eof()){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-u")){
    	    			infile >> username;
					}
					else if(op == string<3>("-i")){
    	    			infile >> trainID;
					}
					else if(op == string<3>("-d")){
						infile >> date;
					}
					else if(op == string<3>("-f")){
						infile >> leaving_station;
					}
					else if(op == string<3>("-t")){
						infile >> arriving_station;
					}
					else if(op == string<3>("-n")){
						infile >> ticketNum;
					}
					else if(op == string<3>("-q")){
						infile >> queue;
					}
				}
				bool log = U.query_log(username);
				if(!log) sout << -1 << std::endl;
				else{
					pair<int, route> tmpdata = T.query_ticket(trainID, date, leaving_station, arriving_station, ticketNum);
					int flag = tmpdata.first;
					if(flag == -1) sout << -1 << std::endl;
					else if(flag == 0 && queue == string<5>("false")) sout << -1 << std::endl;
					else if(flag == 0 && queue == string<5>("true")){
						int data = TUT.add_ticket(username, trainID, date, leaving_station, arriving_station, ticketNum, tmpdata.second.leaving_time, tmpdata.second.arriving_time, tmpdata.second.date_fix, tmpdata.second.date_gap, string<8>("pending"), tmpdata.second.price, tmpdata.second.leaving_station, tmpdata.second.arriving_station);
						TUT.add_queue(username, data, trainID, date, leaving_station, arriving_station, ticketNum, tmpdata.second.leaving_time, tmpdata.second.arriving_time, tmpdata.second.date_fix, tmpdata.second.date_gap, string<8>("pending"), tmpdata.second.price, tmpdata.second.leaving_station, tmpdata.second.arriving_station);
						sout << "queue" << std::endl;
					}
					else if(flag == 1){
					    TUT.add_ticket(username, trainID, date, leaving_station, arriving_station, ticketNum, tmpdata.second.leaving_time, tmpdata.second.arriving_time, tmpdata.second.date_fix, tmpdata.second.date_gap, string<8>("success"), tmpdata.second.price, tmpdata.second.leaving_station, tmpdata.second.arriving_station);
						sout << tmpdata.second.price * ticketNum << std::endl;
					}
				}
		    }
		    else if(tmp == string<15>("query_order")){
		    	string<20> username;
		    	while(infile.get() != 13 && !infile.eof()){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-u")){
    	    			infile >> username;
					}
				}
				bool log = U.query_log(username);
				if(!log) sout << -1 << std::endl;
				else{
					TUT.query_order(username, sout);
				}
		    }
		    else if(tmp == string<15>("refund_ticket")){
		    	string<20> username;
		    	int n = 1;
		    	while(infile.get() != 13 && !infile.eof()){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-u")){
    	    			infile >> username;
					}
					if(op == string<3>("-n")){
						infile >> n;
					}
				}
				bool log = U.query_log(username);
				if(!log) sout << -1 << std::endl;
				else{
				    int op = TUT.refund_ticket(username, n, T);
		    	    sout << op << std::endl;	
				}
		    }
		    else if(tmp == string<10>("clean")){
		    	U.~user_related();
		    	T.~train_related();
		    	TT.~ticket_train_related();
		    	TUT.~ticket_user_train_related();
		    	new(&U) user_related();
		    	new(&T) train_related();
		    	new(&TT) ticket_train_related();
		    	new(&TUT) ticket_user_train_related();
		    	sout << 0 << std::endl;
			}
			else if(tmp == string<5>("exit")){
				U.exit();
				sout << "bye" << std::endl;
				sout.flush();
			}
			else if(infile.get() != infile.eof()){
				continue;
			}
	    }
		infile.close();
		//std::cerr << "sout.str().c_str() = " << sout.str().c_str() << std::endl;
		memcpy(buff, sout.str().c_str(), strlen(sout.str().c_str()) + 1);
    }
};
}
