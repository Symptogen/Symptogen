#ifndef _H_SYMPTOGEN_MENU_MANAGE_GAMES_MENU_H_
#define _H_SYMPTOGEN_MENU_MANAGE_GAMES_MENU_H_

#include "State.h"
#include "MenuManager.h"

/** @namespace Symp */
namespace Symp {

/**
* @class ManageGamesMenu class, inherits the #State interface for implementing the State Machine Pattern
* This Menu show the list of the players currently registrated into the application. The last #Player is shown 
* above the other. The click on a #Player panel will bring the user to the specific #Player menu #ChooseYourLevelMenu.
* The user can create a new #Player from this menu, or go back to the main menu.
* @see State
* @see GameManager
* @see ChooseYourLevelMenu
* @see WelcomeLastPlayerMenu
* @see MenuManager
*/
class ManageGamesMenu : public State {
public:
	ManageGamesMenu();
	~ManageGamesMenu(){}

	virtual void init();
	virtual void handleMouseClic(int mouseX, int mouseY);
	virtual void keyDownPressed();
	virtual void keyUpPressed();
	std::string getLevelName(int level);

	Layout* createPlayerPanel(Player* pPlayer, int iPosX, int iPosY, int iWidth, int iHeight, Color borderColor, Color backgroundColor);

private:
	Image* m_background;/**< the #Image that displays the background */
	Image* m_pCreateNewGameButton; /**< the #Button to create a new #Player */
	Image* m_pBackButton; /**< the go back #Button */
	Image* m_pCurrentGameLabel; /**< the #Image that display "Current Game" label */
	Image* m_pLoadAnotherGameLabel; /**< the #Image that display "Load another game " label*/
	Layout* m_pButtonLayout; /**< the main #Layout of the menu */
	Image* m_pTitleImage; /**< the #Image that display the title of the menu */
	std::map<Button*, Player*> m_buttonMap; /**< associate a #Player with its #Button */
	std::map<Image*, Player*> m_crossMap; /**< associate a #Player with a cross #Button to suppress it */
	

};

}

#endif // _H_SYMPTOGEN_MENU_MANAGE_GAMES_MENU_H_