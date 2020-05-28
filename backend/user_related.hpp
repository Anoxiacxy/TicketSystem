#ifndef SJTU_USER_RELATED_HPP
#define SJTU_USER_RELATED_HPP

#include "user.hpp"
#include "vector.hpp"
#include "Database/b"
namespace sjtu{
	class user_related{
	private:
		bptree<USERNAME, user> usertree;
	    vector<pair<USERNAME, PRIVILEGE>>login_list;  
	public:
		user_related(){
			usertree():usertree("usertree", "userindex"), login_list(){}
		} 
	    int add_user(const USERNAME &Username, const PASSWORD &Password, const NAME &Name, const MAILADDR &MailAddr, PRIVILEGE Privilege){
	    	int success = -usertree.count(Username);
	    	if(!success) return success;
	    	else{
	    		Usertree.insert(Username, user(Password, Name, MailAddr, Privilege));
	    		return success;
			}
		} 
		bool login(const USERNAME &Username, const PASSWORD &Password){
			user tmp = at(Username);
			if(tmp.get_password() != PASSWORD) return false;
			for(int i = 0; i < login_list.size(); ++i) if(login_list[i].first == Username) return false;
			login_list.push_back(pair<USERNAME, PRIVILEGE>(Username, Privilege));
			return true;
		}
		bool logout(const USERNAME &Username){
			int i;
			for(i = 0; i < login_list.size(); ++i){
				if(login_list[i].first == Username) break;
				if(login_list[i].first != Username && i == login_list.size() - 1) return false;
			}
			login_list.erase(i);
			return true;
		}
		pair<bool, pair<USERNAME, user>> query_profile(const USERNAME &Username1, const USERNAME &Username2){
			int i;
			for(i = 0; i < login_list.size(); ++i){
				if(login_list[i].first == Username) break;
				if(login_list[i].first != Username && i == login_list.size() - 1) return pair<bool, pair<USERNAME, user>>(false, pair<USERNAME, user>(Username2, user()));
			}
			user tmp = at(Username2);
			if(Username1 == Username2 || login_list[i].second > tmp.get_privilege()) return pair<bool, pair<USERNAME, user>>(true, pair<USERNMAE, user>(Username2, tmp));
			else  return pair<bool, pair<USERNAME, user>>(false, pair<USERNAME, user>(Username2, user()));
		}
		pair<bool, pair<USERNAME, user>> modify_profile(const USERNAME &Username1, const USERNAME &Username2, const PASSWORD &Password, const NAME &Name, const MAILADDR &MailAddr, const PRIVILEGE &Privilege){
			int i;
			for(i = 0; i < login_list.size(); ++i){
				if(login_list[i].first == Username) break;
				if(login_list[i].first != Username && i == login_list.size() - 1) return pair<bool, pair<USERNAME, user>>(false, pair<USERNAME, user>(Username2, user()));
			}
			user tmp = at(Username2);
			if(Username1 != Username2 && login_list[i].second <= tmp.get_privilege() && Privilege >= login_list[i].second) return pair<bool, pair<USERNAME, user>>(false, pair<USERNAME, user>(Username2, user()));
			else{
				tmp.modify_password(Password);
				tmp.modify_name(Name);
				tmp.modify_mailAddr(Mail_Addr);
				tmp.modify_privilege(Privilege);
				return pair<bool, pair<USERNAME, user>>(true, pair<USERNMAE, user>(Username2, tmp));
			}
		}	
	};
}

#endif
