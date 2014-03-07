#include <Indie.h>

#include "GameManager.h"
#include "menu/PauseMenu.h"
#include "menu/Player.h"
#include "power/Power.h"
#include "power/Sneeze.h"

namespace Symp {

GameManager::GameManager() {
	IndieLib::init(IND_DEBUG_MODE);
	m_pWindow = new Window();
	m_pRender = new Render();
	m_pWindow->setWindow(m_pRender->init("Symptogen", 1000, 800, 32, 0, 0, 1));
	//m_pRender->toggleFullScreen();
	m_pWindow->setCursor(true);

	InputManager::getInstance()->initRender(m_pRender);;

	m_pParser = new Parser("../assets/data.xml");

	m_pLevelManager = nullptr;
	m_bIsMenu = false;
	m_bIsLevelFinished = false;
	m_bIsPlayerDead = false;

	// Set the levels order
	m_levelList.push_back("../assets/map/level1.xml");
	m_levelList.push_back("../assets/map/level2.xml");

	// Scale of menu and game (zoom)
	m_iMenuScale = 1;
	m_iGameScale = 1;
	m_dinoState = PowerType::NormalType;

	m_fExitX = -1.f;
	m_fExitY = -1.f;

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
	EntityManager::getInstance()->deleteAllPowers();
	delete m_pLevelManager;
	MenuManager::removeInstance();
	m_pLevelManager = NULL;
	EntityManager::removeInstance();
	m_bIsMenu = false;
}

void GameManager::startMainLoop(){
	//If the user didn't closed the window or didn't clicked a "quit" button, then update
	while (!MenuManager::getInstance()->isAboutToQuit() && !InputManager::getInstance()->quit()){
		InputManager::getInstance()->update();
		m_pRender->setCamera();
		if(m_bIsInGame) {
			//m_pRender->setZoom(m_iGameScale);
			updateGame();
		}
		else {
			m_pRender->setZoom(m_iMenuScale);
			updateMenu();
		}
	}
}

void GameManager::updateGame() {
	/******************/
	/*    Move Dino   */
	/******************/

	m_dinoState = EntityManager::getInstance()->getCurrentPowerState();

	//if dino can move
	if(EntityManager::getInstance()->isDinoAllowToMove()){
		// Left
		if(InputManager::getInstance()->isKeyPressed(IND_KEYLEFT) && !m_pPhysicalDino->hasContactingLeft()) {
			// Physics
			m_pPhysicalDino->getb2Body()->ApplyLinearImpulse(b2Vec2(-m_fImpulse, 0), m_pPhysicalDino->getb2Body()->GetWorldCenter(), m_pPhysicalDino->isAwake());
			// Render
			if(m_dinoState == PowerType::SneezeType)
				EntityManager::getInstance()->setDinoRender(DinoAction::Sneezing);
			else if(m_dinoState == PowerType::FeverType)
				EntityManager::getInstance()->setDinoRender(DinoAction::WalkHotFever);
			else if(m_dinoState == PowerType::NormalType)
				EntityManager::getInstance()->setDinoRender(DinoAction::Walk);
		}
		// Right
		if(InputManager::getInstance()->isKeyPressed(IND_KEYRIGHT) && !m_pPhysicalDino->hasContactingRight()) {
			// Physics
			m_pPhysicalDino->getb2Body()->ApplyLinearImpulse(b2Vec2(m_fImpulse, 0), m_pPhysicalDino->getb2Body()->GetWorldCenter(), m_pPhysicalDino->isAwake());
			// Render
			if(m_dinoState == PowerType::SneezeType)
				EntityManager::getInstance()->setDinoRender(DinoAction::Sneezing);
			else if(m_dinoState == PowerType::FeverType)
				EntityManager::getInstance()->setDinoRender(DinoAction::WalkHotFever);
			else if(m_dinoState == PowerType::NormalType)
				EntityManager::getInstance()->setDinoRender(DinoAction::Walk);
		}
		// Up		
		if(EntityManager::getInstance()->isDinoAllowToJump()
			&&InputManager::getInstance()->onKeyPress(IND_KEYUP) 
			&& m_pPhysicalDino->hasContactingBelow()) {
			// Physics
		    m_pPhysicalDino->getb2Body()->ApplyLinearImpulse(b2Vec2(0, -m_fJumpForce), m_pPhysicalDino->getb2Body()->GetWorldCenter(), m_pPhysicalDino->isAwake());
		    // Sound
			SoundManager::getInstance()->play(EntityManager::getInstance()->getSoundDino()[DinoAction::Jump]->getIndexSound());
		}
		// If no movements
		if(EntityManager::getInstance()->getPhysicalDino()->getLinearVelocity().x == 0) {
			if(m_dinoState == PowerType::SneezeType)
				EntityManager::getInstance()->setDinoRender(DinoAction::Sneezing);
			else if(m_dinoState == PowerType::FeverType)
				EntityManager::getInstance()->setDinoRender(DinoAction::StopHotFever);
			else if(m_dinoState == PowerType::NormalType)
				EntityManager::getInstance()->setDinoRender(DinoAction::StopNormal);
		}
	}

	/*********/
	/* Death */
	/*********/

	if( EntityManager::getInstance()->getCurrentDinoAction() == DinoAction::Die){
		if(EntityManager::getInstance()->getRenderDino().at(DinoAction::Die)->isAnimationPlaying()){
			EntityManager::getInstance()->getRenderDino().at(DinoAction::Die)->manageAnimationTimer(AnimationLength::DieLength);
		}
		else if(EntityManager::getInstance()->getRenderDino().at(DinoAction::Die)->isAnimationFinish()) {
			switchToGame();
			loadCurrentLevel();
			loadPhysics();
		}
	}

	/*****************/
	/* Manage Camera */
	/*****************/

	//debug
	if (InputManager::getInstance()->isKeyPressed(IND_S)){
		float newZoom = m_pRender->getZoom()+0.005;
		m_pRender->setZoom(newZoom);
	}
	else if (InputManager::getInstance()->isKeyPressed(IND_Q)){
		float newZoom = m_pRender->getZoom()-0.005;
		m_pRender->setZoom(newZoom);
	}
	else if (InputManager::getInstance()->isKeyPressed(IND_D)){
		m_pRender->setZoom(m_iGameScale);
	}

	m_pRender->setCameraPosition(m_pPhysicalDino->getPosition().x, m_pPhysicalDino->getPosition().y);

	/*******************/
	/* Update entities */
	/*******************/

	EntityManager::getInstance()->updateEntities();

	/*****************/
	/* Manage render */
	/*****************/

	m_pRender->clearViewPort(60, 60, 60);
	m_pRender->beginScene();
		EntityManager::getInstance()->renderEntities();
		//test hitbox
		debugPhysicalEntities();
		//debugRenderEntities();
	m_pRender->endScene();

	/*********/
	/* Pause */
	/*********/

	if (InputManager::getInstance()->onKeyPress(IND_ESCAPE)) {
		switchToMenu();
	}

	/********************/
	/* Detect level end */
	/********************/

	// If the player reached the end of the level
	if(abs(m_fExitX - m_pPhysicalDino->getPosition().x) < 10 && abs(m_fExitY - m_pPhysicalDino->getPosition().y) < 10) {
		m_bIsLevelFinished = true;
		switchToGame();
		return;
	}
	// If the player is dead
	if(EntityManager::getInstance()->getCurrentDinoAction() == DinoAction::Die 
		&& EntityManager::getInstance()->getRenderDino().at(DinoAction::Die)->isAnimationFinish()){
		m_bIsPlayerDead = true;
		switchToGame();
	}

	/********************/
	/* has to be after the instruction for death to avoid bug for the death by power (temperature)*/
	/* Powers */
	/********************/
	if(!EntityManager::getInstance()->getRenderDino().at(DinoAction::Die)->isAnimationPlaying()){
		EntityManager::getInstance()->executePowers();
	}
}

void GameManager::updateMenu() {
	//Forward inputs

	//The following offsets are necessary to the mouse pointer to click on the pause menu
	//because it's not the same coordinate space
	int offsetX = 0;
	int offsetY = 0;
	if(MenuManager::getInstance()->isDisplayingPauseState()){
		offsetX = m_pPhysicalDino->getPosition().x - m_pWindow->getIND_Window()->getWidth()*0.5;
		offsetY = m_pPhysicalDino->getPosition().y - m_pWindow->getIND_Window()->getHeight()*0.5;
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
		loadPhysics();
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
					loadPhysics();
					fprintf(stderr, "Next Level loaded \n");
					m_bIsLevelFinished = false;
					m_bIsInGame = true;
					break;
				}
			}
		}
	}

	// If the player resume game from the pause menu
	else{
		m_bIsInGame = true;
	}
}

