#ifndef _H_SYMPTOGEN_MENU_PAUSE_MENU_H_
#define _H_SYMPTOGEN_MENU_PAUSE_MENU_H_

#include "State.h"
#include "MenuManager.h"

/** @namespace Symp */
namespace Symp {
/**
* @class PauseMenu class, inherits the #State interface for implementing the State Machine Pattern
* This Menu is the only in game menu. It appears while the player is in a game, on the escape key event. This
* class is directly managed by the #GameManager for it is appearing in the middle of the game. The user can from
* this menu restart a level, quit the game to display the main menu or resume its game.
* @see State
* @see GameManager
* @see WelcomeLastPlayerMenu
* @see MenuManager
*/
class PauseMenu : public State {
public:
	PauseMenu(MenuManager* pMenuManager, float posX, float posY);
	~PauseMenu();

	virtual void init();
	virtual void handleMouseClic(int mouseX, int mouseY);
	virtual void keyDownPressed();
	virtual void keyUpPressed();

private:
	Button* m_resumeGameButton; /**< the resume game #Button */
	Button* m_optionsButton; /**< the options or restart level #Button */
	Button* m_backToMenuButton; /**< the #Button that quit game and display the main menu */
	Layout* m_buttonLayout; /**< the main #Layout */
	float 	m_posX;
	float 	m_posY;
};

}
#endif // _H_SYMPTOGEN_MENU_PAUSE_MENU_H_