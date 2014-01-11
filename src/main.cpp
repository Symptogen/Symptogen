// This file was created the 3-12-2013 by Cecilia Lejeune
// in order to test the portability of Indielib game engine.
// Supposed to display the Dino picture on a background


#include <iostream>

//IndieLib
/*#include "CIndieLib.h"
#include <IND_Surface.h>
#include <IND_Entity2d.h>
#include <IND_Image.h>
#include <IND_Animation.h>*/

#include "EntityManager.h"
#include "LevelManager.h"


/*================== Main ==================*/

#ifdef _WIN32
int IndieLib()
#elif __linux__
Indielib_Main
#endif
{

	symptogen::EntityManager entityManager;
	symptogen::LevelManager lvlManager;
	lvlManager.loadLevel(entityManager, "../assets/map/map1.xml");
	entityManager.printEntities();
	

	return 0;
}
