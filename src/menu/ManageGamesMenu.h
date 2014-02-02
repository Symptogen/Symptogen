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

	//Getter
	Player* getLastPlayer() const {return m_pLastPlayer;}
	std::vector<Player*> getPlayers() const {return m_pPlayers;}

private:
	Button* m_pCreateNewGameButton;
	Layout* m_pButtonLayout;
	Image* m_pTitleImage;
	Player* m_pLastPlayer;
	std::vector<Player*> m_pPlayers;

};

}

#endif // _H_SYMPTOGEN_MENU_MANAGE_GAMES_MENU_H_