#include <iostream>
#include <string>
#include <map>

using namespace std;

// Abstract Factory pattern
class User
{
public:
    virtual ~User() {}
    virtual void action() = 0;
};

class Admin : public User
{
public:
    void action() { cout << "Admin action" << endl; }
};

class Mod : public User
{
public:
    void action() { cout << "Mod action" << endl; }
};

class NormalUser : public User
{
public:
    void action() { cout << "Normal user action" << endl; }
};

class UserFactory
{
public:
    virtual ~UserFactory() {}
    virtual User* createUser() = 0;
};

class AdminFactory : public UserFactory
{
public:
    User* createUser() { return new Admin(); }
};

class ModFactory : public UserFactory
{
public:
    User* createUser() { return new Mod(); }
};

class NormalUserFactory : public UserFactory
{
public:
    User* createUser() { return new NormalUser(); }
};

// Singleton pattern
class UserFactoryManager
{
public:
    static UserFactoryManager& getInstance()
    {
        static UserFactoryManager instance; // create only one instance
        return instance;
    }

    void registerFactory(const string& type, UserFactory* factory)
    {
        factories[type] = factory;
    }

    UserFactory* getFactory(const string& type)
    {
        return factories[type];
    }

private:
    UserFactoryManager() {} // private constructor makes you can't create instances from outside
    map<string, UserFactory*> factories;
};

// Class to represent the database
class Database
{
public:
    // Returns the user type based on the username
    // Returning constants here is just an example
    string getUserType(const string& username)
    {
        if (username == "admin")
            return "admin";
        else if (username == "moderator")
            return "mod";
        else
            return "user";
    }
};

int main()
{
    AdminFactory adminFactory;
    ModFactory modFactory;
    NormalUserFactory normalUserFactory;

    UserFactoryManager& manager = UserFactoryManager::getInstance();
    manager.registerFactory("admin", &adminFactory);
    manager.registerFactory("mod", &modFactory);
    manager.registerFactory("user", &normalUserFactory);

    Database db;

    string username = "moderator";
    string userType = db.getUserType(username);

    UserFactory* factory = manager.getFactory(userType);
    User* user = factory->createUser();
    user->action();

    delete user;

    return 0;
}