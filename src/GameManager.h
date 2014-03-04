#ifndef _H_SYMPTOGEN_RENDER_LINK_PHYSICAL_GAME_MANAGER_H_
#define _H_SYMPTOGEN_RENDER_LINK_PHYSICAL_GAME_MANAGER_H_

#include "render/Window.h"
#include "render/Render.h"
#include "input/InputManager.h"
#include "sound/SoundManager.h"
#include "persistence/Parser.h"
#include "menu/MenuManager.h"
#include "EntityManager.h"
#include "util/Singleton.h"

namespace Symp {

/* *************************************************************************************** */
/* CLASS DEFINITION */
/* *************************************************************************************** */
/**
*	Manages the loop of the game.
*	It has to be a singleton.
*	It has the role of a dispatcher, distributing tasks to other objects (Render, LevelManager, Parser...).
*	It provides tools to : 
*		- Manage main loop
*		- Switch to menu
*		- Switch to game
*/

class GameManager : public Singleton<GameManager>{

	// Friend to use private constructor/destructor
	friend class Singleton<GameManager>;

public:

	/**
	* @brief Clear the game entities and attributes for displaying the main menu
	* @see MenuManager
	* @see EntityManager
	* @see LevelManager
	* @see updateMenu()
	* @see GameManager()
	* @see ~GameManager()
	*/
	void clear();

	/**
	* @brief The main loop of the game responsible the application to run
	* @see updateMenu()
	* @see updateGame()
	* @see switchToGame()
	* @see switchToMenu()
	* @see GameManager()
	* @see ~GameManager()
	*/
	void startMainLoop();

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
	void updateGame();

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
	void updateMenu();

	/**
	* @brief Displays the game when it needs to be
	* This function displays the game, usually called from a menu.
	* @see MenuManager
	* @see updateGame()
	* @see switchToMenu()
	* @see GameManager()
	* @see ~GameManager()
	*/
	void switchToGame();

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
	void switchToMenu();

	/**
	* @brief Delete all existing entities and powers, and load the mapFile by calling the parser.
	* @see Parser
	*/
	void loadLevel(const char* mapFile);

	/**
	* @brief Debug tool, useful to display hitboxes and center of all physical entities.
	* @see EntityManager
	*/
	void debugPhysicalEntities();

	/**
	* @brief Debug tool, useful to display the size and the center of all render entities.
	* @see EntityManager
	*/
	void debugRenderEntities();

	/**
	* @brief Reload physical value at the beginning or after death and reload level
	*/
	void loadPhysics();

	/**
	*	Getters
	*/
	Window* 	getWindow() const {return m_pWindow;}
	Render* 	getRender() const {return m_pRender;}
	bool		getIsInGame() const {return m_bIsInGame;}	

private:
	Window* 			m_pWindow;
	Render* 			m_pRender;
	LevelManager*		m_pLevelManager;
	Parser*				m_pParser;
	PowerType			m_dinoState;
	PhysicalEntity* 	m_pPhysicalDino;

	bool 				m_bIsInGame;
	bool 				m_bIsMenu;
	bool 				m_bIsLevelFinished;
	bool 				m_bIsPlayerDead;

	std::vector<std::string> 	m_levelList;
	std::string 				m_sCurrentLevel;

	int 						m_iGameScale;
	int 						m_iMenuScale;

	//Physics
	float 	m_fForceFactor;
	float 	m_fImpulse;
	float 	m_Ft;
	float 	m_fGravity; 
	float 	m_fJumpForce;


	/**
	* @brief Initialize the engine.
	* Set the Window (title, size...), load the xml of player's data, initialize the game elements to NULL and start the menu.
	* @see Window
	* @see InputManager
	* @see Parser
	* @see SoundManager
	*/
	GameManager();

	/**
	* @brief Delete the dispatcher.
	* Deallocate InputManager and MenuManager (both Singleton).
	* @see InputManager
	* @see MenuManager
	*/
	~GameManager();

};

}

#endif //_H_SYMPTOGEN_RENDER_LINK_PHYSICAL_GAME_MANAGER_H_