#ifndef _H_SYMPTOGEN_MENU_MANAGE_GAMES_MENU_H_
#define _H_SYMPTOGEN_MENU_MANAGE_GAMES_MENU_H_

#include "State.h"
#include "MenuManager.h"


namespace Symp {
/**
	WelcomeLAST_PLAYERMenu inherited from the State interface.
*/

class ManageGamesMenu : public State {
public:
	ManageGamesMenu(MenuManager* pMenuManager);
	~ManageGamesMenu();

	virtual void init();
	virtual void handleMouseClic(int mouseX, int mouseY);
	virtual void keyDownPressed();
	virtual void keyUpPressed();

	Layout* createPlayerPanel(Player* pPlayer, int iPosX, int iPosY, int iWidth, int iHeight, Color color);

private:
	Button* m_pCreateNewGameButton;
	Button* m_pBackButton;
	Image* m_pCurrentGameLabel;
	Image* m_pLoadAnotherGameLabel;
	Layout* m_pButtonLayout;
	Image* m_pTitleImage;

	//Temporary
	Button* m_pPlayer1Button;
	Layout* m_pLastPlayerLayout;
	Button* m_TestButton;

};

}

#endif // _H_SYMPTOGEN_MENU_MANAGE_GAMES_MENU_H_