#include "Database.h"

using namespace std;

string Database::getUserType(const string& username)
{
    if (username == "admin")
        return "admin";
    else if (username == "moderator")
        return "mod";
    else
        return "user";
}