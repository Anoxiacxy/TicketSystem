#ifndef SJTU_TYPEDEF_HPP
#define SJTU_TYPEDEF_HPP

#include "string.hpp"
namespace sjtu{
	typedef string<20> USERNAME;
	typedef string<30> PASSWORD;
	typedef string<10> NAME;
	typedef string<30> MAILADDR;
	typedef int PRIVILEGE;
	
	const USERNAME invalid_username("invalid");
	const PASSWORD invalid_password("invalid");
	const NAME invalid_name("invalid");
	const MAILADDR invalid_mailAddr("invalid");
	const PRIVILEGE invalid_privilege = -1;
	
	typedef string<20> TRAINID;
	typedef int STATIONNUM;
	typedef string<20> STATIONS;
}

#endif
