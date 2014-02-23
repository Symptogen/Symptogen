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

	InputManager::getInstance();
	InputManager::initRender(m_pRender);
	m_pParser = new Parser("../assets/data.xml");
	//m_pSoundManager = new SoundManager();
	//m_pSoundManager->loadSound("../assets/audio/test.wav"); //test sound

	//Initialize the game elements to null and start the menu
	m_pLevelManager = nullptr;
	m_bIsMenu = false;
	switchToMenu();
}

GameManager::~GameManager(){
	IndieLib::end();
	delete m_pWindow;
	delete m_pRender;
	InputManager::removeInstance();
	MenuManager::removeInstance();
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
	MenuManager::getInstance()->setLevelChoosen(false);

	EntityManager::getInstance();
	
	if (m_pLevelManager == nullptr) {
		EntityManager::initRender(m_pRender);
		//If no game have been created before then create a new one (from the main menu)
		m_pLevelManager = new LevelManager();
		loadLevel(MenuManager::getInstance()->getLevelToLoad().c_str());
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
	EntityManager::getInstance()->deleteAllEntities();
	delete m_pLevelManager;
	MenuManager::removeInstance();
	m_bIsMenu = false;
	EntityManager::removeInstance();
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
	if (m_bIsMenu == false){

		// Retrive data from the player data file
		std::pair<Player*, std::vector<Player*>> playerData = m_pParser->loadPlayerData();

		// Start the menus
		MenuManager::getInstance();
		MenuManager::init(m_pRender, playerData);
		m_bIsMenu = true;

 		// Manage Camera
		m_pRender->setCamera();
 		m_pRender->setCameraPosition(m_pWindow->getIND_Window()->getWidth()*0.5, m_pWindow->getIND_Window()->getHeight()*0.5);
 	
 	}else {
 		// Pause menu
 		RenderEntity* pDino = EntityManager::getInstance()->getRenderDino();
 		PauseMenu* pPauseMenu = new PauseMenu(MenuManager::getInstance(), pDino->getPosX(), pDino->getPosY());
 		MenuManager::getInstance()->setState(pPauseMenu);
 		
 	}

 	m_bIsInGame = false;
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
	PhysicalEntity* pDino = EntityManager::getInstance()->getPhysicalDino();
	//debug : velocity of dino
	//std::cout << pDino->getLinearVelocity().x << " - " << pDino->getLinearVelocity().y << std::endl;
	float impulse = pDino->getMass();
	if (InputManager::getInstance()->isKeyPressed(IND_KEYLEFT)){
		pDino->getb2Body()->ApplyLinearImpulse(b2Vec2(-impulse-pDino->getLinearVelocity().x, 0.f), pDino->getb2Body()->GetWorldCenter(), pDino->isAwake());
	}
	else if (InputManager::getInstance()->isKeyPressed(IND_KEYRIGHT)){
		pDino->getb2Body()->ApplyLinearImpulse(b2Vec2(impulse+pDino->getLinearVelocity().x, 0.f), pDino->getb2Body()->GetWorldCenter(), pDino->isAwake());
	}
	else if (InputManager::getInstance()->isKeyPressed(IND_KEYUP)){
		float force = pDino->getMass() / (1/60.0); //f = mv/t
	    pDino->getb2Body()->ApplyForce(b2Vec2(0,-force), pDino->getb2Body()->GetWorldCenter(), pDino->isAwake());
	}
	else if (InputManager::getInstance()->isKeyPressed(IND_KEYDOWN)){
		pDino->getb2Body()->ApplyLinearImpulse(b2Vec2(0.f, impulse+pDino->getLinearVelocity().y), pDino->getb2Body()->GetWorldCenter(), pDino->isAwake());
	}

	// The following condition manage the camera zoom (for debug)
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

	//manage Camera
	m_pRender->setCamera();
	m_pRender->setCameraPosition(pDino->getPosition().x, pDino->getPosition().y);
	
	//update all list of entities
	EntityManager::getInstance()->updateEntities();
	
	//sound
	if (InputManager::getInstance()->isKeyPressed(IND_SPACE)){
		//m_pSoundManager->play(0); //seg fault : still no song !
	}

	//render openGL
	m_pRender->clearViewPort(60, 60, 60);
	m_pRender->beginScene();
		EntityManager::getInstance()->renderEntities();
		//test hitbox
		displayHitboxes();
	m_pRender->endScene();

	//Pause
	if (InputManager::getInstance()->onKeyPress(IND_ESCAPE)){
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

void GameManager::loadLevel(const char* mapFile) {
	EntityManager::getInstance()->deleteAllEntities();
	EntityManager::getInstance()->addDino(-500, 0);
	m_pLevelManager->loadLevel(mapFile);
}

void GameManager::displayHitboxes() {
	for (unsigned int idEntity = 0; idEntity < EntityManager::getInstance()->getPhysicalEntityArray().size(); ++idEntity) {
		PhysicalEntity* pEntity = EntityManager::getInstance()->getPhysicalEntityArray()[idEntity];
		if(pEntity != NULL) {
			b2Vec2 topleft;
			topleft.x = pEntity->getPosition().x - pEntity->getWidth()/2;
			topleft.y = pEntity->getPosition().y + pEntity->getHeight()/2;
			b2Vec2 botright;
			botright.x = pEntity->getPosition().x + pEntity->getWidth()/2;
			botright.y = pEntity->getPosition().y - pEntity->getHeight()/2;
			m_pRender->getIND_Render()->blitRectangle(topleft.x, topleft.y, botright.x, botright.y, 255, 0, 0, 255);
		}
	}
}

}
