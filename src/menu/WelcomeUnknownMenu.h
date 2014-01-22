#ifndef _H_SYMPTOGEN_MENU_WELCOME_UNKNOWN_MENU_H_
#define _H_SYMPTOGEN_MENU_WELCOME_UNKNOWN_MENU_H_

#include "State.h"
#include "GuiComponent.h"

namespace Symp {

class WelcomeUnknownMenu : public State {
public:
	WelcomeUnknownMenu(){}

	void handleKeyPressed(char* key);

	//Setters
	void setMenuManager(MenuManager* pMenuManager) {m_pMenuManager = pMenuManager;}

private:
	std::vector<GuiComponent*>	m_guiComponentArray;
	MenuManager* m_pMenuManager;
};

}

#endif // _H_SYMPTOGEN_MENU_WELCOME_UNKNOWN_MENU_H_