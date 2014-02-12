#ifndef _H_SYMPTOGEN_MENU_PLAYER_H_
#define _H_SYMPTOGEN_MENU_PLAYER_H_

#include <iostream>

namespace Symp {

class Player {
public:
	Player(std::string sName, int iIndex, unsigned int uiLevel = 0);
	~Player();

	//Setters
	void setName(std::string sName) {m_sName = sName;}
	void setAvatarIndex( int iIndex) {m_iAvatarIndex = iIndex;}
	void setCurrentLevel(unsigned int uiLevel) {m_uiCurrentLevel = uiLevel;}

	//Getters
	std::string getName() const {return m_sName;}
	int getAvatarIndex() const {return m_iAvatarIndex;}
	unsigned int getCurrentLevel() const {return m_uiCurrentLevel;}

private:
	std::string m_sName;
	int m_iAvatarIndex;
	unsigned int m_uiCurrentLevel;
};
}

#endif //_H_SYMPTOGEN_MENU_PLAYER_H_