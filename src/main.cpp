// This file was created the 3-12-2013 by Cecilia Lejeune
// in order to test the portability of Indielib game engine.
// Supposed to display the Dino picture on a background

#include "CIndieLib.h"


#include <IND_Surface.h>
#include <IND_Entity2d.h>
#include <IND_Image.h>
 
/*================== Main ==================*/

#ifdef _WIN32
int IndieLib()
#elif __linux__
Indielib_Main
#endif
{
	// ----- IndieLib intialization -----
 
	CIndieLib *mI = CIndieLib::instance();
	if (!mI->init()) return 0;	
 	
 	//Load a pictures
	IND_Image *mImageCave = IND_Image::newImage();
	if (!mI->_imageManager->add(mImageCave, "../assets/cave.png")) return 0;

	IND_Image *mImageRabbit = IND_Image::newImage();
	if (!mI->_imageManager->add(mImageRabbit, "../assets/rabbit.png")) return 0;


	// Loading Background
	IND_Surface *mSurfaceBack = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceBack, mImageCave, IND_OPAQUE, IND_32)) return 0;

	// Loading Rabbit
	IND_Surface *mSurfaceRabbit = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceRabbit, mImageRabbit, IND_ALPHA, IND_32)) return 0;

	//Delete the images
	mI->_imageManager->remove(mImageCave);
	mI->_imageManager->remove(mImageRabbit);

	// Creating 2d entity for the background

	IND_Entity2d *mBack = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mBack);					// Entity adding
	mBack->setSurface(mSurfaceBack);					// Set the surface into the entity

	// Creating 2d entity for the Rabbit
	IND_Entity2d *mRabbit = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mRabbit);					// Entity adding
	mRabbit->setSurface(mSurfaceRabbit);

	// Creating 2d entity for the Rabbit
	IND_Entity2d *mRabbit2 = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mRabbit2);					// Entity adding
	mRabbit2->setSurface(mSurfaceRabbit);

	// ----- Changing the attributes of the 2d entities -----

	int step = 5;
	// Background
	mBack->setHotSpot(0.5f, 0.5f);
	mBack->setPosition(400, 300, 0);
	mBack->setScale(1.7f, 1.7f);

	// Rabbit
	mRabbit->setHotSpot(0.5f, 0.5f);
	mRabbit->setPosition(400, 200, 0);
	mRabbit->setBoundingTriangle("rabbit", 10, 5, 56, 5, 56, 62);
	mRabbit->setBoundingTriangle("rabbit", 10, 62, 10, 5, 56, 62);

	mRabbit->setBoundingTriangle("rabbit_left", 10 - step, 5, 56, 5, 56, 62);
	mRabbit->setBoundingTriangle("rabbit_left", 10 - step, 62, 10 - step, 5, 56, 62);
	
	mRabbit->setBoundingTriangle("rabbit_right", 10, 5, 56 + step, 5, 56 + step, 62);
	mRabbit->setBoundingTriangle("rabbit_right", 10, 62, 10, 5, 56 + step, 62);

	mRabbit->setBoundingTriangle("rabbit_up", 10, 5 - step, 56, 5 - step, 56, 62);
	mRabbit->setBoundingTriangle("rabbit_up", 10, 62, 10, 5 - step, 56, 62);

	mRabbit->setBoundingTriangle("rabbit_down", 10, 5, 56, 5, 56, 62 + step);
	mRabbit->setBoundingTriangle("rabbit_down", 10, 62 + step, 10, 5, 56, 62 + step);


	// Rabbit
	mRabbit2->setHotSpot(0.5f, 0.5f);
	mRabbit2->setPosition(400, 100, 0);
	//mRabbit2->setMirrorX(1);
	mRabbit2->setBoundingTriangle("rabbit2", 10, 5, 56, 5, 56, 62);
	mRabbit2->setBoundingTriangle("rabbit2", 10, 62, 10, 5, 56, 62);

	mRabbit2->setBoundingTriangle("rabbit_left2", 10 - step, 5, 56, 5, 56, 62);
	mRabbit2->setBoundingTriangle("rabbit_left2", 10 - step, 62, 10 - step, 5, 56, 62);
	
	mRabbit2->setBoundingTriangle("rabbit_right2", 10, 5, 56 + step, 5, 56 + step, 62);
	mRabbit2->setBoundingTriangle("rabbit_right2", 10, 62, 10, 5, 56 + step, 62);

	mRabbit2->setBoundingTriangle("rabbit_up2", 10, 5 - step, 56, 5 - step, 56, 62);
	mRabbit2->setBoundingTriangle("rabbit_up2", 10, 62, 10, 5 - step, 56, 62);

	mRabbit2->setBoundingTriangle("rabbit_down2", 10, 5, 56, 5, 56, 62 + step);
	mRabbit2->setBoundingTriangle("rabbit_down2", 10, 62 + step, 10, 5, 56, 62 + step);
	

	// ----- Main Loop -----
 
	while (!mI->_input->onKeyPress (IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input Update ----
 
		mI->_input->update();
 
		// -------- Render -------

		/***** Déplacement Rabbit *****/
		if (mI->_input->isKeyPressed(IND_KEYRIGHT)){

			// S'il n'y a pas collision
			if (!mI->_entity2dManager->isCollision(mRabbit, "rabbit_right", mRabbit2, "*")){
				mRabbit->setPosition(mRabbit->getPosX() + step, mRabbit->getPosY(), 0);
			}
		}
		if (mI->_input->isKeyPressed(IND_KEYLEFT)){

			// S'il n'y a pas collision
			if (!mI->_entity2dManager->isCollision(mRabbit, "rabbit_left", mRabbit2, "*")){
				mRabbit->setPosition(mRabbit->getPosX() - step, mRabbit->getPosY(), 0);
			}

		}
		if (mI->_input->isKeyPressed(IND_KEYUP)){
			
			if (!mI->_entity2dManager->isCollision(mRabbit, "rabbit_up", mRabbit2, "*")){
				mRabbit->setPosition(mRabbit->getPosX(), mRabbit->getPosY() - step, 0);
			}
		}
		if (mI->_input->isKeyPressed(IND_KEYDOWN)){
			
			if (!mI->_entity2dManager->isCollision(mRabbit, "rabbit_down", mRabbit2, "*")){
				mRabbit->setPosition(mRabbit->getPosX(), mRabbit->getPosY() + step, 0);
			}
			
		}


		/***** Déplacement Rabbit2 *****/
		if (mI->_input->isKeyPressed(IND_D)){

			if (!mI->_entity2dManager->isCollision(mRabbit2, "rabbit_right2", mRabbit, "*")){
				mRabbit2->setPosition(mRabbit2->getPosX() + step, mRabbit2->getPosY(), 0);
			}
		}
		if (mI->_input->isKeyPressed(IND_Q)){

			if (!mI->_entity2dManager->isCollision(mRabbit2, "rabbit_left2", mRabbit, "*")){
				mRabbit2->setPosition(mRabbit2->getPosX() - step, mRabbit2->getPosY(), 0);
			}
		}
		if (mI->_input->isKeyPressed(IND_Z)){

			if (!mI->_entity2dManager->isCollision(mRabbit2, "rabbit_up2", mRabbit, "*")){
				mRabbit2->setPosition(mRabbit2->getPosX(), mRabbit2->getPosY() - step, 0);
			}
		}
		if (mI->_input->isKeyPressed(IND_S)){

			if (!mI->_entity2dManager->isCollision(mRabbit2, "rabbit_down2", mRabbit, "*")){
				mRabbit2->setPosition(mRabbit2->getPosX(), mRabbit2->getPosY() + step, 0);
			}
		}
 
		mI->_render->clearViewPort(60, 60, 60);
		mI->_render->beginScene();
		mI->_entity2dManager->renderEntities2d();
		//mI->_entity2dManager->renderCollisionAreas(255, 0, 0, 255); // To see the collisions areas
		mI->_render->endScene();
	}
 
	// ----- Indielib End -----
 
	mI->end();
 
	return 0;
}