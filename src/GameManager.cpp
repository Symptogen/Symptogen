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

	//Start the menus
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
* Function to initialize the level datas and the game once the user is done with the main menus.
* @see GameManager::GameManager
* @see MenuManager
*/
void GameManager::switchToGame(){
	m_pMenuManager->setLevelChoosen(false);
	if (m_pEntityManager == nullptr && m_pLevelManager == nullptr) {
		m_pEntityManager = new EntityManager(m_pRender);
		m_pLevelManager = new LevelManager(m_pEntityManager);

		//m_pEntityManager->loadTestWorld();
		loadLevel(m_pMenuManager->getLevelToLoad().c_str());

	 	m_bIsInGame = true;
	}else{
		m_bIsInGame = true;
	}
}

/**
* Function to initialize the menus when the user is in game or at the first launch of the application.
* @see GameManager::GameManager
* @see MenuManager
*/
void GameManager::switchToMenu(){
	if (m_pMenuManager == nullptr){

		//Retrive data from the player data file
		std::pair<Player*, std::vector<Player*>> playerData = m_pParser->loadPlayerData();

		//Start app by the menus
		m_pMenuManager = new MenuManager(m_pRender, playerData);
 		m_bIsInGame = false;
 	}else {
 		//Pause menu
 		PauseMenu* pPauseMenu = new PauseMenu(m_pMenuManager);
 		m_pMenuManager->setState(pPauseMenu);
 		m_bIsInGame = false;
 	}
}

void GameManager::startMainLoop(){
	// ----- Main Loop -----
	while (!m_pInputManager->onKeyPress(IND_ESCAPE) && !m_pInputManager->quit())
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
	if (m_pInputManager->isKeyPressed(IND_P)){
		switchToMenu();
	}
}

void GameManager::updateMenu() {

	//Forward inputs
	if (m_pInputManager->onKeyPress(IND_KEYDOWN)){
		m_pMenuManager->handleKeyPressed("KEYDOWN");
	}
	else if (m_pInputManager->onKeyPress(IND_KEYUP)){
		m_pMenuManager->handleKeyPressed("KEYUP");
	}
	else if (m_pInputManager->isMouseMotion()){
		m_pMenuManager->handleMouseHover(m_pInputManager->getMouseX(), m_pInputManager->getMouseY());
	}
	else if(m_pInputManager->onMouseButtonPress(IND_MBUTTON_LEFT)){
		m_pMenuManager->handleMouseClic(m_pInputManager->getMouseX(), m_pInputManager->getMouseY());
	}

	if(m_pMenuManager->isDisplayingPauseState()){
		m_pRender->beginScene();
		m_pMenuManager->renderEntities();
		m_pRender->endScene();
	}
	else {
		//render openGL
		m_pRender->clearViewPort(60, 60, 60);
		m_pRender->beginScene();
		m_pMenuManager->renderEntities();
		m_pRender->endScene();
	}

	if(m_pMenuManager->isLevelChoosen()){
		switchToGame();
		m_pMenuManager->clear();
	}
}

void GameManager::loadLevel(const char* mapFile) {
	m_pEntityManager->deleteAllEntities();
	m_pLevelManager->loadLevel(mapFile);
}

}
