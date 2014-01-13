#include <iostream>
//physics
#include "PhysicalEntity.h"
//render
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
	Symp::GameManager* pGameManager = new Symp::GameManager("Symptogen", 800, 600, 32, 0, 0, 1);
	
	// ----- PHYSIC DATA -----
	//static body => ground
	Symp::PhysicalEntity* pGround = new Symp::PhysicalEntity(pGameManager->getEntityManager()->getPhysicalManager()->getWorld(), 400.f, 300.f);
	pGround->setHitBox(200.0f, 200.0f, 0.f, 0.f);
	pGround->setActive(false);
	//dynamic body => rabbit1
	Symp::PhysicalEntity* pRabbit1 = new Symp::PhysicalEntity(pGameManager->getEntityManager()->getPhysicalManager()->getWorld(), 400.f, 300.f);
	pRabbit1->setHitBox(1.0f, 1.0f, 1.0f, 0.3f);
	//dynamic body => rabbit2
	Symp::PhysicalEntity* pRabbit2 = new Symp::PhysicalEntity(pGameManager->getEntityManager()->getPhysicalManager()->getWorld(), 200.f, 300.f);
	pRabbit2->setHitBox(1.0f, 1.0f, 1.0f, 0.3f);

	// ----- RENDER DATA -----
	// background
	Symp::RenderEntity* rBack = new Symp::RenderEntity("../assets/cave.png", Symp::Surface);
	rBack->setHotSpot(0.5f, 0.5f);
	//rBack->setScale(1.7f, 1.7f);
	// Creating 2d entity for the Rabbit1
	Symp::RenderEntity *rRabbit1 = new Symp::RenderEntity("../assets/rabbit_animation.xml", Symp::Animation);
	rRabbit1->setHotSpot(0.5f, 0.5f);
	rRabbit1->setSequence(0); //sequence "rabbit_flash_normal" in rabbit_anmaition.xml
	// Creating 2d entity for the Rabbit2
	Symp::RenderEntity *rRabbit2 = new Symp::RenderEntity("../assets/rabbit_animation.xml", Symp::Animation);
	rRabbit2->setHotSpot(0.5f, 0.5f);
	rRabbit2->setSequence(1); //sequence "rabbit_flash_fast" in rabbit_anmaition.xml

	// ----- ADD ENTITIES TO THE MANAGER -----
	pGameManager->getEntityManager()->addEntity(rBack, 0, pGround);
	pGameManager->getEntityManager()->addEntity(rRabbit1, 0, pRabbit1);
	pGameManager->getEntityManager()->addEntity(rRabbit2, 0, pRabbit2);

	// ----- MAIN LOOP -----
	pGameManager->startGame();

	return 0;
}