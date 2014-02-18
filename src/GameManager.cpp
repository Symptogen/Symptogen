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
	m_pLevelManager = nullptr;
	switchToMenu();
}

GameManager::~GameManager(){
	IndieLib::end();
	delete m_pWindow;
	delete m_pRender;
	delete m_pInputManager;
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

	EntityManager::getEntityManagerInstance();
	EntityManager::initRender(m_pRender);

	if (m_pLevelManager == nullptr) {
		//If no game have been created before then create a new one (from the main menu)
		m_pLevelManager = new LevelManager(EntityManager::getEntityManagerInstance());
		loadLevel(m_pMenuManager->getLevelToLoad().c_str());
	 	m_bIsInGame = true;
	}
	else{
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
	EntityManager::getEntityManagerInstance()->deleteAllEntities();
	delete m_pLevelManager;
	delete m_pMenuManager;
	m_pMenuManager = nullptr;
	m_pLevelManager = nullptr;
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
 		//manage Camera
		m_pRender->setCamera();
 		m_pRender->setCameraPosition(m_pWindow->getIND_Window()->getWidth()*0.5, m_pWindow->getIND_Window()->getHeight()*0.5);
 	
 	}else {

 		//Pause menu
 		RenderEntity* pDino = EntityManager::getEntityManagerInstance()->getRenderDino();
 		PauseMenu* pPauseMenu = new PauseMenu(m_pMenuManager, pDino->getPosX(), pDino->getPosY());
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
	//move dino
	PhysicalEntity* pDino = EntityManager::getEntityManagerInstance()->getPhysicalDino();
		float impulse = pDino->getMass() * 1;
	if (m_pInputManager->isKeyPressed(IND_KEYLEFT)){
		pDino->getb2Body()->ApplyLinearImpulse(b2Vec2(-impulse, 0.f), pDino->getb2Body()->GetWorldCenter(), pDino->isAwake());
	}
	if (m_pInputManager->isKeyPressed(IND_KEYRIGHT)){
		pDino->getb2Body()->ApplyLinearImpulse(b2Vec2(impulse, 0.f), pDino->getb2Body()->GetWorldCenter(), pDino->isAwake());
	}
	if (m_pInputManager->isKeyPressed(IND_KEYUP)){
		pDino->getb2Body()->ApplyLinearImpulse(b2Vec2(0.f, -impulse), pDino->getb2Body()->GetWorldCenter(), pDino->isAwake());
	}
	if (m_pInputManager->isKeyPressed(IND_KEYDOWN)){
		pDino->getb2Body()->ApplyLinearImpulse(b2Vec2(0.f, impulse), pDino->getb2Body()->GetWorldCenter(), pDino->isAwake());
	}

	//manage Camera
	m_pRender->setCamera();
	m_pRender->setCameraPosition(pDino->getPosition().x, pDino->getPosition().y);

	//update all list of entities
	EntityManager::getEntityManagerInstance()->updateEntities();
	
	//sound
	if (m_pInputManager->isKeyPressed(IND_SPACE)){
		//m_pSoundManager->play(0); //seg fault : still no song !
	}

	//render openGL
	m_pRender->clearViewPort(60, 60, 60);
	m_pRender->beginScene();
	EntityManager::getEntityManagerInstance()->renderEntities();
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
	//manage camera
	m_pRender->setCamera();

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
		m_pRender->setCameraPosition(m_pWindow->getIND_Window()->getWidth()*0.5, m_pWindow->getIND_Window()->getHeight()*0.5);
		clear();
		switchToMenu();
	}
}

void GameManager::loadLevel(const char* mapFile) {
	EntityManager::getEntityManagerInstance()->deleteAllEntities();
	EntityManager::getEntityManagerInstance()->addDino();
	m_pLevelManager->loadLevel(mapFile);
}

}
