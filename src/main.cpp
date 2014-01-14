#include <iostream>

//FMOD
#include <fmod.h>
#include <fmod_errors.h>

// Box2D + facade
#include <Box2D/Box2D.h>

//Game
#include "GameManager.h"
#include "LevelManager.h"

/*================== Main ==================*/

#ifdef _WIN32
int IndieLib()
#elif __linux__
Indielib_Main
#endif
{
	
	// ----- Fmod test ----- 
	FMOD_SYSTEM *system;
    FMOD_SOUND *test;
    
    FMOD_RESULT resultat;
    // Création et initialisation d'un objet système
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
    // Chargement du son et vérification du chargement
    resultat = FMOD_System_CreateSound(system, "../assets/audio/test.wav", FMOD_CREATESAMPLE, 0, &test);
    if (resultat != FMOD_OK)
    {
        std::cerr << "Impossible de lire le son test" << std::endl;
        exit(EXIT_FAILURE);
    }

	// ----- Game intialization -----
	Symp::GameManager* pGameManager = new Symp::GameManager("Symptogen", 800, 600, 32, 0, 0, 1);
	
	// ----- PHYSIC DATA -----
	b2World* world = pGameManager->getEntityManager()->getPhysicalManager()->getWorld();


	// ----- MAIN LOOP -----
	pGameManager->update();
	//FMOD
	/* On libère le son et on ferme et libère l'objet système */
    /*FMOD_Sound_Release(test);
    FMOD_System_Close(system);
    FMOD_System_Release(system);*/


	// ----- MAIN LOOP -----
	pGameManager->startGame();

	return 0;
}
