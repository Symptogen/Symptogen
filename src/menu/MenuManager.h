#ifndef _H_SYMPTOGEN_MENU_MENU_MANAGER_H_
#define _H_SYMPTOGEN_MENU_MENU_MANAGER_H_

#include <iostream>
#include <deque>
#include <Indie.h>
#include "Player.h"
#include <IND_Entity2dManager.h>
#include "State.h"
#include "Layout.h"
#include "Button.h"
#include "Image.h"
#include "LineEdit.h"
#include "Slider.h"

namespace Symp {

extern unsigned int gTotalLevelNumber;

/**
* MenuManager class. This class is responsible for the creation and the updating of the different menus that
* can be displayed in the application. The MenuManager is part of the state machine pattern settled for the menu
* management. The communication between the logic part of the menu and the renderer / inputs is made by the
* GameManager class that knows the MenuManager. The communication between these two class in one-sided : the
* GameManager checks few values of the MenuManager each loop, in order to know what actions have been done by
* the user and to react in response.
* @see GameManager
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
	void setLevelToLoad( std::string sPath ) {m_sLevelToLoad = sPath;}
	void setState( State* pState);
	void setLevelChoosen( bool bValue ) { m_bIsLevelChoosen = bValue;}
	void setIsDisplayingPauseState( bool bValue ) {m_bIsDisplayingPauseState = bValue;}
	void setQuitGameChoosen( bool value ) {m_bIsQuitGameChoosen = value;}

	//Getters
	Player* getLastPlayer() const {return m_pLastPlayer;}
	std::vector<Player*> getPlayers() const {return m_playerArray;}
	std::string getLevelToLoad() const {return m_sLevelToLoad;}
	IND_Entity2dManager* getIND_Entity2dManager() const {return m_pEntity2dManager;}
	State* getCurrentState() const {return m_pCurrentState;}
	std::vector<GuiComponent*>	getGuiComponentArray() const {return m_guiComponentArray;}
	bool isLevelChoosen() const {return m_bIsLevelChoosen;}
	bool isDisplayingPauseState() const {return m_bIsDisplayingPauseState;}
	bool isQuitGameChoosen() const {return m_bIsQuitGameChoosen;}

private:
	Player* m_pLastPlayer; /**< store the last known player of the application */
	std::deque<State*> m_pLastStates; /**< store the previous displayed states, in case the user needs to go back to this state */
	State* m_pCurrentState; /**< the state currently displayed */
	IND_Entity2dManager* m_pEntity2dManager; /**< the reference to the Indielib Entity2DManager */
	std::deque<std::vector<GuiComponent*>> m_lastGuiComponentArrays; /**< deque of the GuiComponents that composed the previous states */
	std::vector<GuiComponent*>	m_guiComponentArray; /**< vector of the GuiComponents that compose the current state */
	bool m_bIsLevelChoosen; /**< boolean that indicates the GameManager if it needs to switch to the game or not */
	bool m_bIsDisplayingPauseState; /**< boolean that indicates the GameManager to not handle the PauseMenu like the others */
	bool m_bIsQuitGameChoosen; /**< boolean that indicates the GameManager to quit the current game and display the menus */
	std::string m_sLevelToLoad; /**< the filename of the level to be loaded */
	std::vector<Player*> m_playerArray; /**< vector that stores all the registered players */
};

}

#endif //_H_SYMPTOGEN_MENU_MENU_MANAGER_H_