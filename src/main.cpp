// This file was created the 3-12-2013 by Cecilia Lejeune
// in order to test the portability of Indielib game engine.
// Supposed to display the Dino picture on a background

#include <iostream>
//FMOD
#include <fmod.h>
#include <fmod_errors.h>
//Box2D + facade
#include <Box2D/Box2D.h>
#include "PhysicalEntity.h"
//IndieLib + facade
#include <Indie.h>
#include "RenderEntity.h"
//Game
#include "GameManager.h"

/*================== Main ==================*/

#ifdef _WIN32
int IndieLib()
#elif __linux__
Indielib_Main
#endif
{
	// ----- Game intialization -----
	GameManager* pGameManager = new GameManager("Symptogen", 800, 600, 32, 0, 0, 1);
	b2World* world = pGameManager->getPhysicalManager()->getWorld();
	
	// ----- PHYSIC DATA -----
	//static body => ground
	PhysicalEntity* pGround = new PhysicalEntity(world, 400.f, 300.f);
	pGround->setHitBox(200.0f, 200.0f, 0.f, 0.f);
	pGround->setActive(false);
	//dynamic body => rabbit1
	PhysicalEntity* pRabbit1 = new PhysicalEntity(world, 400.f, 300.f);
	pRabbit1->setHitBox(1.0f, 1.0f, 1.0f, 0.3f);
	//dynamic body => rabbit2
	PhysicalEntity* pRabbit2 = new PhysicalEntity(world, 200.f, 300.f);
	pRabbit2->setHitBox(1.0f, 1.0f, 1.0f, 0.3f);

	// ----- RENDER DATA -----
	// background
	RenderEntity* rBack = new RenderEntity("../assets/cave.png", Surface);
	rBack->setHotSpot(0.5f, 0.5f);
	//rBack->setScale(1.7f, 1.7f);
	// Creating 2d entity for the Rabbit1
	RenderEntity *rRabbit1 = new RenderEntity("../assets/rabbit_animation.xml", Animation);
	rRabbit1->setHotSpot(0.5f, 0.5f);
	rRabbit1->setSequence(0); //sequence "rabbit_flash_normal" in rabbit_anmaition.xml
	// Creating 2d entity for the Rabbit2
	RenderEntity *rRabbit2 = new RenderEntity("../assets/rabbit_animation.xml", Animation);
	rRabbit2->setHotSpot(0.5f, 0.5f);
	rRabbit2->setSequence(1); //sequence "rabbit_flash_fast" in rabbit_anmaition.xml

	// ----- ADD ENTITIES TO THE MANAGER -----
	pGameManager->getEntityManager()->addEntity(rBack, 0, pGround);
	pGameManager->getEntityManager()->addEntity(rRabbit1, 0, pRabbit1);
	pGameManager->getEntityManager()->addEntity(rRabbit2, 0, pRabbit2);

	// ----- MAIN LOOP -----
	pGameManager->update();
	
	return 0;
}