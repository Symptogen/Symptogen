// This file was created the 3-12-2013 by Cecilia Lejeune
// in order to test the portability of Indielib game engine.
// Supposed to display the Dino picture on a background

#include "CIndieLib.h"


#include <IND_Surface.h>
#include <IND_Entity2d.h>
#include <IND_Image.h>
#include <IND_Animation.h>
 
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
 	
 	//Load cave picture
	IND_Image *mImageCave = IND_Image::newImage();
	if (!mI->_imageManager->add(mImageCave, "../assets/cave.png")) return 0;
 	//Load rabbit picture
	//IND_Image *mImageRabbit = IND_Image::newImage();
	//if (!mI->_imageManager->add(mImageRabbit, "../assets/rabbit.png")) return 0;

	// Loading Background
	IND_Surface *mSurfaceBack = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceBack, mImageCave, IND_OPAQUE, IND_32)) return 0;
	// Loading Rabbit
	//IND_Surface *mSurfaceRabbit = IND_Surface::newSurface();
	//if (!mI->_surfaceManager->add(mSurfaceRabbit, mImageRabbit, IND_ALPHA, IND_32)) return 0;

	//Delete the images
	mI->_imageManager->remove(mImageCave);
	//mI->_imageManager->remove(mImageRabbit);

	//Rabbit animations
	IND_Animation *mAnimationRabbit = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mAnimationRabbit, "../assets/rabbit_animation.xml", IND_ALPHA, IND_32)) return 0;
	
	// Creating 2d entity for the background
	IND_Entity2d *mBack = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mBack);
	mBack->setSurface(mSurfaceBack);

	// Creating 2d entity for the Rabbit1
	IND_Entity2d *mRabbit = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mRabbit);
	//mRabbit->setSurface(mSurfaceRabbit);
	mRabbit->setAnimation(mAnimationRabbit);

	// Creating 2d entity for the Rabbit2
	IND_Entity2d *mRabbit2 = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mRabbit2);
	//mRabbit2->setSurface(mSurfaceRabbit);
	mRabbit2->setAnimation(mAnimationRabbit);

	// ----- Changing the attributes of the 2d entities -----
	// ----- Background -----
	mBack->setHotSpot(0.5f, 0.5f);
	mBack->setPosition(400, 300, 0);
	mBack->setScale(1.7f, 1.7f);

	// ----- Rabbit1 -----
	mRabbit->setHotSpot(0.5f, 0.5f);
	mRabbit->setPosition(400, 200, 0);
	mRabbit->setSequence(0); //sequence "rabbit_flash_normal" in rabbit_anmaition.xml
	//mRabbit->setMirrorX(1); //fucked the collisions !

	// ----- Rabbit2 -----
	mRabbit2->setHotSpot(0.5f, 0.5f);
	mRabbit2->setPosition(400, 100, 0);
	mRabbit2->setSequence(1); //sequence "rabbit_flash_fast" in rabbit_anmaition.xml
	

	int stepMov = 5;
	// ----- Main Loop -----
	while (!mI->_input->onKeyPress (IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input Update ----
		mI->_input->update();
 
		// -------- Render -------
		/***** Déplacements Rabbit1 *****/
		if (mI->_input->isKeyPressed(IND_KEYRIGHT)){
			if (!mI->_entity2dManager->isCollision(mRabbit, "rabbit_right", mRabbit2, "*")){
				mRabbit->setPosition(mRabbit->getPosX() + stepMov, mRabbit->getPosY(), 0);
			}
		}
		if (mI->_input->isKeyPressed(IND_KEYLEFT)){
			if (!mI->_entity2dManager->isCollision(mRabbit, "rabbit_left", mRabbit2, "*")){
				mRabbit->setPosition(mRabbit->getPosX() - stepMov, mRabbit->getPosY(), 0);
			}

		}
		if (mI->_input->isKeyPressed(IND_KEYUP)){
			if (!mI->_entity2dManager->isCollision(mRabbit, "rabbit_up", mRabbit2, "*")){
				mRabbit->setPosition(mRabbit->getPosX(), mRabbit->getPosY() - stepMov, 0);
			}
		}
		if (mI->_input->isKeyPressed(IND_KEYDOWN)){
			if (!mI->_entity2dManager->isCollision(mRabbit, "rabbit_down", mRabbit2, "*")){
				mRabbit->setPosition(mRabbit->getPosX(), mRabbit->getPosY() + stepMov, 0);
			}
		}

		/***** Déplacements Rabbit2 *****/
		if (mI->_input->isKeyPressed(IND_D)){
			if (!mI->_entity2dManager->isCollision(mRabbit2, "rabbit_right2", mRabbit, "*")){
				mRabbit2->setPosition(mRabbit2->getPosX() + stepMov, mRabbit2->getPosY(), 0);
			}
		}
		if (mI->_input->isKeyPressed(IND_Q)){
			if (!mI->_entity2dManager->isCollision(mRabbit2, "rabbit_left2", mRabbit, "*")){
				mRabbit2->setPosition(mRabbit2->getPosX() - stepMov, mRabbit2->getPosY(), 0);
			}
		}
		if (mI->_input->isKeyPressed(IND_Z)){
			if (!mI->_entity2dManager->isCollision(mRabbit2, "rabbit_up2", mRabbit, "*")){
				mRabbit2->setPosition(mRabbit2->getPosX(), mRabbit2->getPosY() - stepMov, 0);
			}
		}
		if (mI->_input->isKeyPressed(IND_S)){
			if (!mI->_entity2dManager->isCollision(mRabbit2, "rabbit_down2", mRabbit, "*")){
				mRabbit2->setPosition(mRabbit2->getPosX(), mRabbit2->getPosY() + stepMov, 0);
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