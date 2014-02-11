#ifndef _H_SYMPTOGEN_MENU_STATE_H_
#define _H_SYMPTOGEN_MENU_STATE_H_

#include "GuiComponent.h"

namespace Symp {
class MenuManager;
/**
* Interface State. This class is part of the State Machine pattern that implements the menus management.
* @see GameManager
*/
class State {
public:
	State(MenuManager* pMenuManager){m_pMenuManager = pMenuManager;}
	virtual ~State(){}
	virtual void init() = 0;
	virtual void handleMouseClic(int mouseX, int mouseY) = 0;
	virtual void keyDownPressed() = 0;
	virtual void keyUpPressed() = 0;

protected:
	MenuManager* m_pMenuManager;

};

}

#endif //_H_SYMPTOGEN_MENU_STATE_H_