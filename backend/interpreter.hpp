#pragma once

#include "user_related.hpp"
#include "train_related.hpp"
#include "ticket_train_related.hpp"
#include "ticket_user_train_related.hpp"
#include <iostream> 
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
    	ticket_train_related TT;
    	ticket_user_train_related TUT;
    public:
    Interpreter(){}
    void operator() (char *buff) {
    	std::ofstream outfile("data.txt");
    	std::ifstream infile;
    	outfile << buff;
    	outfile.close();
    	infile.open("data.txt");
    	string<15>tmp;
    	while(infile >> tmp){
    	    if(tmp == string<10>("add_user")){
    	    	string<20> username1;
    	    	string<20> username2;
    	    	string<30> password;
    	    	string<10> name;
    	    	string<30> mailAddr;
    	    	int privilege;
    	    	while(infile.get() != '\n'){
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
					std::cout << U.add_user(username2, password, name, mailAddr, privilege) << std::endl;
				}
				else{
					std::cout << U.add_user(username1, username2, password, name, mailAddr, privilege) << std::endl;
				}
		    } 
		    else if(tmp == string<10>("login")){
		    	string<20> username;
    	    	string<30> password;
    	    	while(infile.get() != '\n'){
    	    		string<3> op;
    	    		infile >> op;
					if(op == string<3>("-u")){
    	    			infile >> username;
					}
					else if(op == string<3>("-p")){
    	    			infile >> password;
					}
				}
				std::cout << U.login(username, password) << std::endl;
		    }
		    else if(tmp == string<10>("logout")){
		    	string<20> username;
    	    	while(infile.get() != '\n'){
    	    		string<3> op;
    	    		infile >> op;
					if(op == string<3>("-u")){
    	    			infile >> username;
					}
				}
				std::cout << U.logout(username) << std::endl;
		    }
		    else if(tmp == string<15>("query_profile")){
		    	string<20> username1;
    	    	string<20> username2;
    	    	while(infile.get() != '\n'){
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
				if(Tmp.first == false) std::cout << -1 << std::endl;
				else{
					std::cout << username2 << " " << Tmp.second.second << std::endl;
				}
		    }
		    else if(tmp == string<15>("modify_profile")){
		    	string<20> username1;
    	    	string<20> username2;
    	    	string<30> password = invalid_password;
    	    	string<10> name = invalid_name;
    	    	string<30> mailAddr = invalid_mailAddr;
    	    	int privilege = invalid_privilege;
    	    	while(infile.get() != '\n'){
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
				if(Tmp.first == false) std::cout << -1 << std::endl;
				else{
					std::cout << username2 << " " << Tmp.second.second << std::endl;
				}
		    }
		    else if(tmp == string<10>("add_train")){
		    	string<20> trainID;
		    	int stationNum;
		    	int seatNum;
		    	while(infile.get() != '\n'){
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
		    	while(infile.get() != '\n'){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-s")){
    	    			for(int i = 0; i < stationNum - 1; ++i){
    	    				char Tmp[20];
    	    				infile.getline(Tmp, 20, '|');
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
						infile >> saleDate_from;
						infile.get();
						infile >> saleDate_to;
					}
					else if(op == string<3>("-y")){
						infile >> type;
					}
				}
				int tmp = T.add_train(trainID, stationNum, stations, prices, startTime, travelTimes, stopoverTimes, saleDate_from, saleDate_to, type, seatNum);
				std::cout << tmp << std::endl;
				if(tmp == -1) continue;
				else{
					int date_gap = 0;
					int date_fix = 0;
					int price = 0;
					string<5> arriving_time = startTime;
					string<5> leaving_time = startTime;
					for(int i = 0; i < stationNum; ++i){
					    TT.add_ticket_train(stations[i], trainID, saleDate_from, saleDate_to, date_gap, date_fix, leaving_time, arriving_time, i, price);
					    if(i != stationNum - 1){
					    	price += prices[i + 1];
					        pair<int, string<5> > tmpTime = add_time(leaving_time, travelTimes[i + 1]);
					        date_fix += tmpTime.first;
					        pair<int, string<5> > tmpTime2 = add_time(leaving_time, travelTimes[i + 1] + stopoverTimes[i + 1]);
					        date_gap = tmpTime2.first - tmpTime.first;
					        arriving_time = tmpTime.second;
					        leaving_time = tmpTime2.second;
						}
				    }
				}
		    }
		    else if(tmp == string<15>("release_train")){
		    	string<20> trainID;
		    	while(infile.get() != '\n'){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-i")){
    	    			infile >> trainID;
					}
				}
				std::cout << T.release_train(trainID) << std::endl;
		    }
		    else if(tmp == string<15>("query_train")){
		    	string<20> trainID;
		    	string<5> date;
		    	while(infile.get() != '\n'){
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
				if(Tmp.first == false) std::cout << -1 << std::endl;
				else{
					if(cmp_date(Tmp.second.second.get_saleDate_from(), date) == 1 || cmp_date(Tmp.second.second.get_saleDate_to(), date) == -1) std::cout << -1 << std::endl;
					else{
						std::cout << trainID << " ";
						Tmp.second.second.print(date);
					}
				}
		    }
		    else if(tmp == string<15>("delete_train")){
		    	string<20> trainID;
		    	while(infile.get() != '\n'){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-i")){
    	    			infile >> trainID;
					}
				}
				std::cout << T.delete_train(trainID) << std::endl;
		    }
		    else if(tmp == string<15>("query_ticket")){
		    	string<5> date;
		    	string<20> station1;
		    	string<20> station2;
		    	string<5> cmp;
		    	while(infile.get() != '\n'){
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
					std::cout << num << std::endl;
					for(int i = 0; i < num; ++i){
						ticket_train ans = res.top();
						res.pop();
						bool flag = T.release(ans.get_trainID());
						if(flag){
							ans.print();
						    int seat = T.query_seat(ans.get_trainID(), ans.get_start_date(), ans.get_num1(), ans.get_num2());
						    std::cout << seat << std::endl;
						}
					}
				}
				else if(cmp == string<5>("price")){
					priority_queue<ticket_train, cmp_price> res = TT.query_ticket_price(station1, station2, date);
					int num = res.size();
					for(int i = 0; i < num; ++i){
						ticket_train ans = res.top();
						res.pop();
						bool flag = T.release(ans.get_trainID());
						if(flag){
							ans.print();
						    int seat = T.query_seat(ans.get_trainID(), ans.get_start_date(), ans.get_num1(), ans.get_num2());
						    std::cout << seat << std::endl;
						}
					}
				}
	 	    }
		    else if(tmp == string<15>("query_transfer")){
		    	string<5> date;
		    	string<20> station1;
		    	string<20> station2;
		    	string<5> cmp;
		    	while(infile.get() != '\n'){
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
				}
				else if(cmp == string<5>("price")){
					
				}
		    }
		    else if(tmp == string<15>("buy_ticket")){
		    	string<20> username;
		    	string<20> trainID;
		    	string<5> date;
		    	string<20> leaving_station;
		    	string<20> arriving_station;
		    	int ticketNum;
		    	bool queue = false;
		    	while(infile.get() != '\n'){
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
				if(!log) std::cout << -1 << std::endl;
				else{
					pair<int, route> tmpdata = T.query_ticket(trainID, date, leaving_station, arriving_station, ticketNum);
					int flag = tmpdata.first;
					if(flag == -1) std::cout << -1 << std::endl;
					else if(flag == 0 && queue == false) std::cout << -1 << std::endl;
					else if(flag == 0 && queue == true){
						TUT.add_ticket(username, trainID, date, leaving_station, arriving_station, ticketNum, tmpdata.second.leaving_time, tmpdata.second.arriving_time, tmpdata.second.date_fix, tmpdata.second.date_gap, string<8>("queue"), tmpdata.second.price, tmpdata.second.leaving_station, tmpdata.second.arriving_station);
						TUT.add_queue(username, trainID, date, leaving_station, arriving_station, ticketNum, tmpdata.second.leaving_time, tmpdata.second.arriving_time, tmpdata.second.date_fix, tmpdata.second.date_gap, string<8>("queue"), tmpdata.second.price, tmpdata.second.leaving_station, tmpdata.second.arriving_station);
						std::cout << "queue" << std::endl;
					}
					else if(flag == 1){
					    TUT.add_ticket(username, trainID, date, leaving_station, arriving_station, ticketNum, tmpdata.second.leaving_time, tmpdata.second.arriving_time, tmpdata.second.date_fix, tmpdata.second.date_gap, string<8>("success"), tmpdata.second.price, tmpdata.second.leaving_station, tmpdata.second.arriving_station);
						std::cout << tmpdata.second.price * ticketNum << std::endl;
					}
				}
		    }
		    else if(tmp == string<15>("query_order")){
		    	string<20> username;
		    	while(infile.get() != '\n'){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-u")){
    	    			infile >> username;
					}
				}
				std::cout << TUT.query_order(username) << std::endl;
		    }
		    else if(tmp == string<15>("refund_ticket")){
		    	string<20> username;
		    	int n = 1;
		    	while(infile.get() != '\n'){
    	    		string<3> op;
    	    		infile >> op;
    	    		if(op == string<3>("-u")){
    	    			infile >> username;
					}
					if(op == string<3>("-n")){
						infile >> n;
					}
				}
		    	int op = TUT.refund_ticket(username, n, T);
		    	std::cout << op << std::endl;
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
		    	std::cout << 0 << std::endl;
			}
			else if(tmp == string<5>("exit")){
				U.exit();
				std::cout << "bye" << std::endl;
				std::cout.flush();
			}
	    }
    }
};
}
