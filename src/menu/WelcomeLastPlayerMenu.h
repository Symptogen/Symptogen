#ifndef _H_SYMPTOGEN_MENU_WELCOME_LAST_PLAYER_MENU_H_
#define _H_SYMPTOGEN_MENU_WELCOME_LAST_PLAYER_MENU_H_

#include "State.h"
#include "MenuManager.h"

/** @namespace Symp */
namespace Symp {

/**
* @class WelcomeLastPlayerMenu class, inherits the #State interface for implementing the State Machine Pattern
* This Menu is the first menu of the application in case players data have been found in the resources. The last known
* #Player have a direct button for going back into its game. Otherwise, the user can go to the #ManageGamesMenu or quit
* the application.
* @see State
* @see Player
* @see GameManager
* @see ManageGamesMenu
* @see MenuManager
*/
class WelcomeLastPlayerMenu : public State {
public:
	WelcomeLastPlayerMenu(Player* lastPlayer);
	~WelcomeLastPlayerMenu(){}

	virtual void init();
	virtual void handleMouseClic(int mouseX, int mouseY);
	virtual void keyDownPressed();
	virtual void keyUpPressed();

	//Getter
	Player* getLastPlayer() const {return m_pLastPlayer;}

private:
	Image* m_resumeGameButton; /**< the resume last #Player game #Button */
	Image* m_quitButton; /**< the quit #Button */
	Image* m_manageGamesButton; /**< the #Button for accesing the #ManageGamesMenu */
	Layout* m_buttonLayout; /**< the main #Layout */
	Image* m_titleImage; /**< the #Image that displays the title */
	Player* m_pLastPlayer; /**< the last #Player data */

};

}

#endif // _H_SYMPTOGEN_MENU_WELCOME_LAST_PLAYER_MENU_H_