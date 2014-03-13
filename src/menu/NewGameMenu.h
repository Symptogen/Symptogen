#ifndef _H_SYMPTOGEN_MENU_NEW_GAME_MENU_H_
#define _H_SYMPTOGEN_MENU_NEW_GAME_MENU_H_

#include "State.h"
#include "MenuManager.h"

/** @namespace Symp */
namespace Symp {

/**
* @class NewGameMenu class, inherits the #State interface for implementing the State Machine Pattern
* This Menu is the graphical interface for creating a new #Player. The user can choose an avatar and type a name
* to register its #Player. Then it can directly start a new game, or go back.
* @see State
* @see LineEdit
* @see GameManager
* @see ManageGamesMenu
* @see MenuManager
*/
class NewGameMenu : public State {
public:
	NewGameMenu();
	~NewGameMenu(){}

	virtual void init();
	virtual void handleMouseClic(int mouseX, int mouseY);
	virtual void keyDownPressed();
	virtual void keyUpPressed();

private:
	Text* m_pExplanations;
	Image* m_pBackButton; /**< the go back #Button */
	Image* m_pLaunchButton; /**< the create #Player and launch new game #Button */
	Layout* m_pButtonLayout; /**< the main #Layout */
	Layout* m_pArrowLayout; /**< the #Layout for the avatar naviguation arrows */
	Button* m_pLeftArrow; /**< the left arrow #Button */
	Button* m_pRightArrow; /**< the right arrow #Button */
	Image* m_pTitleImage; /**< the #Image that displays the title */
	LineEdit* m_pLineEdit; /**< the #LineEdit for the user to enter a name */
	std::vector<Image*> m_avatarVector; /**< the list of avatar #Image */
	Image* m_pCurrentAvatar; /**< the currently displayed avatar #Image */
	std::string m_sName; /**< the name typed by the user */

};

}

#endif // _H_SYMPTOGEN_MENU_NEW_GAME_MENU_H_