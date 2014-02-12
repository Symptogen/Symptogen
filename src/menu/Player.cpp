#include "Player.h"

/** @namespace Symp */
namespace Symp {

/**
* @brief Player constructor
* Responsible for the initialization of the private attributes of the #Player class. 
* @param sName is the string that contain the name of the #Player
* @param iIndex is the index of its avatar following a naming convention in the image folder
* @param uiLevel is the last finished level of the #Player
* @see MenuManager
* @see GameManager
* @see Parser
*/
Player::Player(std::string sName, int iIndex, unsigned int uiLevel){
	m_sName = sName;
	m_iAvatarIndex = iIndex;
	m_uiCurrentLevel = uiLevel;
}

}