#include "UserFactoryManager.h"

using namespace std;

UserFactoryManager& UserFactoryManager::getInstance()
{
    static UserFactoryManager instance; // create only one instance
    return instance;
}

void UserFactoryManager::registerFactory(const string& type, UserFactory* factory)
{
    factories[type] = factory;
}

UserFactory* UserFactoryManager::getFactory(const string& type)
{
    return factories[type];
}