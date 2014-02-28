#include <Indie.h>

#include "GameManager.h"
#include "menu/PauseMenu.h"
#include "menu/Player.h"
#include "power/Power.h"
#include "power/Sneeze.h"

#define DEATH_VELOCITY 120

namespace Symp {

GameManager::GameManager(const char *title, int width, int height, int bpp, bool vsync, bool fs, bool dBuffer) {
	IndieLib::init(IND_DEBUG_MODE);
	m_pWindow = new Window();
	m_pRender = new Render();
	m_pWindow->setWindow(m_pRender->init(title, width, height, bpp, vsync, fs, dBuffer));
	//m_pRender->toggleFullScreen();
	m_pWindow->setCursor(true);

	InputManager::getInstance();
	InputManager::initRender(m_pRender);

	m_pParser = new Parser("../assets/data.xml");

	m_pLevelManager = nullptr;
	m_bIsMenu = false;
	m_bIsLevelFinished = false;

	// Set the levels order
	m_levelList.push_back("../assets/map/level1.xml");
	m_levelList.push_back("../assets/map/level2.xml");

	switchToMenu();
}

GameManager::~GameManager() {
	IndieLib::end();
	if(m_pRender->isFullScreen()) {
		m_pRender->toggleFullScreen();
	}
	delete m_pWindow;
	delete m_pRender;
	InputManager::removeInstance();
	MenuManager::removeInstance();
}

void GameManager::clear() {
	EntityManager::getInstance()->deleteAllEntities();
	delete m_pLevelManager;
	MenuManager::removeInstance();
	m_pLevelManager = NULL;
	EntityManager::removeInstance();
	m_bIsMenu = false;
	
		
}

void GameManager::startMainLoop(){
	//If the user didn't closed the window or didn't clicked a "quit" button, then update
	while (!MenuManager::getInstance()->isAboutToQuit() && !InputManager::getInstance()->quit())
	{
		InputManager::getInstance()->update();
 		if(m_bIsInGame) {
			updateGame();
		}
		else {
			updateMenu();
		}
	}
}

void GameManager::updateGame() {

	/******************/
	/*   Move Dino    */
	/******************/
	
	PhysicalEntity* pDino = EntityManager::getInstance()->getPhysicalDino();
    std::vector<SoundEntity*> sDinoArray = EntityManager::getInstance()->getSoundDino();

	float forceFactor = 10.f;
	float impulse = pDino->getMass() * forceFactor;

	// Left
	if (InputManager::getInstance()->isKeyPressed(IND_KEYLEFT) && !pDino->isContactingLeft()) {
		// Physics
		pDino->getb2Body()->ApplyLinearImpulse(b2Vec2(-impulse, 0.f), pDino->getb2Body()->GetWorldCenter(), pDino->isAwake());
		// Render : flip to the left all render entities
		for(size_t i = 0; i < EntityManager::getInstance()->getRenderDino().size(); ++i) {
			if(EntityManager::getInstance()->getRenderDino().at(i) != NULL)
				EntityManager::getInstance()->getRenderDino().at(i)->flipHorizontaly(true);
		}
		EntityManager::getInstance()->updateDinoRender(DinoAction::Walk);
	}
	// Right
	if (InputManager::getInstance()->isKeyPressed(IND_KEYRIGHT) && !pDino->isContactingRight()) {
		// Physics
		pDino->getb2Body()->ApplyLinearImpulse(b2Vec2(impulse, 0.f), pDino->getb2Body()->GetWorldCenter(), pDino->isAwake());
		// Render : flip to the right all render entities
		for(size_t i = 0; i < EntityManager::getInstance()->getRenderDino().size(); ++i) {
			if(EntityManager::getInstance()->getRenderDino().at(i) != NULL)
				EntityManager::getInstance()->getRenderDino().at(i)->flipHorizontaly(false);
		}
		EntityManager::getInstance()->updateDinoRender(DinoAction::Walk);
	}
	// Up
	if (InputManager::getInstance()->isKeyPressed(IND_KEYUP) && pDino->getNumContacts() > 0 && pDino->isContactingBelow()) {
		// Physics
		float force = impulse / (1/60.0); //f = mv/t
	    pDino->getb2Body()->ApplyLinearImpulse(b2Vec2(pDino->getLinearVelocity().x, -force), pDino->getb2Body()->GetWorldCenter(), pDino->isAwake());
	    // Sound
		SoundManager::getInstance()->play(sDinoArray[DinoAction::Jump]->getIndexSound());
	}
	if (InputManager::getInstance()->isKeyPressed(IND_KEYDOWN)) {
		// Physics
		pDino->getb2Body()->ApplyLinearImpulse(b2Vec2(0.f, impulse), pDino->getb2Body()->GetWorldCenter(), pDino->isAwake());
	}

	// If no movements
	if(EntityManager::getInstance()->getPhysicalDino()->getLinearVelocity().x == 0) {
		EntityManager::getInstance()->updateDinoRender(DinoAction::Stop);
	}


	/***********/
	/*  Death  */
	/***********/

	// std::cout << "Velocity : " << pDino->getLinearVelocity().x << " - " << pDino->getLinearVelocity().y << std::endl;
	// if(pDino->getLinearVelocity().y >= DEATH_VELOCITY) {
	// 	std::cout << "Tou est mort ! ;) " << std::endl;
	// 	EntityManager::getInstance()->killDino(DinoAction::Die);

	// 	loadLevel(MenuManager::getInstance()->getLevelToLoad().c_str());

	// }

	/****************************/
	/*  Camera zoom (for debug) */
	/****************************/

	if (InputManager::getInstance()->isKeyPressed(IND_S)){
		float newZoom = m_pRender->getZoom()+0.005;
		m_pRender->setZoom(newZoom);
	}
	else if (InputManager::getInstance()->isKeyPressed(IND_Q)){
		float newZoom = m_pRender->getZoom()-0.005;
		m_pRender->setZoom(newZoom);
	}
	else if (InputManager::getInstance()->isKeyPressed(IND_D)){
		m_pRender->setZoom(1);
	}

	/********************/
	/* Camera movements */
	/********************/

	m_pRender->setCamera();
	m_pRender->setCameraPosition(pDino->getPosition().x, pDino->getPosition().y);
	
	/********************/
	/*  Update entities */
	/********************/

	EntityManager::getInstance()->updateEntities();

	/********************/
	/*     Powers       */
	/********************/

	EntityManager::getInstance()->executePowers();

	/********************/
	/*  Manage render   */
	/********************/

	m_pRender->clearViewPort(60, 60, 60);
	m_pRender->beginScene();
		EntityManager::getInstance()->renderEntities();
		//test hitbox
		debugPhysicalEntities();
		//debugRenderEntities();
	m_pRender->endScene();

	/********************/
	/*      Pause       */
	/********************/

	if (InputManager::getInstance()->onKeyPress(IND_ESCAPE)) {
		switchToMenu();
	}

	/***********************/
	/*  Detect level exit  */
	/***********************/

	float exitX = EntityManager::getInstance()->getExitCoordinates()[0];
	float exitY = EntityManager::getInstance()->getExitCoordinates()[1];

	if(abs(exitX - pDino->getPosition().x) < 10 && abs(exitY - pDino->getPosition().y) < 10) {
		fprintf(stderr, "You finished this level !\n");
		m_bIsLevelFinished = true;
		switchToGame();
	}
}

void GameManager::updateMenu() {
	//Forward inputs

	//The following offsets are necessary to the mouse pointer to click on the pause menu
	//because it's not the same coordinate space
	int offsetX = 0;
	int offsetY = 0;
	if(MenuManager::getInstance()->isDisplayingPauseState()){
		PhysicalEntity* pDino = EntityManager::getInstance()->getPhysicalDino();
		offsetX = pDino->getPosition().x - m_pWindow->getIND_Window()->getWidth()*0.5;
		offsetY = pDino->getPosition().y - m_pWindow->getIND_Window()->getHeight()*0.5;
	}

	if (InputManager::getInstance()->onKeyPress(IND_KEYDOWN)){
		MenuManager::getInstance()->handleKeyPressed("KEYDOWN");
	}
	else if (InputManager::getInstance()->onKeyPress(IND_KEYUP)){
		MenuManager::getInstance()->handleKeyPressed("KEYUP");
	}
	else if (InputManager::getInstance()->isMouseMotion()){
		// Mouse hover
		MenuManager::getInstance()->handleMouseHover(InputManager::getInstance()->getMouseX()+offsetX, InputManager::getInstance()->getMouseY()+offsetY);
	}
	else if(InputManager::getInstance()->onMouseButtonPress(IND_MBUTTON_LEFT)){
		// Clic
		MenuManager::getInstance()->handleMouseClic(InputManager::getInstance()->getMouseX()+offsetX, InputManager::getInstance()->getMouseY()+offsetY);
	}
	else if (InputManager::getInstance()->onKeyPress(IND_ESCAPE) && MenuManager::getInstance()->isDisplayingPauseState()){
		// Hidding the Pause menu
		MenuManager::getInstance()->setLevelChoosen(false);
		m_bIsInGame = true;
	}

	// The PauseMenu need not to refresh the window in order to displayed upon the game view
	if(!MenuManager::getInstance()->isDisplayingPauseState()){
		m_pRender->clearViewPort(60, 60, 60);
	}
	// Otherwise, all the menus needs to refresh the window
	m_pRender->beginScene();
		MenuManager::getInstance()->renderEntities();
	m_pRender->endScene();

	//manage camera
	m_pRender->setCamera();
	//Manage user decisions
	if(MenuManager::getInstance()->isLevelChoosen()){
		// If the game part needs to be launch
		switchToGame();
		MenuManager::getInstance()->clear();
	}else if (MenuManager::getInstance()->isGoingBackToMenu() && MenuManager::getInstance()->isDisplayingPauseState()){
		// If the user wants to go back to the main menu from the pause menu
		m_pRender->setCameraPosition(m_pWindow->getIND_Window()->getWidth()*0.5, m_pWindow->getIND_Window()->getHeight()*0.5);
		clear();
		switchToMenu();
	}
}

void GameManager::switchToGame() {

	// Reset the menuManager attribut
	MenuManager::getInstance()->setLevelChoosen(false);
	
	//If no game have been created before then create a new one (from the main menu)
	if (m_pLevelManager == NULL) {
		//EntityManager::getInstance()->initRender(m_pRender);
		m_pLevelManager = new LevelManager();
		m_sCurrentLevel = MenuManager::getInstance()->getLevelToLoad();
		loadLevel(m_sCurrentLevel.c_str());
	 	m_bIsInGame = true;
	}
	// If the Player has finished the current level, then load the following
	else if(m_bIsLevelFinished){
		for (unsigned int i = 0; i < m_levelList.size(); ++i){
			if (m_sCurrentLevel == m_levelList[i]){
				if(i+1 == m_levelList.size()){
					fprintf(stderr, "You reached the latest level ! Back to menus.\n");
					clear();
					switchToMenu();
				}
				else {
					m_sCurrentLevel = m_levelList[i+1];
					loadLevel(m_sCurrentLevel.c_str());
					fprintf(stderr, "Next Level loaded \n");
					m_bIsInGame = true;
					break;
				}
			}
		}
	}
	// If the player resume game from the pause menu
	else {
		m_bIsInGame = true;
	}
}

void GameManager::switchToMenu() {

	//If the MenuManager doesn't exists, means at the first launch or when the user quit the game, then create it.
	if (m_bIsMenu == false) {
		// Retrive data from the player data file
		std::pair<Player*, std::vector<Player*>> playerData = m_pParser->loadPlayerData();

		// Start the menus
		MenuManager::getInstance();
		MenuManager::init(m_pRender, playerData);
		m_bIsMenu = true;

 		// Manage Camera
		m_pRender->setCamera();
 		m_pRender->setCameraPosition(m_pWindow->getIND_Window()->getWidth()*0.5, m_pWindow->getIND_Window()->getHeight()*0.5);
 	
 	}
 	else {
 		// Pause menu
 		std::vector<RenderEntity*> pDinos = EntityManager::getInstance()->getRenderDino();
 		PauseMenu* pPauseMenu = new PauseMenu(pDinos[0]->getPosX(), pDinos[0]	->getPosY());
 		MenuManager::getInstance()->setState(pPauseMenu);	
 	}

 	m_bIsInGame = false;
}

void GameManager::loadLevel(const char* mapFile) {
	EntityManager::getInstance()->initRender(m_pRender);
	EntityManager::getInstance()->deleteAllEntities();
	m_pLevelManager->loadLevel(mapFile);
}

void GameManager::debugPhysicalEntities() {
	for (unsigned int idEntity = 0; idEntity < EntityManager::getInstance()->getPhysicalEntityArray().size(); ++idEntity) {
		PhysicalEntity* pEntity = EntityManager::getInstance()->getPhysicalEntityArray()[idEntity];
		if(pEntity != NULL) {
			b2Vec2 topleft;
			topleft.x = pEntity->getPosition().x - pEntity->getWidth()/2;
			topleft.y = pEntity->getPosition().y + pEntity->getHeight()/2;
			b2Vec2 botright;
			botright.x = pEntity->getPosition().x + pEntity->getWidth()/2;
			botright.y = pEntity->getPosition().y - pEntity->getHeight()/2;
			//draw the hitbox in red
			m_pRender->getIND_Render()->blitRectangle(topleft.x, topleft.y, botright.x, botright.y, 255, 0, 0, 255);
			//draw the position
			m_pRender->getIND_Render()->blitRectangle(pEntity->getPosition().x-5, pEntity->getPosition().y+5, pEntity->getPosition().x+5, pEntity->getPosition().y-5, 255, 0, 255, 255);
		}
	}
}

void GameManager::debugRenderEntities() {
	for (unsigned int idEntity = 0; idEntity < EntityManager::getInstance()->getRenderEntityArray().size(); ++idEntity) {
		std::vector<RenderEntity*> entityArray = EntityManager::getInstance()->getRenderEntityArray()[idEntity];
		if(entityArray.size() > 0) {
			for (unsigned int indexEntity = 0; indexEntity < entityArray.size(); ++indexEntity) {
				RenderEntity* rEntity = entityArray[indexEntity];
				b2Vec2 topleft;
				topleft.x = rEntity->getPosX() - rEntity->getWidth()/2;
				topleft.y = rEntity->getPosY() + rEntity->getHeight()/2;
				b2Vec2 botright;
				botright.x = rEntity->getPosX() + rEntity->getWidth()/2;
				botright.y = rEntity->getPosY() - rEntity->getHeight()/2;
				//draw the size in green
				m_pRender->getIND_Render()->blitRectangle(topleft.x, topleft.y, botright.x, botright.y, 0, 255, 0, 255);
				//draw the position
				m_pRender->getIND_Render()->blitRectangle(rEntity->getPosX()-5, rEntity->getPosY()+5, rEntity->getPosX()+5, rEntity->getPosY()-5, 0, 255, 255, 255);
			}
		}
	}
}

}
