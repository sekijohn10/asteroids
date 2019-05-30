/*********************************************************************
 * File: levels.cpp
 * Description: Contains the implementaiton of the levels class
 *  methods. Along with static variable declarations.
 *********************************************************************/
#include "levels.h"

//Password uses a - z on the ASCII scale, must convert from int.
std::string Levels::getPasskey()
{
	std::string password;
	password += "cx";
	password += (97 + (level / 25));
	password += (97 + (level % 25));
	password += (97 + (lives / 25));
	password += (97 + (lives % 25));
	return password;
}

void Levels::reset()
{
	level = 1;
	lives = 3;
}

//Password uses a - z on the ASCII scale, must convert to int.
bool Levels::usePasskey(const std::string & password)
{
	if (!validate(password))
		return false;
	else
	{
		level = ((password[2] - 97) * 25 + (password[3] - 97));
		lives = ((password[4] - 97) * 25 + (password[5] - 97));
		return true;
	}
}

//Will make sure string is a valid size and has passkey.
bool Levels::validate(const std::string & password)
{
	if (password.size() == 6)
		if (password[0] == passKey)
			if (password[1] == passKey2)
				return true;
	return false;
}

//Static variables
int Levels::level = 1;
int Levels::lives = 3;