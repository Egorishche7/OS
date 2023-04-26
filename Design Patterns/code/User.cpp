#include "User.h"

using namespace std;

void Admin::action()
{
	cout << "Admin action" << endl;
}

void Mod::action()
{
	cout << "Mod action" << endl;
}

void NormalUser::action()
{
	cout << "Normal user action" << endl;
}