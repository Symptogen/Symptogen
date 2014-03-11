#include <Indie.h>

#include "GameManager.h"
#include "menu/PauseMenu.h"
#include "menu/Player.h"
#include "power/Power.h"
#include "power/Sneeze.h"
#include "power/Fever.h"
#include "power/Headache.h"


namespace Symp {

GameManager::GameManager() {
	IndieLib::init(IND_DEBUG_MODE);
	m_pWindow = new Window();
	m_pRender = new Render();
	m_pWindow->setWindow(m_pRender->init("Symptogen", 1000, 800, 32, 0, 0, 1));
	//m_pRender->toggleFullScreen();
	m_pWindow->setCursor(true);

	InputManager::getInstance()->initRender(m_pRender);;

	m_pParserPlayer = new ParserPlayer("../assets/data.xml");

	m_pParserLevel = nullptr;
	m_bIsMenu = false;
	m_bIsLevelFinished = false;
	m_bIsPlayerDead = false;

	// Set the levels order
	m_levelList.push_back("../assets/map/level1.xml");
	m_levelList.push_back("../assets/map/level2.xml");
	m_levelList.push_back("../assets/map/level3.xml");

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
	delete m_pParserLevel;
	MenuManager::removeInstance();
	m_pParserLevel = NULL;
	EntityManager::removeInstance();
	m_bIsMenu = false;
}

void GameManager::startMainLoop(){
	//If the user didn't closed the window or didn't clicked a "quit" button, then update
	while (!MenuManager::getInstance()->isAboutToQuit() && !InputManager::getInstance()->quit()){
		InputManager::getInstance()->update();
		m_pRender->setCamera();
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
	/*    Move Dino   */
	/******************/

	m_dinoState = EntityManager::getInstance()->getCurrentPowerState();
	//if dino can move
	if(EntityManager::getInstance()->isDinoAllowToMove()){
		// Up		
		if(EntityManager::getInstance()->isDinoAllowToJump() &&InputManager::getInstance()->onKeyPress(IND_KEYUP)) {
			// Physics
		    m_pPhysicalDino->getb2Body()->ApplyLinearImpulse(b2Vec2(0, -m_fJumpForce), m_pPhysicalDino->getb2Body()->GetWorldCenter(), m_pPhysicalDino->isAwake());
		    // Sound
			SoundManager::getInstance()->play(EntityManager::getInstance()->getSoundDino()[DinoAction::Jump]->getIndexSound());
		}
		// Left
		if(InputManager::getInstance()->isKeyPressed(IND_KEYLEFT)) {
			// Physics
			m_pPhysicalDino->getb2Body()->ApplyLinearImpulse(b2Vec2(-m_fImpulse, m_fImpulse/5.f), m_pPhysicalDino->getb2Body()->GetWorldCenter(), m_pPhysicalDino->isAwake());
			// Render
			EntityManager::getInstance()->setDinoRender(EntityManager::getInstance()->getRightWalk());
		}
		// Right
		if(InputManager::getInstance()->isKeyPressed(IND_KEYRIGHT)) {
			// Physics
			m_pPhysicalDino->getb2Body()->ApplyLinearImpulse(b2Vec2(m_fImpulse, m_fImpulse/5.f), m_pPhysicalDino->getb2Body()->GetWorldCenter(), m_pPhysicalDino->isAwake());
			// Render
			EntityManager::getInstance()->setDinoRender(EntityManager::getInstance()->getRightWalk());
		}
		// If no movements
		if(EntityManager::getInstance()->getPhysicalDino()->getLinearVelocity().x == 0) {
				EntityManager::getInstance()->setDinoRender(EntityManager::getInstance()->getRightStop());
		}
	}
	if(InputManager::getInstance()->isKeyPressed(IND_SPACE)){
		dynamic_cast<Headache*>(EntityManager::getInstance()->getPower(PowerType::HeadacheType))->forceExecution();

	}
	/*********/
	/* Death */
	/*********/

	if( EntityManager::getInstance()->getCurrentDinoAction() == EntityManager::getInstance()->getRightDeath()){
		if(EntityManager::getInstance()->getRenderDino().at(EntityManager::getInstance()->getRightDeath())->isAnimationPlaying()) {
			EntityManager::getInstance()->getRenderDino().at(EntityManager::getInstance()->getRightDeath())->manageAnimationTimer(AnimationLength::DieLength);
		}
		if(EntityManager::getInstance()->getRenderDino().at(EntityManager::getInstance()->getRightDeath())->isAnimationFinish()) {
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
		// Draw tests
		//debugPhysicalEntities();
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
	if(EntityManager::getInstance()->getCurrentDinoAction() == EntityManager::getInstance()->getRightDeath()
		&& EntityManager::getInstance()->getRenderDino().at(EntityManager::getInstance()->getRightDeath())->isAnimationFinish()){
		m_bIsPlayerDead = true;
		switchToGame();
	}

	/********************/
	/* has to be after the instruction for death to avoid bug for the death by power (temperature)*/
	/* Powers */
	/********************/
	if(!EntityManager::getInstance()->getRenderDino().at(EntityManager::getInstance()->getRightDeath())->isAnimationPlaying()){
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
	m_pRender->setZoom(m_iGameScale);
	
		//If no game have been created before then create a new one (from the main menu)
	if (m_pParserLevel == NULL) {

		//EntityManager::getInstance()->initRender(m_pRender);
		m_pParserLevel = new ParserLevel();
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
 	m_pRender->setZoom(m_iMenuScale);//need to set zoom before draw pause menu (can't clear viewport !)

	//If the MenuManager doesn't exists, means at the first launch or when the user quit the game, then create it.
	if (m_bIsMenu == false) {
		// Retrive data from the player data file
		std::pair<Player*, std::vector<Player*>> playerData = m_pParserPlayer->loadPlayerData();

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
 		MenuManager::getInstance()->setState(pPauseMenu);
  	}

 	m_bIsInGame = false;
}

void GameManager::loadLevel(const char* mapFile) {
	PhysicalEntity::clearMovableObjectArray();
	EntityManager::getInstance()->initRender(m_pRender);
	EntityManager::getInstance()->deleteAllEntities();
	EntityManager::getInstance()->deleteAllPowers();
	m_iGameScale = m_pParserLevel->loadLevel(mapFile);
	m_pRender->setZoom(m_iGameScale);

	m_fExitX = EntityManager::getInstance()->getExitCoordinates()[0];
	m_fExitY = EntityManager::getInstance()->getExitCoordinates()[1];
}

void GameManager::debugPhysicalEntities() {
	for (unsigned int idEntity = 0; idEntity < EntityManager::getInstance()->getPhysicalEntityArray().size(); ++idEntity) {
		PhysicalEntity* pEntity = EntityManager::getInstance()->getPhysicalEntity(idEntity);
		if(pEntity != nullptr) {

			// Draw the center
			m_pRender->getIND_Render()->blitRectangle(
				pEntity->getPosition().x-2, pEntity->getPosition().y+2, 
				pEntity->getPosition().x+2, pEntity->getPosition().y-2, 
				255, 0, 255, 255);
		
			
			// Draw hitbox if custom
			b2Shape* pShape = pEntity->getb2Shape();
			b2Vec2 physicalOrigin = pEntity->getPosition();
			//b2ChainShape
			if(pShape != nullptr && pShape->GetType() == b2Shape::Type::e_chain){
				b2ChainShape* pChain = static_cast<b2ChainShape*>(pShape);
				for(int i = 1; i < pChain->m_count; ++i){
					m_pRender->getIND_Render()->blitLine(
						physicalOrigin.x + pChain->m_vertices[i-1].x, physicalOrigin.y + pChain->m_vertices[i-1].y, 
						physicalOrigin.x + pChain->m_vertices[i].x, physicalOrigin.y + pChain->m_vertices[i].y, 
						255, 0, 0, 255);
				}
			}
			//b2PolygonShape
			else if(pShape != nullptr && pShape->GetType() == b2Shape::Type::e_polygon){
				b2PolygonShape* pPolygon = static_cast<b2PolygonShape*>(pShape);
				for(int i = 1; i < pPolygon->m_count; ++i){
					m_pRender->getIND_Render()->blitLine(
						physicalOrigin.x + pPolygon->m_vertices[i-1].x, physicalOrigin.y + pPolygon->m_vertices[i-1].y, 
						physicalOrigin.x + pPolygon->m_vertices[i].x, physicalOrigin.y + pPolygon->m_vertices[i].y, 
						255, 0, 0, 255);
				}
			}
			// Draw the hitbox if no custom
			else{
				b2Vec2 topleft;
				topleft.x = pEntity->getPosition().x - pEntity->getWidth()/2;
				topleft.y = pEntity->getPosition().y + pEntity->getHeight()/2;
				b2Vec2 botright;
				botright.x = pEntity->getPosition().x + pEntity->getWidth()/2;
				botright.y = pEntity->getPosition().y - pEntity->getHeight()/2;
				m_pRender->getIND_Render()->blitRectangle(
					topleft.x, topleft.y, 
					botright.x, botright.y, 
					255, 0, 0, 255);
			}

		}
	}
}

void GameManager::debugRenderEntities() {
	for(size_t idEntity = 0; idEntity < EntityManager::getInstance()->getRenderEntityArray().size(); ++idEntity) {
		std::vector<RenderEntity*> entityArray = EntityManager::getInstance()->getRenderEntity(idEntity);
		if(entityArray.size() > 0) {
			for(size_t indexEntity = 0; indexEntity < entityArray.size(); ++indexEntity) {
				RenderEntity* rEntity = entityArray.at(indexEntity);
				if(rEntity != nullptr){
					b2Vec2 topleft;
					topleft.x = rEntity->getPosX() - rEntity->getWidth()/2;
					topleft.y = rEntity->getPosY() + rEntity->getHeight()/2;
					b2Vec2 botright;
					botright.x = rEntity->getPosX() + rEntity->getWidth()/2;
					botright.y = rEntity->getPosY() - rEntity->getHeight()/2;
					
					// Draw the borders
					m_pRender->getIND_Render()->blitRectangle(
						topleft.x, topleft.y, 
						botright.x, botright.y, 
						0, 255, 0, 255);
					// Draw the center
					m_pRender->getIND_Render()->blitRectangle(
						rEntity->getPosX()-2, rEntity->getPosY()+2, 
						rEntity->getPosX()+2, rEntity->getPosY()-5, 
						0, 255, 255, 255);
				}
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

