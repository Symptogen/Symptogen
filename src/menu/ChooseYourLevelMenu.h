#ifndef _H_SYMPTOGEN_MENU_CHOOSE_YOUR_LEVEL_MENU_H_
#define _H_SYMPTOGEN_MENU_CHOOSE_YOUR_LEVEL_MENU_H_

#include "State.h"
#include "MenuManager.h"

/** @namespace Symp */
namespace Symp {

/**
* @class ChooseYourLevelMenu class, inherits the #State interface for implementing the State Machine Pattern
* This Menu show the personal panel of a player. The player can see its current level, and can replay any level
* he already finished. From that menu, the user can go back to #ManageGamesMenu or launch a game by clicking on a
* level.
* @see State
* @see GameManager
* @see ManageGamesMenu
* @see MenuManager
*/
class ChooseYourLevelMenu : public State {
public:
	ChooseYourLevelMenu(Player* pPlayer);
	~ChooseYourLevelMenu(){}

	virtual void init();
	virtual void handleMouseClic(int mouseX, int mouseY);
	virtual void keyDownPressed();
	virtual void keyUpPressed();

	//Getter
	Player* getPlayer() const {return m_pPlayer;}


private:
	Player* m_pPlayer; /**< the #Player whom data are currently displayed */
	Image* m_pChooseLabel; /**< the #Image class reference for the label "Choose your level" texture */
	Button* m_pBackButton; /**< the #Button for going back */
	Layout* m_pPlayerLayout; /**< the #Layout that display the #Player main elements */
	Layout* m_pSliderLayout; /**< the #Layout that display the #Player's progress bar */
	Layout* m_pButtonLayout; /**< the main #Layout that agregate all the #Buttons*/
	Image* m_pTitleImage; /**< the #Image class reference the title label */
	std::vector<Button*> m_levelButtonVector; /**< the list of all the #Button that stands for a level */

};

}

#endif // _H_SYMPTOGEN_MENU_CHOOSE_YOUR_LEVEL_MENU_H_