#ifndef _H_SYMPTOGEN_RENDER_LINK_PHYSICAL_GAME_MANAGER_H_
#define _H_SYMPTOGEN_RENDER_LINK_PHYSICAL_GAME_MANAGER_H_

#include "render/Window.h"
#include "render/Render.h"
#include "InputManager.h"
#include "sound/SoundManager.h"
#include "EntityManager.h"
#include "Parser.h"
#include "menu/MenuManager.h"

namespace Symp {

/**
	Manager of the game.
*/
class GameManager {
public:
	GameManager(const char *title, int width, int height, int bpp, bool vsync, bool fs, bool dBuffer);
	~GameManager();

	void startMainLoop();
	void updateGame();
	void updateMenu();
	void loadLevel(const char* mapFile);
	void switchToGame();
	void switchToMenu();
	void clear();

	//Getters
	Window* 			getWindow() const {return m_pWindow;}
	Render* 			getRender() const {return m_pRender;}
	InputManager* 		getInputManager() const {return m_pInputManager;}
	SoundManager* 		getSoundManager() const {return m_pSoundManager;}
	MenuManager*		getMenuManager() const {return m_pMenuManager;}
	bool				getIsInGame() const {return m_bIsInGame;}

private:
	Window* 			m_pWindow;
	Render* 			m_pRender;
	InputManager* 		m_pInputManager;
	SoundManager*		m_pSoundManager;
	LevelManager*		m_pLevelManager;
	Parser*				m_pParser;
	MenuManager*		m_pMenuManager;
	bool 				m_bIsInGame;
};

}

#endif //_H_SYMPTOGEN_RENDER_LINK_PHYSICAL_GAME_MANAGER_H_