#ifndef USER_H
#define USER_H
#include <cstring>
class User {
private:
    char * username;
    char * passwprd;
    wchar_t * name;
    char * mailAddr;
    int privilege;
public:
    User() {

    }
    User(
        const char * username,
        const char * passwprd,
        const wchar_t * name,
        const char * mailAddr,
        const int privilege
    );
};
#endif