#include "Player.h"

namespace Symp {
Player::Player(std::string sName, int iIndex, unsigned int uiLevel){
	m_sName = sName;
	m_iAvatarIndex = iIndex;
	m_uiCurrentLevel = uiLevel;
}

}