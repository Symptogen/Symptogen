#ifndef _H_SYMPTOGEN_MENU_WELCOME_UNKNOWN_MENU_H_
#define _H_SYMPTOGEN_MENU_WELCOME_UNKNOWN_MENU_H_

#include "State.h"
#include "MenuManager.h"

/** @namespace Symp */
namespace Symp {

/**
* @class WelcomeUnknownMenu class, inherits the #State interface for implementing the State Machine Pattern
* This Menu is the first menu of the application in case no players data have been found in the resources. The user can
* only create a new game or quit the application.
* @see NewGameMenu
* @see State
* @see GameManager
* @see ManageGamesMenu
* @see MenuManager
*/
class WelcomeUnknownMenu : public State {
public:
	WelcomeUnknownMenu();
	~WelcomeUnknownMenu(){}

	virtual void init();
	virtual void handleMouseClic(int mouseX, int mouseY);
	virtual void keyDownPressed();
	virtual void keyUpPressed();

private:
	Image* m_background;/**< the #Image that displays the background */
	Image* m_createGameButton; /**< the #Button that displays the NewGameMenu */
	Image* m_quitButton; /**< the quit #Button */
	Image* m_manageGamesButton; /**< the #Button for accesing the #ManageGamesMenu */
	Layout* m_buttonLayout; /**< the main #Layout */

};

}

#endif // _H_SYMPTOGEN_MENU_WELCOME_UNKNOWN_MENU_H_