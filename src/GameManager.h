#ifndef _H_SYMPTOGEN_RENDER_LINK_PHYSICAL_GAME_MANAGER_H_
#define _H_SYMPTOGEN_RENDER_LINK_PHYSICAL_GAME_MANAGER_H_

#include "Window.h"
#include "Render.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "PhysicalManager.h"

/**
	Manager of the game.
*/
class GameManager {
public:
	GameManager(const char *title, int width, int height, int bpp, bool vsync, bool fs, bool dBuffer);
	~GameManager();

	//the main loop of the game
	void update();
	void updateGame();
	void updateMenu();

	//getters
	Window* 			getWindow(){return m_pWindow;}
	Render* 			getRender(){return m_pRender;}
	InputManager* 		getInputManager(){return m_pInputManager;}
	EntityManager* 		getEntityManager(){return m_pEntityManager;}
	PhysicalManager*	getPhysicalManager(){return m_pPhysicalManager;}
	bool				getIsInGame(){return m_bIsInGame;}

	//setters
	void setPhysicalManager(PhysicalManager* pPhysicalManager){m_pPhysicalManager = pPhysicalManager;}

private:
	Window* 			m_pWindow;
	Render* 			m_pRender;
	InputManager* 		m_pInputManager;
	EntityManager* 		m_pEntityManager;
	PhysicalManager*	m_pPhysicalManager;

	bool 			m_bIsInGame;
};

#endif //_H_SYMPTOGEN_RENDER_LINK_PHYSICAL_GAME_MANAGER_H_