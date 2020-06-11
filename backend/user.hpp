#ifndef SJTU_USER_HPP
#define SJTU_USER_HPP

#include "string.hpp"
#include "typedef.hpp"
namespace sjtu{
	class user{
	private:
		PASSWORD password;
		NAME name;
		MAILADDR mailAddr;
		PRIVILEGE privilege;
	public:
		user() = default;
		user(const PASSWORD &Password, const NAME &Name, const MAILADDR &MailAddr, PRIVILEGE Privilege = 10):password(Password), name(Name), mailAddr(MailAddr), privilege(Privilege){}
		user(const user &other) = default;
		user &operator = (const user &other){
			new(this)user(other);
			return *this;
		}
		bool operator == (const user &other){
			return password == other.password && name == other.name && mailAddr == other.mailAddr && privilege == other.privilege;
		}
		inline void modify_privilege(PRIVILEGE Privilege){privilege = Privilege;}
		inline void modify_password(const PASSWORD &Password){password = Password;}
		inline void modify_name(const NAME &Name){name = Name;}
		inline void modify_mailAddr(const MAILADDR &MailAddr){mailAddr = MailAddr;}
		inline PASSWORD &get_password() {return password;}
		inline NAME &get_name() {return name;}
		inline MAILADDR &get_mailAddr() {return mailAddr;}
		inline PRIVILEGE get_privilege() {return privilege;}
		friend inline std::ostream &operator <<(std::ostream &os, const user &us){
			os << us.name << " " << us.mailAddr << " " << us.privilege;
			return os;
		}
	};
}
#endif
