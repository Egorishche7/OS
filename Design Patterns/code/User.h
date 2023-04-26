#pragma once

#include <iostream>

class User
{
public:
    virtual ~User() {}
    virtual void action() = 0;
};

class Admin : public User
{
public:
    void action();
};

class Mod : public User
{
public:
    void action();
};

class NormalUser : public User
{
public:
    void action();
};