#ifndef SJTU_USER_RELATED_HPP
#define SJTU_USER_RELATED_HPP

#include "user.hpp"
#include "vector.hpp"
#include "../uti.hpp"
namespace sjtu{
	class user_related{
	private:
		bptree<USERNAME, user> usertree;
	    vector<pair<USERNAME, PRIVILEGE> >login_list;  
	public:
		user_related():usertree("usertree", "userindex"), login_list(){}
	    int add_user(const USERNAME &Username, const PASSWORD &Password, const NAME &Name, const MAILADDR &MailAddr, PRIVILEGE Privilege){
	    	int success = usertree.count(Username);
	    	if(success == 1) return -1;
	    	else{
	    		usertree.insert(Username, user(Password, Name, MailAddr, Privilege));
	    		return 0;
			}
		} 
		bool query_log(const USERNAME &Username){
			for(int i = 0; i < login_list.size(); ++i){
				if(login_list[i].first == Username) return true;
			}
			return false;
		}
		int add_user(const USERNAME &Username1, const USERNAME &Username2, const PASSWORD &Password, const NAME &Name, const MAILADDR &MailAddr, PRIVILEGE Privilege){
			int i;
			if(login_list.size() == 0) return -1;
			for(i = 0; i < login_list.size(); ++i){
				if(login_list[i].first == Username1) break;
				if(login_list[i].first != Username1 && i == login_list.size() - 1) return -1;
			}
			if(login_list[i].second <= Privilege) return -1;
	    	int success = usertree.count(Username2);
	    //	std::cout << usertree.count(string<20>("Croissant")) << "WWW" << std::endl;
	    //	std::cout << Username1 << std::endl;
	    	if(success == 1) return -1;
	    	else{
	    		usertree.insert(Username2, user(Password, Name, MailAddr, Privilege));
	    //		std::cout << usertree.count(string<20>("Croissant")) << "WWW" << std::endl;
	    		return 0;
			}
		} 
		int login(const USERNAME &Username, const PASSWORD &Password){
			user tmp = usertree.at(Username);
			if(tmp == user()) return -1;
			if(tmp.get_password() != Password) return -1;
			for(int i = 0; i < login_list.size(); ++i) if(login_list[i].first == Username) return -1;
			login_list.push_back(pair<USERNAME, PRIVILEGE>(Username, tmp.get_privilege()));
			return 0;
		}
		int logout(const USERNAME &Username){
			int i;
			if(login_list.size() == 0) return -1;
			for(i = 0; i < login_list.size(); ++i){
				if(login_list[i].first == Username) break;
				if(login_list[i].first != Username && i == login_list.size() - 1) return -1;
			}
			login_list.erase(i);
			return 0;
		}
		pair<bool, pair<USERNAME, user> > query_profile(const USERNAME &Username1, const USERNAME &Username2){
			int i;
			if(login_list.size() == 0) return pair<bool, pair<USERNAME, user> >(false, pair<USERNAME, user>(Username2, user()));
			for(i = 0; i < login_list.size(); ++i){
				if(login_list[i].first == Username1) break;
				if(login_list[i].first != Username1 && i == login_list.size() - 1) return pair<bool, pair<USERNAME, user> >(false, pair<USERNAME, user>(Username2, user()));
			}
			user tmp = usertree.at(Username2);
			if((Username1 != Username2 && login_list[i].second <= tmp.get_privilege()) || tmp == user()) return pair<bool, pair<USERNAME, user> >(false, pair<USERNAME, user>(Username2, user()));
			else return pair<bool, pair<USERNAME, user> >(true, pair<USERNAME, user>(Username2, tmp));
		}
		pair<bool, pair<USERNAME, user> > modify_profile(const USERNAME &Username1, const USERNAME &Username2, const PASSWORD &Password, const NAME &Name, const MAILADDR &MailAddr, const PRIVILEGE &Privilege){
			int i;
			if(login_list.size() == 0)  return pair<bool, pair<USERNAME, user> >(false, pair<USERNAME, user>(Username2, user()));
			for(i = 0; i < login_list.size(); ++i){
				if(login_list[i].first == Username1) break;
				if(login_list[i].first != Username1 && i == login_list.size() - 1) return pair<bool, pair<USERNAME, user> >(false, pair<USERNAME, user>(Username2, user()));
			}
			user tmp = usertree.at(Username2);
			if((Username1 != Username2 && login_list[i].second <= tmp.get_privilege()) || tmp == user() || login_list[i].second <= Privilege) return pair<bool, pair<USERNAME, user> >(false, pair<USERNAME, user>(Username2, tmp));
			else{
				if(Password != invalid_password) tmp.modify_password(Password);
				if(Name != invalid_name)tmp.modify_name(Name);
				if(MailAddr != invalid_mailAddr)tmp.modify_mailAddr(MailAddr);
				if(Privilege != invalid_privilege){
					tmp.modify_privilege(Privilege);
					for(int i = 0; i < login_list.size(); ++i){
						if(login_list[i].first == Username2) login_list[i].second = Privilege; 
					}
				}
				bool flag = usertree.modify(Username2, tmp);
				return pair<bool, pair<USERNAME, user> >(true, pair<USERNAME, user>(Username2, tmp));
			}
		}
		inline void exit(){
			login_list.clear();
			new(&login_list)vector<pair<USERNAME, PRIVILEGE> >();
		}	
	};
}

#endif
