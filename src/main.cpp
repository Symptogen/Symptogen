#include <iostream>

#include <fmod.h>
#include <fmod_errors.h>

#include <Box2D/Box2D.h>

#include "GameManager.h"
#include "sound/SoundManager.h"

#ifdef _WIN32
int IndieLib()
#elif __linux__
Indielib_Main
#endif
{
	
	/* ------ Game intialization ----- */
	// Symp::GameManager* pGameManager = new Symp::GameManager("Symptogen", 800, 600, 32, 0, 0, 1);
	// pGameManager->startMainLoop();

	Symp::GameManager::getInstance()->startMainLoop();

	return 0;
}