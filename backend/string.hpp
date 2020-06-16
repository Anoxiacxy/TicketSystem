#ifndef SJTU_STRING_HPP
#define SJTU_STRING_HPP

#include "array.hpp"
namespace sjtu{
	template <int length>
	class string;
	template <int length>
	std::istream &operator >> (std::istream &is, string<length> &str); 
	template <int length>
	std::ostream &operator << (std::ostream &os, const string<length> &str); 
	template <int length>
	class string{
	private:
		array<char, length> str;
	public:
		string(const char* STR){
			int Length = strlen(STR);
			for(int i = 0; i < Length; ++i){
				str.push_back(STR[i]);
			}
		}
		string(int a){
			int Length = 1;
			str.push_back(a +'0');
		}
		template <int other_length>
		string(const string<other_length> &other){
			for(int i = 0; i < other.size(); ++i){
				str.push_back(other[i]);
			}
		}
		string() = default;
		~string() = default;
		template <int other_length>
		string &operator = (const string<other_length> &other){
			new(this)string(other);
			return *this;
		}
		inline int size() const{return str.size();}
		inline char &operator[](int pos){return str[pos];}
		inline const char &operator[](int pos) const {return str[pos];}
		friend std::istream &operator >> <length> (std::istream &is, string<length> &str);
		friend std::ostream &operator << <length> (std::ostream &os, const string<length> &str);
		template<int other_length>
		int Compare(const string<other_length> &other){
			for(int i = 0; i < std::min(size(), other.size()); ++i){
				if((*this)[i] < other[i]){return -1;}
				else if((*this)[i] > other[i]){return 1;}
			}
			if(size() < other.size()) return -1;
			else if(size() == other.size()) return 0;
			else return 1;
		}
		int toint(){
			int ans = 0;
			for(int i = 0; i < size(); ++i){
				ans += (int)(*this)[i];
			}
			return ans;
		}
		template <int other_length>
		bool operator == (const string<other_length> &other) const{
			if(size() != other.size()) return false;
			for(int i = 0; i < size(); ++i){
				if((*this)[i] != other[i]) return false;
			}
			return true;
		}
		template <int other_length>
		bool operator != (const string<other_length> &other) const{
			return !((*this) == other);
		}
		template <int other_length>
		bool operator < (const string<other_length> &other) const{
			for(int i = 0; i < std::min(size(), other.size()); ++i){
				if((*this)[i] < other[i]) return true;
				else if((*this)[i] > other[i]) return false;
			}
			if(size() < other.size()) return true;
			else return false;
		}
		template <int other_length>
		bool operator > (const string<other_length> &other) const{
			return !((*this) < other || (*this) == other);
		}
		template <int other_length>
		bool operator <= (const string<other_length> &other) const{
			return !((*this) > other);
		}
		template <int other_length>
		bool operator >= (const string<other_length> &other) const{
			return !((*this) < other);
		}
	};
	template <int length>
	std::istream &operator >>(std::istream &is, string<length> &str){
		char tmp[31];
		is >> tmp;
		new(&str) string<length>(tmp);
		return is;
	}
	template <int length>
	std::ostream &operator <<(std::ostream &os, const string<length> &str){
		for(int i = 0 ; i < str.size(); ++i){os << str[i];}
		return os;
	}
}
#endif
