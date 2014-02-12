#ifndef _H_SYMPTOGEN_MENU_WELCOME_LAST_PLAYER_MENU_H_
#define _H_SYMPTOGEN_MENU_WELCOME_LAST_PLAYER_MENU_H_

#include "State.h"
#include "MenuManager.h"


namespace Symp {
/**
	WelcomeLAST_PLAYERMenu inherited from the State interface.
*/

class WelcomeLastPlayerMenu : public State {
public:
	WelcomeLastPlayerMenu(Player* lastPlayer, MenuManager* pMenuManager);
	~WelcomeLastPlayerMenu();

	virtual void init();
	virtual void handleMouseClic(int mouseX, int mouseY);
	virtual void keyDownPressed();
	virtual void keyUpPressed();

	//Getter
	Player* getLastPlayer() const {return m_pLastPlayer;}

private:
	Button* m_resumeGameButton;
	Button* m_quitButton;
	Button* m_manageGamesButton;
	Layout* m_buttonLayout;
	Image* m_titleImage;
	Player* m_pLastPlayer;

};

}

#endif // _H_SYMPTOGEN_MENU_WELCOME_LAST_PLAYER_MENU_H_