void GameManager::switchToMenu() {
	//If the MenuManager doesn't exists, means at the first launch or when the user quit the game, then create it.
	if (m_bIsMenu == false) {
		// Retrive data from the player data file
		std::pair<Player*, std::vector<Player*>> playerData = m_pParser->loadPlayerData();

		// Start the menus
		MenuManager::getInstance()->init(m_pRender, playerData);
		m_bIsMenu = true;

		// Camera
 		m_pRender->setCameraPosition(m_pWindow->getIND_Window()->getWidth()*0.5, m_pWindow->getIND_Window()->getHeight()*0.5);
 	}
 	else {
 		// Pause menu
 		std::vector<RenderEntity*> pDinos = EntityManager::getInstance()->getRenderDino();
 		PauseMenu* pPauseMenu = new PauseMenu(pDinos[0]->getPosX(), pDinos[0]->getPosY());
 		m_pRender->setZoom(m_iMenuScale);//need to set zoom before draw pause menu (can't clear viewport !)
 		MenuManager::getInstance()->setState(pPauseMenu);
 	}

 	m_bIsInGame = false;
}

void GameManager::loadLevel(const char* mapFile) {
	PhysicalEntity::clearMovableObjectArray();
	EntityManager::getInstance()->initRender(m_pRender);
	EntityManager::getInstance()->deleteAllEntities();
	EntityManager::getInstance()->deleteAllPowers();
	m_iGameScale = m_pLevelManager->loadLevel(mapFile);

	m_fExitX = EntityManager::getInstance()->getExitCoordinates()[0];
	m_fExitY = EntityManager::getInstance()->getExitCoordinates()[1];
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

void GameManager::loadPhysics(){
	m_fT = 1.f/60.f;
	m_fForceFactor = 20.f;
	m_pPhysicalDino =  EntityManager::getInstance()->getPhysicalDino();
	m_fImpulse = m_pPhysicalDino->getMass() * m_fForceFactor;
	float gravity = EntityManager::getInstance()->getPhysicalWorld()->getGravity().y;
	m_fJumpForce = sqrt(gravity*m_fImpulse) / m_fT;
}

}

