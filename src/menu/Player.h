#ifndef _H_SYMPTOGEN_MENU_PLAYER_H_
#define _H_SYMPTOGEN_MENU_PLAYER_H_

#include <iostream>

/** @namespace Symp */
namespace Symp {

/**
* @class Player class 
* This class contain the main information that describe a #Player : its avatar, its name and its progression in the game.
* The data are loaded and saved thanks to the #Parser. A new #Player can be create by the menus, and destroyed 
* the same way.
* @see State
* @see GameManager
* @see Parser
* @see MenuManager
*/
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
	std::string m_sName; /**< the name of the #Player */
	int m_iAvatarIndex; /**< the index of the Avatar of the #Player following a naming convention for the image files */
	unsigned int m_uiCurrentLevel; /**< the last level finished by the #Player */
};
}

#endif //_H_SYMPTOGEN_MENU_PLAYER_H_