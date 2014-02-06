#ifndef _H_SYMPTOGEN_MENU_CHOOSE_YOUR_LEVEL_MENU_H_
#define _H_SYMPTOGEN_MENU_CHOOSE_YOUR_LEVEL_MENU_H_

#include "State.h"
#include "MenuManager.h"


namespace Symp {
/**
	WelcomeLAST_PLAYERMenu inherited from the State interface.
*/

class ChooseYourLevelMenu : public State {
public:
	ChooseYourLevelMenu(Player* pPlayer, MenuManager* pMenuManager);
	~ChooseYourLevelMenu(){}

	virtual void init();
	virtual void handleMouseClic(int mouseX, int mouseY);
	virtual void keyDownPressed();
	virtual void keyUpPressed();

	//Getter
	Player* getPlayer() const {return m_pPlayer;}


private:
	Player* m_pPlayer;
	Image* m_pChooseLabel;
	Button* m_pBackButton;
	Button* m_pLevel1Button;
	Layout* m_pPlayerLayout;
	Layout* m_pButtonLayout;
	Image* m_pTitleImage;

};

}

#endif // _H_SYMPTOGEN_MENU_CHOOSE_YOUR_LEVEL_MENU_H_