#ifndef _H_SYMPTOGEN_MENU_PAUSE_MENU_H_
#define _H_SYMPTOGEN_MENU_PAUSE_MENU_H_

#include "State.h"
#include "MenuManager.h"


namespace Symp {

class PauseMenu : public State {
public:
	PauseMenu(MenuManager* pMenuManager);
	~PauseMenu();

	void init();
	virtual void handleMouseClic(int mouseX, int mouseY);
	virtual void keyDownPressed();
	virtual void keyUpPressed();

private:
	Button* m_resumeGameButton;
	Button* m_optionsButton;
	Button* m_backToMenuButton;
	Layout* m_buttonLayout;
};

}
#endif // _H_SYMPTOGEN_MENU_PAUSE_MENU_H_