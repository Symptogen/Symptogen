#ifndef _H_SYMPTOGEN_MENU_STATE_H_
#define _H_SYMPTOGEN_MENU_STATE_H_

#include "GuiComponent.h"

namespace Symp {

class MenuManager;
/**
	Interface State part of the state machine pattern
*/
class State {
public:
	State(){}

	void init(){}
	void handleKeyPressed(char* key);

	//Setters
	void setMenuManager(MenuManager* pMenuManager) {m_pMenuManager = pMenuManager;}

private:
	std::vector<GuiComponent*>	m_guiComponentArray;
	MenuManager* m_pMenuManager;

};

}

#endif //_H_SYMPTOGEN_MENU_STATE_H_