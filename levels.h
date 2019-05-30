/*********************************************************************
 * File: levels.h
 * Description: This class will store info related to the level and lives
 *  of the any game. It is compatiable with all games. It incorportates a 
 *  password that can be used.
 *********************************************************************/
#ifndef LEVELS_H
#define LEVELS_H

#include <string>

class Levels
{
private:
	//These two varaibles will store the level and lives of the object
	static int level;
	static int lives;

	//These two varaibles will store the passkey of the password.
	const static char passKey = 'c';
	const static char passKey2 = 'x';
public:
	Levels() {  }
	
	int getLevel() const
	{
		return level;
	}
	int getLives() const
	{
		return lives;
	}
	void addLife()
	{
		if(lives < 650)
			lives++;
	}
	void addLevel()
	{
		if (level < 650)
			level++;
	}
	bool takeLife()
	{
		return --lives;
	}
	std::string getPasskey();

	void reset();

	/***************
	*This function will set the level and lives according to the password entered,
	*It returns false if the password was invalid
	***************/
	bool usePasskey(const std::string & password);
	bool validate(const std::string & password);
};


#endif //LEVELS_H