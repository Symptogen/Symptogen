#include "MenuManager.h"
#include "WelcomeUnknownMenu.h"
#include "WelcomeLastPlayerMenu.h"

#include <Indie.h>
#ifdef _WIN32
#include <string>
#endif

/** @namespace Symp */
namespace Symp {

unsigned int gTotalLevelNumber = 10;

/**
* @brief MenuManager constructor. 
* Responsible for the initialization of the private attributes of MenuManager class and for the choice between two 
* start menus available to be displayed.
* @param pRender is the reference to the renderer that is the only one able to display menus
* @param playerData is the result of the data's parsing that contain the last player, and the list of all the players
* @see Parser::loadPlayerData()
* @see ~MenuManager()
*/
MenuManager::MenuManager(){
	m_bIsLevelChoosen = false;
	m_bIsDisplayingPauseState = false;
	m_bIsGoingBackToMenu = false;
	m_bIsAboutToQuit = false;
	m_pEntity2dManager = new IND_Entity2dManager();
}

/**
* @brief MenuManager destructor. 
* Responsible for the destruction of the main attributes of the MenuManager class, and the suppression of the 
* Indielib Entity2DManager.
* @see MenuManager()
*/
MenuManager::~MenuManager() {
	m_pEntity2dManager->end();
    DISPOSE(m_pEntity2dManager);
    GuiComponent::end();
	//delete m_pCurrentState;
}

void MenuManager::init(Render* pRender, std::pair<Player*, std::vector<Player*>> playerData) {
	
	m_playerArray = playerData.second;
	m_pLastPlayer = playerData.first;

	m_pEntity2dManager->init(pRender->getIND_Render());
	GuiComponent::init(pRender);
	
	// Choose the menu to display following the presence or not of a player in data
	if (m_playerArray.empty()){
		//Case no players have been registrated
		WelcomeUnknownMenu* welcomeMenu = new WelcomeUnknownMenu();
		MenuManager::getInstance()->setState(welcomeMenu);
	}else {
		//Case a player at least have been found in data
		WelcomeLastPlayerMenu* welcomeLastPlayerMenu = new WelcomeLastPlayerMenu(m_playerArray[0]);
		MenuManager::getInstance()->setState(welcomeLastPlayerMenu);
	}
}

/**
* @brief Function that erase the Indielib components. 
* When the game world and components needs to be displayed, the Indielib menu components have to be removed in order 
* not to be displayed anymore on the screen.
* @see GameManager::updateMenu()
* @see MenuManager::m_guiComponentArray
* @see MenuManager::m_pEntity2dManager
*/
void MenuManager::clear() {
	for (unsigned int i=0; i < m_guiComponentArray.size(); ++i){
		m_pEntity2dManager->remove(m_guiComponentArray[i]->getIND_Entity2d());
	}
	m_guiComponentArray.clear();
	m_lastGuiComponentArrays.clear();
	m_pLastStates.clear();
	delete m_pCurrentState;
}

/**
* @brief Add a component to the Indielib and MenuManager containers
* When a menu needs to render a component, the component is added by this function in both containers of the MenuManager
* and the Indielib library : the first to store historic and the second to pass to the renderer.
* @param pGuiComponent is the reference to the component, inherited from GuiComponent class, to be added
* @param iLayer is the int needed by Indielib to seperate 2dEntities into layers, starting at 0. The values availables 
* are shown in the renderEntities method.
* @see MenuManager::m_guiComponentArray
* @see MenuManager::m_pEntity2dManager
* @see MenuManager::renderEntities()
* @see MenuManager()
* @return bool value standing for the success of the addition in the Indielib container IND_Entity2dManager
*/
bool MenuManager::addGuiComponent(GuiComponent* pGuiComponent, int iLayer){
	m_guiComponentArray.push_back(pGuiComponent);
	return m_pEntity2dManager->add(iLayer, pGuiComponent->getIND_Entity2d());
}

/**
* @brief Add a layout and its components to the Indielib and MenuManager containers
* A Layout contains multiple GuiComponents, and all need to be added to the IndieLib and MenuManager containers.
* @param pLayout is the reference to the layout to be added
* @param iLayer is the int needed by Indielib to seperate 2dEntities into layers, starting at 0; 1 is a goog value because Layout
* need to be displayed under its components. The values availables are shown in the renderEntities method.
* @see MenuManager::renderEntities()
* @see MenuManager::addGuiComponent()
* @see MenuManager()
*/
void MenuManager::addGuiLayout(Layout* pLayout, int iLayer){
	//If multiple layers, then render the layout under its components
	if (iLayer > 0){
		addGuiComponent(pLayout, iLayer-1);
	}
	for (unsigned int i = 0; i < pLayout->getComponents().size(); ++i){
		addGuiComponent(pLayout->getComponents()[i], iLayer);
	}
}

/**
* @brief Change the current state of the MenuManager
* As the MenuManager is part of a state machine pattern, a menu is a state and displaying a new menu means changing
* current state. The old state needs to be removed. When the new state is settled as the current state, the state is initialized and its components added
* to the MenuManager and Indielib containers. The history can be traced back with the goBack method.
* @param pState is the reference to the new State to be displayed
* @see MenuManager::m_lastGuiComponentArrays
* @see MenuManager::m_pLastStates
* @see MenuManager::goBack()
* @see State::init()
* @see MenuManager()
*/
void MenuManager::setState(State* pState){
	// Remove the ancients components from the Indielib container for them not to be rendered anymore
	for (unsigned int i=0; i < m_guiComponentArray.size(); ++i){
		m_pEntity2dManager->remove(m_guiComponentArray[i]->getIND_Entity2d());
	}
	// Update the deque that store the history of the menus
	// Note : 3 is the max depth that the user can reach in the actual menu scheme, so no need to store more history
	if (m_lastGuiComponentArrays.size() == 3 && m_pLastStates.size() == 3){
		m_lastGuiComponentArrays.pop_front();
		m_pLastStates.pop_front();
	}

	// Add in queue of the history the new state and its components 
	m_lastGuiComponentArrays.push_back(m_guiComponentArray);
	m_pLastStates.push_back(m_pCurrentState);

	// Clear the actual component container and init the state
	m_pCurrentState = pState;
	m_guiComponentArray.clear();
	m_pCurrentState->init();
}

/**
* @brief Retrieve the previous state the MenuManager was in
* As the MenuManager is part of a state machine pattern, a menu is a state and displaying a new menu means changing
* current state. The history of menus is stored and can be traced back, without changing the history itself. The
* opposite of the goBack method is the setState method that update the history.
* @see MenuManager::m_lastGuiComponentArrays
* @see MenuManager::m_pLastStates
* @see MenuManager::setState()
* @see State::init()
* @see MenuManager()
*/
void MenuManager::goBack() {
	// Remove the ancients components from the Indielib container for them not to be rendered anymore
	for (unsigned int i=0; i < m_guiComponentArray.size(); ++i){
		m_pEntity2dManager->remove(m_guiComponentArray[i]->getIND_Entity2d());
	}

	// Place the values of the previous historical state in the current attributes
	m_guiComponentArray.swap(m_lastGuiComponentArrays.back());
	m_pCurrentState = m_pLastStates.back();

	// Suppress the last (and more recent that the current) state, means reducing the history stack size
	m_lastGuiComponentArrays.pop_back();
	m_pLastStates.pop_back();

	// Init the state known now as current
	m_pCurrentState->init();

}

/**
* @brief Command to the Indielib library which layers of components are displayed
* When a component is added to the Indielib IND_Entity2dManager, it is placed in a layer. A layer can group
* component and can be displayed or not.
* @see MenuManager::addGuiComponent()
* @see MenuManager::m_pEntity2dManager
* @see MenuManager()
*/
void MenuManager::renderEntities(){
	m_pEntity2dManager->renderEntities2d(0);
	m_pEntity2dManager->renderEntities2d(1);
	m_pEntity2dManager->renderEntities2d(2);
}

/**
* @brief Handle the mouse hover event
* The GameManager pass the mouve hover to the MenuManager if the menus are currently displayed. The MenuManager owns 
* the GuiComponents currently displayed by its actual state, and can tell them if there are hovered or not. That can
* make the GuiComponents update their appearence. This event is the only one that is not passed to the current state.
* @see MenuManager::m_guiComponentArray
* @see GuiComponent::GuiComponent()
* @see GameManager::updateMenu()
* @see MenuManager()
*/
void MenuManager::handleMouseHover(int mouseX, int mouseY) {
	// Retrieve all the components currently on the screen
	for (unsigned int i =0; i < m_guiComponentArray.size(); ++i) {
		// Set all values to false to reset components
		m_guiComponentArray[i]->setHovered(false);
		// Check if the component is under the mouse
		if (m_guiComponentArray[i]->isTargetedByMouse(mouseX, mouseY) && m_guiComponentArray[i]->isEnabled()){
			m_guiComponentArray[i]->setHovered(true);
		}
	}
}

/**
* @brief Handle the mouse clic event
* The GameManager pass the mouve clic to the MenuManager if the menus are currently displayed. The MenuManager
* passes the event to its current state that manage the response to the event.
* @see State::handleMouseClic()
* @see GameManager::updateMenu()
* @see MenuManager()
*/
void MenuManager::handleMouseClic(int mouseX, int mouseY){
	m_pCurrentState->handleMouseClic(mouseX, mouseY);
}

/**
* @brief Handle the key press event
* The GameManager pass the key press to the MenuManager if the menus are currently displayed. The MenuManager
* passes the event to its current state that manage the response to the event.
* @see State::keyDownPressed()
* @see State::keyUpPressed()
* @see GameManager::updateMenu()
* @see MenuManager()
*/
void MenuManager::handleKeyPressed(std::string key){
	if(key == "KEYDOWN" ){
		m_pCurrentState->keyDownPressed();
	}
	else if (key == "KEYUP"	){
		m_pCurrentState->keyUpPressed();
	}
}

}