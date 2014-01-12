// This file was created the 3-12-2013 by Cecilia Lejeune
// in order to test the portability of Indielib game engine.
// Supposed to display the Dino picture on a background


#include <iostream>
//FMOD
#include <fmod.h>
#include <fmod_errors.h>
//Box2D
#include <Box2D/Box2D.h>
//facade Box2D
#include "PhysicalEntity.h"
//IndieLib
#include <Indie.h>
//facade IndieLib
#include "GameManager.h"



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
    /* Création et initialisation d'un objet système */
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
    /* Chargement du son et vérification du chargement */
    resultat = FMOD_System_CreateSound(system, "../assets/audio/test.wav", FMOD_CREATESAMPLE, 0, &test);
    if (resultat != FMOD_OK)
    {
        std::cerr << "Impossible de lire le son test" << std::endl;
        exit(EXIT_FAILURE);
    }


	// ----- Box2D test -----
	// Construct a world object, which will hold and simulate the rigid bodies.
	b2Vec2 gravity(0.0f, -10.0f);
	b2World world(gravity); 
	
	//static body => ground
	PhysicalEntity* pGround = new PhysicalEntity(&world, 0.0f, -10.0f);
	pGround->setHitBox(50.0f, 10.0f, 0.f, 0.f);
	pGround->display();

	//dynamic body => rabbit1
	PhysicalEntity* pRabbit1 = new PhysicalEntity(&world, 0.0f, 4.0f);
	pRabbit1->setHitBox(1.0f, 1.0f, 1.0f, 0.3f);
	pRabbit1->display();
	
	// ----- Game intialization -----
	GameManager* pGameManager = new GameManager("Symptogen", 800, 600, 32, 0, 0, 1);
	
	// ----- Background -----
	RenderEntity* mBack = new RenderEntity("../assets/cave.png", Surface);
	pGameManager->getEntityManager()->addRenderEntity(mBack, 0);
	mBack->setHotSpot(0.5f, 0.5f);
	mBack->setPosition(400, 300, 0);
	mBack->setScale(1.7f, 1.7f);

	// Creating 2d entity for the Rabbit1
	RenderEntity *mRabbit = new RenderEntity("../assets/rabbit_animation.xml", Animation);
	pGameManager->getEntityManager()->addRenderEntity(mRabbit, 0);
	mRabbit->setHotSpot(0.5f, 0.5f);
	mRabbit->setPosition(400, 200, 0);
	mRabbit->setSequence(0); //sequence "rabbit_flash_normal" in rabbit_anmaition.xml
	//mRabbit->setMirrorX(1); //fucked the collisions !

	// Creating 2d entity for the Rabbit2
	RenderEntity *mRabbit2 = new RenderEntity("../assets/rabbit_animation.xml", Animation);
	pGameManager->getEntityManager()->addRenderEntity(mRabbit2, 0);
	mRabbit2->setHotSpot(0.5f, 0.5f);
	mRabbit2->setPosition(400, 100, 0);
	mRabbit2->setSequence(1); //sequence "rabbit_flash_fast" in rabbit_anmaition.xml

	// MAIN LOOP
	pGameManager->update();
  
	//FMOD
	/* On libère le son et on ferme et libère l'objet système */
    FMOD_Sound_Release(test);
    FMOD_System_Close(system);
    FMOD_System_Release(system);

	return 0;
}