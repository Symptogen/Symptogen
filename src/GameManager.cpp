#include "GameManager.h"

#include <Indie.h>

namespace Symp {

GameManager::GameManager(const char *title, int width, int height, int bpp, bool vsync, bool fs, bool dBuffer){
	IndieLib::init(IND_DEBUG_MODE);
	m_pWindow = new Window();
	m_pRender = new Render();
	m_pWindow->setWindow(m_pRender->init(title, width, height, bpp, vsync, fs, dBuffer));
	m_pWindow->setCursor(true);
	m_pInputManager = new InputManager(m_pRender);
	//m_pSoundManager = new SoundManager();
	//m_pSoundManager->loadSound("../assets/audio/test.wav"); //test sound
 	m_pEntityManager = new EntityManager(m_pRender);
	m_pLevelManager = new LevelManager(m_pEntityManager);

 	m_bIsInGame = true;
}

GameManager::~GameManager(){
	IndieLib::end();
	delete m_pWindow;
	delete m_pRender;
	delete m_pInputManager;
	delete m_pEntityManager;
}

void GameManager::startGame(){
	// ----- Main Loop -----
	while (!m_pInputManager->onKeyPress (IND_ESCAPE) && !m_pInputManager->quit())
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
	/*if (m_pInputManager->isKeyPressed(IND_SPACE)){
		m_pSoundManager->play(0);
	}*/

	//render openGL
	m_pRender->clearViewPort(60, 60, 60);
	m_pRender->beginScene();
	m_pEntityManager->renderEntities();
	m_pRender->endScene();
}

void GameManager::updateMenu() {

}

void GameManager::loadLevel(const char* mapFile) {
	m_pEntityManager->deleteAllEntities();
	m_pLevelManager->loadLevel(mapFile);
}

}
