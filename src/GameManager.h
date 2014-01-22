#ifndef _H_SYMPTOGEN_RENDER_LINK_PHYSICAL_GAME_MANAGER_H_
#define _H_SYMPTOGEN_RENDER_LINK_PHYSICAL_GAME_MANAGER_H_

#include "Window.h"
#include "Render.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "EntityManager.h"
#include "LevelManager.h"
#include "menu/MenuManager.h"

namespace Symp {

/**
	Manager of the game.
*/
class GameManager {
public:
	GameManager(const char *title, int width, int height, int bpp, bool vsync, bool fs, bool dBuffer);
	~GameManager();

	void startGame();
	void updateGame();
	void updateMenu();
	void loadLevel(const char* mapFile);

	//Setters
	void setIsInGame(bool isInGame) { m_bIsInGame = isInGame;}

	//Getters
	Window* 			getWindow() const {return m_pWindow;}
	Render* 			getRender() const {return m_pRender;}
	InputManager* 		getInputManager() const {return m_pInputManager;}
	SoundManager* 		getSoundManager() const {return m_pSoundManager;}
	EntityManager* 		getEntityManager() const {return m_pEntityManager;}
	MenuManager*		getMenuManager() const {return m_pMenuManager;}
	bool				getIsInGame() const {return m_bIsInGame;}

private:
	Window* 			m_pWindow;
	Render* 			m_pRender;
	InputManager* 		m_pInputManager;
	SoundManager*		m_pSoundManager;
	EntityManager* 		m_pEntityManager;
	LevelManager*		m_pLevelManager;
	MenuManager*		m_pMenuManager;
	bool 				m_bIsInGame;
};

}

#endif //_H_SYMPTOGEN_RENDER_LINK_PHYSICAL_GAME_MANAGER_H_