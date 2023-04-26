#pragma once

#include "User.h"
#include <iostream>

class UserFactory
{
public:
    virtual ~UserFactory() {}
    virtual User* createUser() = 0;
};

class AdminFactory : public UserFactory
{
public:
    User* createUser();
};

class ModFactory : public UserFactory
{
public:
    User* createUser();
};

class NormalUserFactory : public UserFactory
{
public:
    User* createUser();
};