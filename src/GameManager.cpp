#include "GameManager.h"
#include "menu/PauseMenu.h"
#include "menu/Player.h"

#include <Indie.h>

namespace Symp {

GameManager::GameManager(const char *title, int width, int height, int bpp, bool vsync, bool fs, bool dBuffer){
	//Initialize the engine
	IndieLib::init(IND_DEBUG_MODE);
	m_pWindow = new Window();
	m_pRender = new Render();
	m_pWindow->setWindow(m_pRender->init(title, width, height, bpp, vsync, fs, dBuffer));
	m_pWindow->setCursor(true);
	m_pInputManager = new InputManager(m_pRender);
	m_pParser = new Parser("../assets/data.xml");
	//m_pSoundManager = new SoundManager();
	//m_pSoundManager->loadSound("../assets/audio/test.wav"); //test sound

	//Initialize the game elements to null and start the menu
	m_pMenuManager =  nullptr;
	m_pEntityManager = nullptr;
	m_pLevelManager = nullptr;
	switchToMenu();
}

GameManager::~GameManager(){
	IndieLib::end();
	delete m_pWindow;
	delete m_pRender;
	delete m_pInputManager;
	delete m_pEntityManager;
	delete m_pMenuManager;
}

/**
* @brief Displays the game when it needs to be
* This function displays the game, usually called from a menu.
* @see MenuManager
* @see updateGame()
* @see switchToMenu()
* @see GameManager()
* @see ~GameManager()
*/
void GameManager::switchToGame(){
	//Reset the menuManager attribut
	m_pMenuManager->setLevelChoosen(false);

	if (m_pEntityManager == nullptr && m_pLevelManager == nullptr) {
		//If no game have been created before then create a new one (from the main menu)
		m_pEntityManager = new EntityManager(m_pRender);
		m_pLevelManager = new LevelManager(m_pEntityManager);
		loadLevel(m_pMenuManager->getLevelToLoad().c_str());

	 	m_bIsInGame = true;
	}else{
		//Resume the game (from a pause menu)
		m_bIsInGame = true;
	}
}

/**
* @brief Clear the game entities and attributes for displaying the main menu
* @see MenuManager
* @see EntityManager
* @see LevelManager
* @see updateMenu()
* @see GameManager()
* @see ~GameManager()
*/
void GameManager::clear(){
	m_pEntityManager->deleteAllEntities();
	delete m_pEntityManager;
	delete m_pLevelManager;
	delete m_pMenuManager;
	m_pMenuManager = nullptr;
	m_pLevelManager = nullptr;
	m_pEntityManager = nullptr;
}

/**
* @brief Displays the menus when it needs to be
* This function displays wether the main menu, or the PauseMenu. This function can be called in game for displaying 
* again the main menus, in this case, the #GameManager needs to be cleared using the #clear() function. To display the
* game, see #switchToGame().
* @see MenuManager
* @see updateMenu()
* @see switchToGame()
* @see GameManager()
* @see ~GameManager()
*/
void GameManager::switchToMenu(){
	//If the MenuManager doesn't exists, means at the first launch or when the user quit the game, then create it.
	if (m_pMenuManager == nullptr){

		//Retrive data from the player data file
		std::pair<Player*, std::vector<Player*>> playerData = m_pParser->loadPlayerData();

		//Start the menus
		m_pMenuManager = new MenuManager(m_pRender, playerData);
 		m_bIsInGame = false;
 	
 	}else {

 		//Pause menu
 		PauseMenu* pPauseMenu = new PauseMenu(m_pMenuManager);
 		m_pMenuManager->setState(pPauseMenu);
 		m_bIsInGame = false;
 	}
}

/**
* @brief The main loop of the game responsible the application to run
* @see updateMenu()
* @see updateGame()
* @see switchToGame()
* @see switchToMenu()
* @see GameManager()
* @see ~GameManager()
*/
void GameManager::startMainLoop(){
	//If the user didn't closed the window or didn't clicked a "quit" button, then update
	while (!m_pMenuManager->isAboutToQuit() && !m_pInputManager->quit())
	{
		m_pInputManager->update();
 		if(m_bIsInGame) {
			updateGame();
		}
		else {
			updateMenu();
		}
	}
}

/**
* @brief Update the game part of the application each loop
* @see updateMenu()
* @see InputManager
* @see switchToGame()
* @see switchToMenu()
* @see EntityManager
* @see GameManager()
* @see ~GameManager()
*/
void GameManager::updateGame() {

	//update all list of entities
	m_pEntityManager->updateEntities();
	
	//sound
	if (m_pInputManager->onKeyPress(IND_SPACE)){
		m_pSoundManager->play(0);
	}

	//render openGL
	m_pRender->clearViewPort(60, 60, 60);
	m_pRender->beginScene();
	m_pEntityManager->renderEntities();
	m_pRender->endScene();

	//Pause
	if (m_pInputManager->onKeyPress(IND_ESCAPE)){
		switchToMenu();
	}
}

/**
* @brief Update the menu part of the application each loop
* @see updateGame()
* @see InputManager
* @see switchToGame()
* @see switchToMenu()
* @see EntityManager
* @see GameManager()
* @see ~GameManager()
*/
void GameManager::updateMenu() {

	//Forward inputs
	if (m_pInputManager->onKeyPress(IND_KEYDOWN)){
		m_pMenuManager->handleKeyPressed("KEYDOWN");
	}
	else if (m_pInputManager->onKeyPress(IND_KEYUP)){
		m_pMenuManager->handleKeyPressed("KEYUP");
	}
	else if (m_pInputManager->isMouseMotion()){
		// Mouse hover
		m_pMenuManager->handleMouseHover(m_pInputManager->getMouseX(), m_pInputManager->getMouseY());
	}
	else if(m_pInputManager->onMouseButtonPress(IND_MBUTTON_LEFT)){
		// Clic
		m_pMenuManager->handleMouseClic(m_pInputManager->getMouseX(), m_pInputManager->getMouseY());
	}
	else if (m_pInputManager->onKeyPress(IND_ESCAPE) && m_pMenuManager->isDisplayingPauseState()){
		// Hidding the Pause menu
		m_pMenuManager->setLevelChoosen(false);
		m_bIsInGame = true;
	}

	// The PauseMenu need not to refresh the window in order to displayed upon the game view
	if(!m_pMenuManager->isDisplayingPauseState()){
		m_pRender->clearViewPort(60, 60, 60);
	}
	// Otherwise, all the menus needs to refresh the window
	m_pRender->beginScene();
	m_pMenuManager->renderEntities();
	m_pRender->endScene();
	
	//Manage user decisions
	if(m_pMenuManager->isLevelChoosen()){
		// If the game part needs to be launch
		switchToGame();
		m_pMenuManager->clear();
	}else if (m_pMenuManager->isGoingBackToMenu() && m_pMenuManager->isDisplayingPauseState()){
		// If the user wants to go back to the main menu from the pause menu
		clear();
		switchToMenu();
	}
}

void GameManager::loadLevel(const char* mapFile) {
	m_pEntityManager->deleteAllEntities();
	m_pLevelManager->loadLevel(mapFile);
}

}
