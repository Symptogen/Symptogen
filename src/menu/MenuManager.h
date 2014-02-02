#ifndef _H_SYMPTOGEN_MENU_MENU_MANAGER_H_
#define _H_SYMPTOGEN_MENU_MENU_MANAGER_H_

#include <iostream>
#include <Indie.h>
#include "Player.h"
#include <IND_Entity2dManager.h>
#include "State.h"
#include "Layout.h"
#include "Button.h"
#include "Image.h"
#include "LineEdit.h"

namespace Symp {

/**
	Manager of the menus.
*/
class MenuManager {
public:
	MenuManager(Render* pRender, std::pair<Player*, std::vector<Player*>> playerData);
	~MenuManager();

	bool addGuiComponent(GuiComponent* guiComponent, int layer);
	void addGuiLayout(Layout* layout, int layer);
	void handleMouseClic(int mouseX, int mouseY);
	void handleKeyPressed(std::string key);
	void handleMouseHover(int mouseX, int mouseY);
	void renderEntities();
	void clear();
	void goBack();

	//Setters
	void setLevelToLoad(std::string sPath) {m_sLevelToLoad = sPath;}
	void setState(State* pState);
	void setLevelChoosen(bool bValue) { m_bIsLevelChoosen = bValue;}
	void setIsDisplayingPauseState( bool bValue) {m_bIsDisplayingPauseState = bValue;}

	//Getters
	std::vector<Player*> getPlayers() const {return m_playerVector;}
	std::string getLevelToLoad() const {return m_sLevelToLoad;}
	IND_Entity2dManager* getIND_Entity2dManager() const {return m_pEntity2dManager;}
	State* getCurrentState() const {return m_pCurrentState;}
	std::vector<GuiComponent*>	getGuiComponentArray() const {return m_guiComponentArray;}
	bool isLevelChoosen() const {return m_bIsLevelChoosen;}
	bool isDisplayingPauseState() const {return m_bIsDisplayingPauseState;}

private:
	State* m_pLastState;
	State* m_pCurrentState;
	IND_Entity2dManager* m_pEntity2dManager;
	std::vector<GuiComponent*> m_lastGuiComponentArray;
	std::vector<GuiComponent*>	m_guiComponentArray;
	bool m_bIsLevelChoosen;
	bool m_bIsDisplayingPauseState;
	std::string m_sLevelToLoad;
	std::vector<Player*> m_playerVector;
};

}

#endif //_H_SYMPTOGEN_MENU_MENU_MANAGER_H_