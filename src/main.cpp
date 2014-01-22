#include <iostream>

#include <fmod.h>
#include <fmod_errors.h>

#include <Box2D/Box2D.h>

#include "PhysicalEntity.h"
#include "RenderEntity.h"
#include "GameManager.h"
#include "LevelManager.h"

#ifdef _WIN32
int IndieLib()
#elif __linux__
Indielib_Main
#endif
{
	
	// Sound
	FMOD_SYSTEM *system;
    FMOD_SOUND *test;

    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);

	const char* soundFile = "../assets/audio/test.wav";
    FMOD_RESULT result = FMOD_System_CreateSound(system, soundFile, FMOD_CREATESAMPLE, 0, &test);
    if (result != FMOD_OK)
    {
        std::cerr << "Main : Impossible to read " << soundFile << ". The program will end" << std::endl;
        exit(EXIT_FAILURE);
    }

	// ----- Game intialization -----
	Symp::GameManager* pGameManager = new Symp::GameManager("Symptogen", 800, 600, 32, 0, 0, 1);

	// ----- PHYSIC DATA -----
	b2World* world = pGameManager->getEntityManager()->getPhysicalManager()->getWorld();
	//static body => ground
	Symp::PhysicalEntity* pGround = new Symp::PhysicalEntity(world, b2Vec2(100.0f, 1.0f));
	pGround->setMass(0.f, 0.f);
	pGround->setPosition(0.f, -50.f);
	//dynamic body => rabbit1
	Symp::PhysicalEntity* pRabbit1 = new Symp::PhysicalEntity(world, b2Vec2(100.f, 100.f));
	pRabbit1->setMass(100.f, 100.f);
	pRabbit1->setPosition(400.f, 200.f);
	//dynamic body => rabbit2
	Symp::PhysicalEntity* pRabbit2 = new Symp::PhysicalEntity(world, b2Vec2(10.f, 10.f));
	pRabbit2->setMass(10.f, 10.f);
	pRabbit2->setPosition(400.f, 300.f);

	// ----- RENDER DATA -----
	// background
	Symp::RenderEntity* rBack = new Symp::RenderEntity("../assets/cave.png", Symp::Surface);
	rBack->setHotSpot(0.5f, 0.5f);
	rBack->setPosition(0.f, 300.f, 0.f);
	// Creating 2d entity for the Rabbit1
	Symp::RenderEntity *rRabbit1 = new Symp::RenderEntity("../assets/rabbit_animation.xml", Symp::Animation);
	rRabbit1->setHotSpot(0.5f, 0.5f);
	rRabbit1->setSequence(0); //sequence "rabbit_flash_normal" in rabbit_anmaition.xml
	// Creating 2d entity for the Rabbit2
	Symp::RenderEntity *rRabbit2 = new Symp::RenderEntity("../assets/rabbit_animation.xml", Symp::Animation);
	rRabbit2->setHotSpot(0.5f, 0.5f);
	rRabbit2->setSequence(1); //sequence "rabbit_flash_fast" in rabbit_anmaition.xml

	// ----- ADD ENTITIES TO THE MANAGER -----
	pGameManager->getEntityManager()->addRenderEntity(rBack, 0);
	pGameManager->getEntityManager()->addPhysicalEntity(pGround);
	pGameManager->getEntityManager()->addEntity(rRabbit1, 0, pRabbit1);
	pGameManager->getEntityManager()->addEntity(rRabbit2, 0, pRabbit2);

	// ------ Load Level ------------- 
	//pGameManager->loadLevel("../assets/map/map1.xml");

	// ----- MAIN LOOP -----
	std::cerr << "start game" << std::endl;
	pGameManager->startGame();

	// Release Data
	FMOD_Sound_Release(test);
	FMOD_System_Close(system);
	FMOD_System_Release(system);

	return 0;
}