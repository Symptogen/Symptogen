#include <iostream>
#include "GameManager.h"

#ifdef _WIN32
int IndieLib()
#elif __linux__
Indielib_Main
#endif
{
	
	/* ------ Game intialization ----- */
	Symp::GameManager::getInstance()->startMainLoop();
	Symp::GameManager::removeInstance();
	
	return 0;
}