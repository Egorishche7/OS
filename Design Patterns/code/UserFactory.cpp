#include "UserFactory.h"

using namespace std;

User* AdminFactory::createUser()
{
	return new Admin();
}

User* ModFactory::createUser()
{
	return new Mod();
}

User* NormalUserFactory::createUser()
{
	return new NormalUser();
}