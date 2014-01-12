// This file was created the 3-12-2013 by Cecilia Lejeune
// in order to test the portability of Indielib game engine.
// Supposed to display the Dino picture on a background


#include <iostream>
//FMOD
#include <fmod.h>
#include <fmod_errors.h>
//Box2D
#include <Box2D/Box2D.h>
//IndieLib
#include <Indie.h>
//facade IndieLib
#include "Window.h"
#include "Render.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "RenderEntity.h"



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
	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	//Print state of the b2Body
	groundBody->Dump();

	
	// ----- IndieLib intialization -----
	IndieLib::init(IND_DEBUG_MODE);
	Window* window = new Window();
	Render* render = new Render();
	window->setWindow(render->init("Symptogen", 800, 600, 32, 0, 0, 1));
	window->setCursor(true);
	InputManager* inputManager = new InputManager(render);
 	
	// Creating 2d entities
	EntityManager* entityManager = new EntityManager(render);
	
	// ----- Background -----
	RenderEntity* mBack = new RenderEntity("../assets/cave.png", Surface);
	entityManager->addRenderEntity(mBack, 0);
	mBack->setHotSpot(0.5f, 0.5f);
	mBack->setPosition(400, 300, 0);
	mBack->setScale(1.7f, 1.7f);

	// Creating 2d entity for the Rabbit1
	RenderEntity *mRabbit = new RenderEntity("../assets/rabbit_animation.xml", Animation);
	entityManager->addRenderEntity(mRabbit, 0);
	mRabbit->setHotSpot(0.5f, 0.5f);
	mRabbit->setPosition(400, 200, 0);
	mRabbit->setSequence(0); //sequence "rabbit_flash_normal" in rabbit_anmaition.xml
	//mRabbit->setMirrorX(1); //fucked the collisions !

	// Creating 2d entity for the Rabbit2
	RenderEntity *mRabbit2 = new RenderEntity("../assets/rabbit_animation.xml", Animation);
	entityManager->addRenderEntity(mRabbit2, 0);
	mRabbit2->setHotSpot(0.5f, 0.5f);
	mRabbit2->setPosition(400, 100, 0);
	mRabbit2->setSequence(1); //sequence "rabbit_flash_fast" in rabbit_anmaition.xml

	int stepMov = 5;
	// ----- Main Loop -----
	while (!inputManager->onKeyPress (IND_ESCAPE) && !inputManager->quit())
	{
		// ----- Input Update ----
		inputManager->update();
 
		// -------- Render -------
		/***** Déplacements Rabbit1 *****/
		if (inputManager->isKeyPressed(IND_KEYRIGHT)){
			if (!entityManager->getIND_Entity2dManager()->isCollision(mRabbit->getIND_Entity2d(), "rabbit_right", mRabbit2->getIND_Entity2d(), "*")){
				mRabbit->setPosition(mRabbit->getPosX() + stepMov, mRabbit->getPosY(), 0);
			}
		}
		if (inputManager->isKeyPressed(IND_KEYLEFT)){
			if (!entityManager->getIND_Entity2dManager()->isCollision(mRabbit->getIND_Entity2d(), "rabbit_left", mRabbit2->getIND_Entity2d(), "*")){
				mRabbit->setPosition(mRabbit->getPosX() - stepMov, mRabbit->getPosY(), 0);
			}

		}
		if (inputManager->isKeyPressed(IND_KEYUP)){
			if (!entityManager->getIND_Entity2dManager()->isCollision(mRabbit->getIND_Entity2d(), "rabbit_up", mRabbit2->getIND_Entity2d(), "*")){
				mRabbit->setPosition(mRabbit->getPosX(), mRabbit->getPosY() - stepMov, 0);
			}
		}
		if (inputManager->isKeyPressed(IND_KEYDOWN)){
			if (!entityManager->getIND_Entity2dManager()->isCollision(mRabbit->getIND_Entity2d(), "rabbit_down", mRabbit2->getIND_Entity2d(), "*")){
				mRabbit->setPosition(mRabbit->getPosX(), mRabbit->getPosY() + stepMov, 0);
			}
		}

		/***** Déplacements Rabbit2 *****/
		if (inputManager->isKeyPressed(IND_D)){
			if (!entityManager->getIND_Entity2dManager()->isCollision(mRabbit2->getIND_Entity2d(), "rabbit_right", mRabbit->getIND_Entity2d(), "*")){
				mRabbit2->setPosition(mRabbit2->getPosX() + stepMov, mRabbit2->getPosY(), 0);
			}
		}
		if (inputManager->isKeyPressed(IND_Q)){
			if (!entityManager->getIND_Entity2dManager()->isCollision(mRabbit2->getIND_Entity2d(), "rabbit_left", mRabbit->getIND_Entity2d(), "*")){
				mRabbit2->setPosition(mRabbit2->getPosX() - stepMov, mRabbit2->getPosY(), 0);
			}
		}
		if (inputManager->isKeyPressed(IND_Z)){
			if (!entityManager->getIND_Entity2dManager()->isCollision(mRabbit2->getIND_Entity2d(), "rabbit_up", mRabbit->getIND_Entity2d(), "*")){
				mRabbit2->setPosition(mRabbit2->getPosX(), mRabbit2->getPosY() - stepMov, 0);
			}
		}
		if (inputManager->isKeyPressed(IND_S)){
			if (!entityManager->getIND_Entity2dManager()->isCollision(mRabbit2->getIND_Entity2d(), "rabbit_down", mRabbit->getIND_Entity2d(), "*")){
				mRabbit2->setPosition(mRabbit2->getPosX(), mRabbit2->getPosY() + stepMov, 0);
			}
		}
 
		//Lecture du son test
		if (inputManager->isKeyPressed(IND_SPACE)){
			FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, test, 0, NULL);
		}

		render->clearViewPort(60, 60, 60);
		render->beginScene();
		entityManager->renderEntities();
		//_entity2dManager->renderCollisionAreas(255, 0, 0, 255); // To see the collisions areas
		render->endScene();
	}
 
	// ----- Indielib End -----
	IndieLib::end();
 
	//FMOD
	/* On libère le son et on ferme et libère l'objet système */
    FMOD_Sound_Release(test);
    FMOD_System_Close(system);
    FMOD_System_Release(system);

	return 0;
}
