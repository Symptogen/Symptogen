#ifndef _H_SYMPTOGEN_MENU_NEW_GAME_MENU_H_
#define _H_SYMPTOGEN_MENU_NEW_GAME_MENU_H_

#include "State.h"
#include "MenuManager.h"


namespace Symp {
/**
	NewGameMenu inherited from the State interface.
*/

class NewGameMenu : public State {
public:
	NewGameMenu(MenuManager* pMenuManager);
	~NewGameMenu();

	virtual void init();
	virtual void handleMouseClic(int mouseX, int mouseY);
	virtual void keyDownPressed();
	virtual void keyUpPressed();

private:
	Button* m_pBackButton;
	Button* m_pLaunchButton;
	Layout* m_pButtonLayout;
	Layout* m_pArrowLayout;
	Button* m_pLeftArrow;
	Button* m_pRightArrow;
	Image* m_pTitleImage;
	LineEdit* m_pLineEdit;
	std::vector<Image*> m_avatarVector;
	Image* m_pCurrentAvatar;
	std::string m_sName;

};

}

#endif // _H_SYMPTOGEN_MENU_NEW_GAME_MENU_H_