#ifndef _H_SYMPTOGEN_MENU_MENU_MANAGER_H_
#define _H_SYMPTOGEN_MENU_MENU_MANAGER_H_

#include <iostream>
#include <Indie.h>
#include <IND_Entity2dManager.h>
#include "State.h"

namespace Symp {

/**
	Manager of the menus.
*/
class MenuManager {
public:
	MenuManager(Render* pRender);
	~MenuManager();

	bool addGuiComponent(GuiComponent* guiComponent, int layer);
	void handleKeyPressed(std::string key);
	void renderEntities();

	//Setters
	void setState(State* pState) {m_pCurrentState = pState;}

	//Getters
	IND_Entity2dManager* getIND_Entity2dManager() const {return m_pEntity2dManager;}
	State* getCurrentState() const {return m_pCurrentState;}
	std::vector<GuiComponent*>	getGuiComponentArray() const {return m_guiComponentArray;}

private:
	State* m_pCurrentState;
	IND_Entity2dManager* m_pEntity2dManager;
	std::vector<GuiComponent*>	m_guiComponentArray;

};

}

#endif //_H_SYMPTOGEN_MENU_MENU_MANAGER_H_