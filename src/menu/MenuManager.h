#ifndef _H_SYMPTOGEN_MENU_MENU_MANAGER_H_
#define _H_SYMPTOGEN_MENU_MENU_MANAGER_H_

#include <iostream>
#include <Indie.h>
#include <IND_Entity2dManager.h>
#include "State.h"
#include "Layout.h"
#include "Button.h"
#include "Image.h"

namespace Symp {

/**
	Manager of the menus.
*/
class MenuManager {
public:
	MenuManager(Render* pRender);
	~MenuManager();

	bool addGuiComponent(GuiComponent* guiComponent, int layer);
	void addGuiLayout(Layout* layout, int layer);
	void handleMouseClic(int mouseX, int mouseY);
	void handleKeyPressed(std::string key);
	void handleMouseHover(int mouseX, int mouseY);
	void renderEntities();
	void clear();

	//Setters
	void setState(State* pState) {m_pCurrentState = pState;}
	void setLevelChoosen(bool bValue) { m_bIsLevelChoosen = bValue;}
	void setIsDisplayingPauseState( bool bValue) {m_bIsDisplayingPauseState = bValue;}

	//Getters
	IND_Entity2dManager* getIND_Entity2dManager() const {return m_pEntity2dManager;}
	State* getCurrentState() const {return m_pCurrentState;}
	std::vector<GuiComponent*>	getGuiComponentArray() const {return m_guiComponentArray;}
	bool isLevelChoosen() const {return m_bIsLevelChoosen;}
	bool isDisplayingPauseState() const {return m_bIsDisplayingPauseState;}

private:
	State* m_pCurrentState;
	IND_Entity2dManager* m_pEntity2dManager;
	std::vector<GuiComponent*>	m_guiComponentArray;
	bool m_bIsLevelChoosen;
	bool m_bIsDisplayingPauseState;

};

}

#endif //_H_SYMPTOGEN_MENU_MENU_MANAGER_H_