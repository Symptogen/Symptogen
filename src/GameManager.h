#ifndef _H_SYMPTOGEN_RENDER_LINK_PHYSICAL_GAME_MANAGER_H_
#define _H_SYMPTOGEN_RENDER_LINK_PHYSICAL_GAME_MANAGER_H_

#include "Window.h"
#include "Render.h"
#include "InputManager.h"
//#include "SoundManager.h"
#include "EntityManager.h"

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

	Window* 			getWindow(){return m_pWindow;}
	Render* 			getRender(){return m_pRender;}
	InputManager* 		getInputManager(){return m_pInputManager;}
	//SoundManager* 		getSoundManager(){return m_pSoundManager;}
	EntityManager* 		getEntityManager(){return m_pEntityManager;}
	bool				getIsInGame(){return m_bIsInGame;}

private:
	Window* 			m_pWindow;
	Render* 			m_pRender;
	InputManager* 		m_pInputManager;
	//SoundManager*		m_pSoundManager;
	EntityManager* 		m_pEntityManager;

	bool 				m_bIsInGame;
};

}

#endif //_H_SYMPTOGEN_RENDER_LINK_PHYSICAL_GAME_MANAGER_H_