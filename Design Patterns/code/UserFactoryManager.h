#pragma once

#include "UserFactory.h"
#include <iostream>
#include <map>

using namespace std;

class UserFactoryManager
{
public:
    static UserFactoryManager& getInstance();
    void registerFactory(const string&, UserFactory*);
    UserFactory* getFactory(const string&);
private:
    UserFactoryManager() {} // private constructor makes you can't create instances from outside
    map<string, UserFactory*> factories;
};