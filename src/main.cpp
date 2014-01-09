// This file was created the 3-12-2013 by Cecilia Lejeune
// in order to test the portability of Indielib game engine.
// Supposed to display the Dino picture on a background


#include <iostream>
//FMOD
#include <fmod.h>
#include <fmod_errors.h>
#include <iostream>
//Box2D
#include <Box2D/Box2D.h>
//IndieLib
#include "CIndieLib.h"
#include <IND_Surface.h>
#include <IND_Entity2d.h>
#include <IND_Image.h>
#include <IND_Animation.h>

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
	CIndieLib *mI = CIndieLib::instance();
	if (!mI->init()) return 0;

	mI->_window->setTitle("Symptogen");
	mI->_window->cursor(true);
 	
 	//Load cave picture
	IND_Image *mImageCave = IND_Image::newImage();
	if (!mI->_imageManager->add(mImageCave, "../assets/cave.png")) return 0;

	// Loading Background
	IND_Surface *mSurfaceBack = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceBack, mImageCave, IND_OPAQUE, IND_32)) return 0;

	//Delete the images
	mI->_imageManager->remove(mImageCave);

	//Rabbit animations
	IND_Animation *mAnimationRabbit = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mAnimationRabbit, "../assets/rabbit_animation.xml", IND_ALPHA, IND_32)) return 0;
	
	// Creating 2d entity for the background
	// ----- Background -----
	RenderEntity* mBack = new RenderEntity();
	mBack->setSurface(mSurfaceBack);
	mBack->setHotSpot(0.5f, 0.5f);
	mBack->setPosition(400, 300, 0);
	mBack->setScale(1.7f, 1.7f);
	mI->_entity2dManager->add(mBack->getEntity2d());

	// Creating 2d entity for the Rabbit1
	RenderEntity *mRabbit = new RenderEntity();
	mRabbit->setAnimation(mAnimationRabbit);
	mRabbit->setHotSpot(0.5f, 0.5f);
	mRabbit->setPosition(400, 200, 0);
	mRabbit->setSequence(0); //sequence "rabbit_flash_normal" in rabbit_anmaition.xml
	//mRabbit->setMirrorX(1); //fucked the collisions !
	mI->_entity2dManager->add(mRabbit->getEntity2d());

	// Creating 2d entity for the Rabbit2
	RenderEntity *mRabbit2 = new RenderEntity();
	mRabbit2->setAnimation(mAnimationRabbit);
	mRabbit2->setHotSpot(0.5f, 0.5f);
	mRabbit2->setPosition(400, 100, 0);
	mRabbit2->setSequence(1); //sequence "rabbit_flash_fast" in rabbit_anmaition.xml
	mI->_entity2dManager->add(mRabbit2->getEntity2d());

	int stepMov = 5;
	// ----- Main Loop -----
	while (!mI->_input->onKeyPress (IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input Update ----
		mI->_input->update();
 
		// -------- Render -------
		/***** Déplacements Rabbit1 *****/
		if (mI->_input->isKeyPressed(IND_KEYRIGHT)){
			if (!mI->_entity2dManager->isCollision(mRabbit->getEntity2d(), "rabbit_right", mRabbit2->getEntity2d(), "*")){
				mRabbit->setPosition(mRabbit->getPosX() + stepMov, mRabbit->getPosY(), 0);
			}
		}
		if (mI->_input->isKeyPressed(IND_KEYLEFT)){
			if (!mI->_entity2dManager->isCollision(mRabbit->getEntity2d(), "rabbit_left", mRabbit2->getEntity2d(), "*")){
				mRabbit->setPosition(mRabbit->getPosX() - stepMov, mRabbit->getPosY(), 0);
			}

		}
		if (mI->_input->isKeyPressed(IND_KEYUP)){
			if (!mI->_entity2dManager->isCollision(mRabbit->getEntity2d(), "rabbit_up", mRabbit2->getEntity2d(), "*")){
				mRabbit->setPosition(mRabbit->getPosX(), mRabbit->getPosY() - stepMov, 0);
			}
		}
		if (mI->_input->isKeyPressed(IND_KEYDOWN)){
			if (!mI->_entity2dManager->isCollision(mRabbit->getEntity2d(), "rabbit_down", mRabbit2->getEntity2d(), "*")){
				mRabbit->setPosition(mRabbit->getPosX(), mRabbit->getPosY() + stepMov, 0);
			}
		}

		/***** Déplacements Rabbit2 *****/
		if (mI->_input->isKeyPressed(IND_D)){
			if (!mI->_entity2dManager->isCollision(mRabbit2->getEntity2d(), "rabbit_right", mRabbit->getEntity2d(), "*")){
				mRabbit2->setPosition(mRabbit2->getPosX() + stepMov, mRabbit2->getPosY(), 0);
			}
		}
		if (mI->_input->isKeyPressed(IND_Q)){
			if (!mI->_entity2dManager->isCollision(mRabbit2->getEntity2d(), "rabbit_left", mRabbit->getEntity2d(), "*")){
				mRabbit2->setPosition(mRabbit2->getPosX() - stepMov, mRabbit2->getPosY(), 0);
			}
		}
		if (mI->_input->isKeyPressed(IND_Z)){
			if (!mI->_entity2dManager->isCollision(mRabbit2->getEntity2d(), "rabbit_up", mRabbit->getEntity2d(), "*")){
				mRabbit2->setPosition(mRabbit2->getPosX(), mRabbit2->getPosY() - stepMov, 0);
			}
		}
		if (mI->_input->isKeyPressed(IND_S)){
			if (!mI->_entity2dManager->isCollision(mRabbit2->getEntity2d(), "rabbit_down", mRabbit->getEntity2d(), "*")){
				mRabbit2->setPosition(mRabbit2->getPosX(), mRabbit2->getPosY() + stepMov, 0);
			}
		}
 
		//Lecture du son test
		if (mI->_input->isKeyPressed(IND_SPACE)){
			FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, test, 0, NULL);
		}

		mI->_render->clearViewPort(60, 60, 60);
		mI->_render->beginScene();
		mI->_entity2dManager->renderEntities2d();
		//mI->_entity2dManager->renderCollisionAreas(255, 0, 0, 255); // To see the collisions areas
		mI->_render->endScene();
	}
 
	// ----- Indielib End -----
	mI->end();
 
	//FMOD
	/* On libère le son et on ferme et libère l'objet système */
    FMOD_Sound_Release(test);
    FMOD_System_Close(system);
    FMOD_System_Release(system);

	return 0;
}
