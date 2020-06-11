#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include "train.hpp"
#include "string.hpp"
#include "vector.hpp"
#include "ticket_user_train_related.hpp"
using namespace std;

int main(){
	sjtu::vector<sjtu::pair<sjtu::ticket_user_train, sjtu::string<20>>> tmp;
	tmp.push_back(sjtu::pair<sjtu::ticket_user_train, sjtu::string<20>>(sjtu::ticket_user_train(), "1"));
	tmp.push_back(sjtu::pair<sjtu::ticket_user_train, sjtu::string<20>>(sjtu::ticket_user_train(), "2"));
	tmp.push_back(sjtu::pair<sjtu::ticket_user_train, sjtu::string<20>>(sjtu::ticket_user_train(), "1"));
	tmp.push_back(sjtu::pair<sjtu::ticket_user_train, sjtu::string<20>>(sjtu::ticket_user_train(), "2"));
	tmp.push_back(sjtu::pair<sjtu::ticket_user_train, sjtu::string<20>>(sjtu::ticket_user_train(), "1"));
	tmp.push_back(sjtu::pair<sjtu::ticket_user_train, sjtu::string<20>>(sjtu::ticket_user_train(), "2"));
	tmp.push_back(sjtu::pair<sjtu::ticket_user_train, sjtu::string<20>>(sjtu::ticket_user_train(), "1"));
	tmp.push_back(sjtu::pair<sjtu::ticket_user_train, sjtu::string<20>>(sjtu::ticket_user_train(), "2"));
	tmp.push_back(sjtu::pair<sjtu::ticket_user_train, sjtu::string<20>>(sjtu::ticket_user_train(), "1"));
	tmp.push_back(sjtu::pair<sjtu::ticket_user_train, sjtu::string<20>>(sjtu::ticket_user_train(), "2"));
	tmp.erase(3);
	cout << tmp[0].second;
	return 0;
}
