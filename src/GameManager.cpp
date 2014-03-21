#include <Indie.h>

#include "GameManager.h"

#include "menu/PauseMenu.h"
#include "menu/Player.h"

#include "power/Power.h"
#include "power/Sneeze.h"
#include "power/Fever.h"
#include "power/Headache.h"

#include <X11/Xlib.h>

namespace Symp {

// Default values based on the 2013 screen resolution statistics
int g_WindowWidth = 1365;
int g_WindowHeight = 767;


GameManager::GameManager() {

	IndieLib::init(IND_DEBUG_MODE);
	m_pWindow = new Window();
	m_pRender = new Render();

	Display* disp = XOpenDisplay(NULL);
	Screen*  scrn = DefaultScreenOfDisplay(disp);
	g_WindowHeight = scrn->height;
	g_WindowWidth  = scrn->width;

	m_pWindow->setWindow(m_pRender->init("Symptogen", g_WindowWidth, g_WindowHeight, 32, false, false, true));
	//m_pRender->toggleFullScreen();
	m_pWindow->setCursor(true);

	InputManager::getInstance()->initRender(m_pRender);;

	m_pParserPlayer = new ParserPlayer("../assets/data.xml");

	m_pParserLevel = nullptr;
	m_bIsMenu = false;
	m_bIsLevelFinished = false;
	m_bIsPlayerDead = false;
	m_bIsPlayingKinematic = false;
	m_bHasKinematicBeenPlayed = false;

	// Set the levels order
	m_levelList.push_back("../assets/map/level1.xml");
	m_levelList.push_back("../assets/map/level2.xml");
	m_levelList.push_back("../assets/map/level3.xml");
	m_levelList.push_back("../assets/map/level4.xml");
	m_levelList.push_back("../assets/map/level5.xml");
	m_levelList.push_back("../assets/map/level6.xml");
	m_levelList.push_back("../assets/map/level7.xml");
	m_levelList.push_back("../assets/map/level8.xml");
	m_levelList.push_back("../assets/map/level9.xml");

	// Scale of menu and game (zoom)
	m_iMenuScale = 1;
	m_iGameScale = 1;

	m_fExitX = -1.f;
	m_fExitY = -1.f;

	switchToMenu();
}

GameManager::~GameManager() {
	if(m_pRender->isFullScreen())
		m_pRender->toggleFullScreen();
	delete m_pWindow;
	delete m_pRender;
	InputManager::removeInstance();
	MenuManager::removeInstance();
	IndieLib::end();
}


void GameManager::clear() {
	PhysicalEntity::clearMovableObjectArray();
	EntityManager::getInstance()->deleteAllEntities();
	EntityManager::getInstance()->deleteAllPowers();
	EntityManager::getInstance()->destroyRender();
	delete m_pParserLevel;
	MenuManager::removeInstance();
	m_pParserLevel = NULL;
	EntityManager::removeInstance();
	m_bIsMenu = false;
}

void GameManager::startMainLoop() {
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

void GameManager::createKinematic(){
	m_bIsPlayingKinematic = true;
	m_bHasKinematicBeenPlayed = false;

	// Select the correct kinematic
	if(m_sCurrentLevel == m_levelList.front()){
		kinematic = new RenderEntity("../assets/animation/KinematicBegin.xml", Symp::Animation);
	}else if(m_sCurrentLevel == m_levelList.back()){
		kinematic = new RenderEntity("../assets/animation/KinematicEnd.xml", Symp::Animation);
	}
	kinematic->setHotSpot(0.5, 0.5);
	kinematic->setPosition(g_WindowWidth/2, g_WindowHeight/2);

	//Retrieve attributes from the textures themselves
	float surfaceWidth = (float)kinematic->getWidth();
	float surfaceHeight = (float)kinematic->getHeight();
	float scaleX, scaleY, scale;

	// Compute the scale value
	if (g_WindowWidth < surfaceWidth) {
		scaleX = g_WindowWidth / surfaceWidth;
	}else {
		scaleX = surfaceWidth / g_WindowWidth ;
	}
	if (g_WindowHeight < surfaceHeight){
		scaleY = g_WindowHeight / surfaceHeight;
	} else {
		scaleY = surfaceHeight / g_WindowHeight;
	}
	//Scale the kinematics
	if(m_sCurrentLevel == m_levelList.front()){
		scale = min(scaleX, scaleY);
		kinematic->setScale(scale, scale);
	}else if(m_sCurrentLevel == m_levelList.back()) {
	 	kinematic->setScale(scaleX, scaleY);
	}

	// Display the kinematic
	kinematic->setShow(true);
	std::vector<RenderEntity*> renderArray;
	renderArray.push_back(kinematic);
	

	//Start timer
	if(m_sCurrentLevel == m_levelList.front()){
		EntityManager::getInstance()->addRenderEntity(renderArray, 63);
		kinematic->manageAnimationTimer(AnimationLength::KinematicBeginLenght);
	}else if(m_sCurrentLevel == m_levelList.back()){
		EntityManager::getInstance()->addRenderEntity(renderArray, 0);
		kinematic->manageAnimationTimer(AnimationLength::KinematicEndLenght);
	}
	
}

void GameManager::updateGame() {

	if(!m_bIsPlayingKinematic) {

		/******************/
		/*    Move Dino   */
		/******************/

		// Check if the dino can move
		if(EntityManager::getInstance()->isDinoAllowToMove()) {

			// KEYBOARD UP ARROW
			if(EntityManager::getInstance()->isDinoAllowToJump() &&InputManager::getInstance()->onKeyPress(IND_KEYUP)) {
				
				// Physics
				b2Vec2 force(0, -m_fJumpForce);
			    m_pPhysicalDino->getb2Body()->ApplyLinearImpulse(force, m_pPhysicalDino->getb2Body()->GetWorldCenter(), m_pPhysicalDino->isAwake());
			    
			    // Sound
				SoundManager::getInstance()->playSound(EntityManager::getInstance()->getSoundDino()[DinoAction::Jump]->getSound());

			}

			// KEYBOARD LEFT ARROW
			if(InputManager::getInstance()->isKeyPressed(IND_KEYLEFT)) {
				
				// Physics
				b2Vec2 force;

				// If the Dino is jumping (y veloc < 0)
				if(EntityManager::getInstance()->getPhysicalDino()->getLinearVelocity().y < 0) {
					force = b2Vec2(-m_fImpulse, 0);
				}
				else {
					force = b2Vec2(-m_fImpulse, m_fImpulse*0.5f);
				}

				m_pPhysicalDino->getb2Body()->ApplyLinearImpulse(force, m_pPhysicalDino->getb2Body()->GetWorldCenter(), m_pPhysicalDino->isAwake());
				
				// Render
				EntityManager::getInstance()->setDinoRender(EntityManager::getInstance()->getRightWalk());
				EntityManager::getInstance()->flipDinoRenderEntities(true);

			}

			// KEYBOARD RIGHT ARROW
			if(InputManager::getInstance()->isKeyPressed(IND_KEYRIGHT)) {
				
				// Physics
				b2Vec2 force;

				// If the Dino is jumping (y veloc < 0)
				if(EntityManager::getInstance()->getPhysicalDino()->getLinearVelocity().y < 0) {
					force = b2Vec2(m_fImpulse, 0);
				}
				else {
					force = b2Vec2(m_fImpulse, m_fImpulse*0.5f);
				}

				m_pPhysicalDino->getb2Body()->ApplyLinearImpulse(force, m_pPhysicalDino->getb2Body()->GetWorldCenter(), m_pPhysicalDino->isAwake());

				// Render
				EntityManager::getInstance()->setDinoRender(EntityManager::getInstance()->getRightWalk());
				EntityManager::getInstance()->flipDinoRenderEntities(false);

			}

			// IF NO MOVEMENTS
			if(abs(EntityManager::getInstance()->getPhysicalDino()->getLinearVelocity().x) <= 0.001) {
				EntityManager::getInstance()->setDinoRender(EntityManager::getInstance()->getRightStop());
			}

			// Physics : the dino doesn't slide along other physical entities.
			if(!InputManager::getInstance()->isKeyPressed(IND_KEYLEFT) && !InputManager::getInstance()->isKeyPressed(IND_KEYRIGHT))
				EntityManager::getInstance()->getPhysicalDino()->setLinearVelocity(
					b2Vec2(EntityManager::getInstance()->getPhysicalDino()->getLinearVelocity().x*0.5f, EntityManager::getInstance()->getPhysicalDino()->getLinearVelocity().y));
		}

		// TEST
		/*if(InputManager::getInstance()->isKeyPressed(IND_SPACE)){
			if(EntityManager::getInstance()->isPowerExisting(PowerType::HeadacheType))
				dynamic_cast<Headache*>(EntityManager::getInstance()->getPower(PowerType::HeadacheType))->forceExecution();
		}*/

		/*********/
		/* Death */
		/*********/
		if(EntityManager::getInstance()->getCurrentDinoAction() == EntityManager::getInstance()->getRightDeath()){
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
	}
	/*******************/
	/* Update entities */
	/*******************/
	
	EntityManager::getInstance()->updateEntities();
	// EntityManager::getInstance()->shiverBackground();
	
	/*****************/
	/* Manage render */
	/*****************/

	m_pRender->clearViewPort(60, 60, 60);
	m_pRender->beginScene();
		EntityManager::getInstance()->renderEntities();
		// Draw tests
		debugPhysicalEntities();
		//debugRenderEntities();
	m_pRender->endScene();


	/******************/
	/*   Kinematic  */
	/******************/

	if(m_bIsPlayingKinematic){
		if(kinematic->isAnimationPlaying()) {
			m_bIsPlayingKinematic = true;
			if(m_sCurrentLevel == m_levelList.front()){
			 	kinematic->manageAnimationTimer(AnimationLength::KinematicBeginLenght);
			}else if(m_sCurrentLevel == m_levelList.back()){
				kinematic->manageAnimationTimer(AnimationLength::KinematicEndLenght);
			}
		}
		if(kinematic->isAnimationFinish()){
			m_bIsPlayingKinematic = false;
			m_bHasKinematicBeenPlayed = true;
			switchToGame();
		}
	}

	/********************/
	/* Detect level end */
	/********************/

	if(!m_bIsPlayingKinematic){
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
		/*    Powers        */
		/********************/
		
		EntityManager::getInstance()->executePowers();
	}

	/*********/
	/* Pause */
	/*********/

	if (InputManager::getInstance()->onKeyPress(IND_ESCAPE)) {
		if(m_bIsPlayingKinematic){
			switchToGame();
		}else{
			switchToMenu();
		}
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

	if (InputManager::getInstance()->onKeyPress(IND_KEYDOWN)){MenuManager::getInstance()->handleKeyPressed("KEYDOWN");}
	else if (InputManager::getInstance()->onKeyPress(IND_KEYUP)){MenuManager::getInstance()->handleKeyPressed("KEYUP");}
	else if (InputManager::getInstance()->onKeyPress(IND_DELETE)){MenuManager::getInstance()->handleKeyPressed("DELETE");}
	else if (InputManager::getInstance()->onKeyPress(IND_KEYRIGHT)){MenuManager::getInstance()->handleKeyPressed("KEYRIGHT");}
	else if (InputManager::getInstance()->onKeyPress(IND_KEYLEFT)){MenuManager::getInstance()->handleKeyPressed("KEYLEFT");}
	else if (InputManager::getInstance()->onKeyPress(IND_BACKSPACE)){MenuManager::getInstance()->handleKeyPressed("BACKSPACE");}
	else if (InputManager::getInstance()->onKeyPress(IND_A)){MenuManager::getInstance()->handleKeyPressed("a");}
	else if (InputManager::getInstance()->onKeyPress(IND_B)){MenuManager::getInstance()->handleKeyPressed("b");}
	else if (InputManager::getInstance()->onKeyPress(IND_C)){MenuManager::getInstance()->handleKeyPressed("c");}
	else if (InputManager::getInstance()->onKeyPress(IND_D)){MenuManager::getInstance()->handleKeyPressed("d");}
	else if (InputManager::getInstance()->onKeyPress(IND_E)){MenuManager::getInstance()->handleKeyPressed("e");}
	else if (InputManager::getInstance()->onKeyPress(IND_F)){MenuManager::getInstance()->handleKeyPressed("f");}
	else if (InputManager::getInstance()->onKeyPress(IND_G)){MenuManager::getInstance()->handleKeyPressed("g");}
	else if (InputManager::getInstance()->onKeyPress(IND_H)){MenuManager::getInstance()->handleKeyPressed("h");}
	else if (InputManager::getInstance()->onKeyPress(IND_I)){MenuManager::getInstance()->handleKeyPressed("i");}
	else if (InputManager::getInstance()->onKeyPress(IND_J)){MenuManager::getInstance()->handleKeyPressed("j");}
	else if (InputManager::getInstance()->onKeyPress(IND_K)){MenuManager::getInstance()->handleKeyPressed("k");}
	else if (InputManager::getInstance()->onKeyPress(IND_L)){MenuManager::getInstance()->handleKeyPressed("l");}
	else if (InputManager::getInstance()->onKeyPress(IND_M)){MenuManager::getInstance()->handleKeyPressed("m");}
	else if (InputManager::getInstance()->onKeyPress(IND_N)){MenuManager::getInstance()->handleKeyPressed("n");}
	else if (InputManager::getInstance()->onKeyPress(IND_O)){MenuManager::getInstance()->handleKeyPressed("o");}
	else if (InputManager::getInstance()->onKeyPress(IND_P)){MenuManager::getInstance()->handleKeyPressed("p");}
	else if (InputManager::getInstance()->onKeyPress(IND_Q)){MenuManager::getInstance()->handleKeyPressed("q");}
	else if (InputManager::getInstance()->onKeyPress(IND_R)){MenuManager::getInstance()->handleKeyPressed("r");}
	else if (InputManager::getInstance()->onKeyPress(IND_S)){MenuManager::getInstance()->handleKeyPressed("s");}
	else if (InputManager::getInstance()->onKeyPress(IND_T)){MenuManager::getInstance()->handleKeyPressed("t");}
	else if (InputManager::getInstance()->onKeyPress(IND_U)){MenuManager::getInstance()->handleKeyPressed("u");}
	else if (InputManager::getInstance()->onKeyPress(IND_V)){MenuManager::getInstance()->handleKeyPressed("v");}
	else if (InputManager::getInstance()->onKeyPress(IND_W)){MenuManager::getInstance()->handleKeyPressed("w");}
	else if (InputManager::getInstance()->onKeyPress(IND_X)){MenuManager::getInstance()->handleKeyPressed("x");}
	else if (InputManager::getInstance()->onKeyPress(IND_Y)){MenuManager::getInstance()->handleKeyPressed("y");}
	else if (InputManager::getInstance()->onKeyPress(IND_Z)){MenuManager::getInstance()->handleKeyPressed("z");}
	// else if (InputManager::getInstance()->onKeyPress(IND_0)){MenuManager::getInstance()->handleKeyPressed("0");}
	// else if (InputManager::getInstance()->onKeyPress(IND_1)){MenuManager::getInstance()->handleKeyPressed("1");}
	// else if (InputManager::getInstance()->onKeyPress(IND_2)){MenuManager::getInstance()->handleKeyPressed("2");}
	// else if (InputManager::getInstance()->onKeyPress(IND_3)){MenuManager::getInstance()->handleKeyPressed("3");}
	// else if (InputManager::getInstance()->onKeyPress(IND_4)){MenuManager::getInstance()->handleKeyPressed("4");}
	// else if (InputManager::getInstance()->onKeyPress(IND_5)){MenuManager::getInstance()->handleKeyPressed("5");}
	// else if (InputManager::getInstance()->onKeyPress(IND_6)){MenuManager::getInstance()->handleKeyPressed("6");}
	// else if (InputManager::getInstance()->onKeyPress(IND_7)){MenuManager::getInstance()->handleKeyPressed("7");}
	// else if (InputManager::getInstance()->onKeyPress(IND_8)){MenuManager::getInstance()->handleKeyPressed("8");}
	// else if (InputManager::getInstance()->onKeyPress(IND_9)){MenuManager::getInstance()->handleKeyPressed("9");}
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
		m_pRender->setZoom(m_iGameScale);
		if(EntityManager::getInstance()->isPowerExisting(PowerType::HeadacheType))
			m_pRender->setCameraAngle(static_cast<Headache*>(EntityManager::getInstance()->getPower(PowerType::HeadacheType))->getInterpolateAngle());
	}

	// The PauseMenu need not to refresh the window in order to displayed upon the game view
	if(!MenuManager::getInstance()->isDisplayingPauseState()) {
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
	//Save players data
	}else if (MenuManager::getInstance()->hasPlayerDataChanged()){
		m_pParserPlayer->savePlayerData(std::make_pair(MenuManager::getInstance()->getLastPlayer(), MenuManager::getInstance()->getPlayers()));
		MenuManager::getInstance()->setHasPlayerDataChanged(false);
		std::pair<Player*, std::vector<Player*>> playerData = m_pParserPlayer->loadPlayerData();
		MenuManager::getInstance()->reloadData(playerData);
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

	// Reset the camera
	m_pRender->setZoom(m_iGameScale);

	if(EntityManager::getInstance()->isPowerExisting(PowerType::HeadacheType))
		m_pRender->setCameraAngle(static_cast<Headache*>(EntityManager::getInstance()->getPower(PowerType::HeadacheType))->getInterpolateAngle());

	// If no game have been created before then create a new one (from the main menu)
	if (m_pParserLevel == NULL) {
		EntityManager::getInstance()->initRender(m_pRender);
		m_pParserLevel = new ParserLevel();
		m_sCurrentLevel = MenuManager::getInstance()->getLevelToLoad();

		//Starts the kinematics
		if(m_sCurrentLevel.c_str() == m_levelList.front() || (m_sCurrentLevel.c_str() == m_levelList.back() && m_bIsLevelFinished)){
			createKinematic();
			m_bIsInGame = true;
		}

		//Load level with no previous game (from the menu for example)
		if(!m_bIsPlayingKinematic){
			loadLevel(m_sCurrentLevel.c_str());
			loadPhysics();
			m_bIsInGame = true;
		}
	}
	// If the Player has finished the current level, then load the following
	else if(m_bIsLevelFinished){
		for (unsigned int i = 0; i < m_levelList.size(); ++i){
			if (m_sCurrentLevel == m_levelList[i]){
				if(i+1 == m_levelList.size()){
					if(m_bHasKinematicBeenPlayed){
						fprintf(stderr, "You reached the latest level ! Back to menus.\n");
						clear();
						switchToMenu();
					}else{
						PhysicalEntity::clearMovableObjectArray();
						EntityManager::getInstance()->deleteAllEntities();
						EntityManager::getInstance()->deleteAllPowers();
						EntityManager::getInstance()->initRender(m_pRender);
						m_pRender->setCameraAngle(0);
						m_pRender->setZoom(m_iMenuScale);
						m_pRender->setCameraPosition(g_WindowWidth/2, g_WindowHeight/2);
						createKinematic();
						m_bIsInGame = true;
					}
				}
				else {
					m_sCurrentLevel = m_levelList[i+1];
					// Load player data
					std::pair<Player*, std::vector<Player*>> playerData = m_pParserPlayer->loadPlayerData();
					
					// Save player data
					if(playerData.first->getCurrentLevel() < i+2){
						playerData.first->setCurrentLevel(i+2);
						m_pParserPlayer->savePlayerData(playerData);
					}

					// Load next level
					loadLevel(m_sCurrentLevel.c_str());
					loadPhysics();
					fprintf(stderr, "Next Level loaded \n");
					m_bIsLevelFinished = false;
					m_bIsInGame = true;
					break;
				}
			}
		}
	//Load the level after a kinematic
	}else if(m_bHasKinematicBeenPlayed){
		loadLevel(m_sCurrentLevel.c_str());
		loadPhysics();
		m_bIsInGame = true;
	}
	// If the player resume game from the pause menu
	else{
		m_bIsInGame = true;
	}
}

void GameManager::switchToMenu() {
	// Reset Camera
 	m_pRender->setZoom(m_iMenuScale);
	m_pRender->setCameraAngle(0);

	// If the MenuManager doesn't exists, means at the first launch or when the user quit the game, then create it.
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

	// Background Music
	SoundManager::getInstance()->loop(EntityManager::getInstance()->getBackgroundMusic().at(0)->getSound());
	SoundManager::getInstance()->playSound(EntityManager::getInstance()->getBackgroundMusic().at(0)->getSound());
	
	// Reset Camera
	m_pRender->setZoom(m_iGameScale);
	m_pRender->setCameraAngle(0);

	// Set enter and exit of level
	m_fExitX = EntityManager::getInstance()->getExitCoordinates()[0];
	m_fExitY = EntityManager::getInstance()->getExitCoordinates()[1];
}

void GameManager::reloadLevel(){
	loadLevel(m_sCurrentLevel.c_str());
	loadPhysics();
	m_bIsInGame = true;
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

void GameManager::loadPhysics() {
	m_fT = 1.f/60.f;
	m_fForceFactor = 20.f;
	m_pPhysicalDino =  EntityManager::getInstance()->getPhysicalDino();
	m_fImpulse = m_pPhysicalDino->getMass() * m_fForceFactor;
	float gravity = EntityManager::getInstance()->getPhysicalWorld()->getGravity().y;
	m_fJumpForce = sqrt(gravity*m_fImpulse) / m_fT;
}

}

