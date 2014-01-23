#ifndef _H_SYMPTOGEN_MENU_WELCOME_UNKNOWN_MENU_H_
#define _H_SYMPTOGEN_MENU_WELCOME_UNKNOWN_MENU_H_

#include "State.h"
#include "MenuManager.h"


namespace Symp {
/**
	WelcomeUnknownMenu inherited from the State interface.
*/

class WelcomeUnknownMenu : public State {
public:
	WelcomeUnknownMenu(MenuManager* pMenuManager);
	~WelcomeUnknownMenu();

	void init();
	virtual void handleMouseClic(int mouseX, int mouseY);
	virtual void keyDownPressed();
	virtual void keyUpPressed();

};

}

#endif // _H_SYMPTOGEN_MENU_WELCOME_UNKNOWN_MENU_H_