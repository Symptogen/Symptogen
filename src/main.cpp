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
	
	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	// Define the ground box shape.
	b2PolygonShape groundBox;
	// The extents are the half-widths of the box.
	groundBox.SetAsBox(50.0f, 10.0f);
	// Add the ground fixture to the ground body.
	groundBody->CreateFixture(&groundBox, 0.0f);
	//Print state of the b2Body
	groundBody->Dump();

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	b2Body* body = world.CreateBody(&bodyDef);
	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);
	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;
	// Override the default friction.
	fixtureDef.friction = 0.3f;
	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

	//Print state of the b2Body
	body->Dump();

	
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
