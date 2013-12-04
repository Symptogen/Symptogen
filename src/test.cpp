// This file was created the 3-12-2013 by Cecilia Lejeune
// in order to test the portability of Indielib game engine.
// Supposed to display the Dino picture on a background

#include "CIndieLib.h"

#include "IND_Surface.h"
#include "IND_Entity2d.h"
#include "IND_Image.h"
 
/*================== Main ==================*/
Indielib_Main		
{
	// ----- IndieLib intialization -----
 
	CIndieLib *mI = CIndieLib::instance();
	if (!mI->init()) return 0;	
 	
 	//Load a picture
	IND_Image *mImageBugOriginal = IND_Image::newImage();
	if (!mI->_imageManager->add(mImageBugOriginal, "../assets/dino.png")) return 0;

	// Loading Background
	IND_Surface *mSurfaceBack = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceBack, "../assets/blue_background.jpg", IND_OPAQUE, IND_32)) return 0;

	// Creating the "Original Bug" surface from the IND_Image
	IND_Surface *mOriginalSurface = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mOriginalSurface, mImageBugOriginal, IND_ALPHA, IND_32)) return 0;

	//Delete the image
	mI->_imageManager->remove(mImageBugOriginal);

	// Creating 2d entity for the background
	IND_Entity2d *mBack = IND_Entity2d::newEntity2d();				
	mI->_entity2dManager->add(mBack);
	mBack->setSurface(mSurfaceBack);

	// Creating 2d entity for the "Original bug"
	IND_Entity2d *mOriginal = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mOriginal);
	mOriginal->setSurface(mOriginalSurface);
	mOriginal->setPosition(290,  90, 0);	

	// ----- Main Loop -----
 
	while (!mI->_input->onKeyPress (IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input Update ----
 
		mI->_input->update();
 
		// -------- Render -------
 
		mI->_render->clearViewPort(60, 60, 60);
		mI->_render->beginScene();
		mI->_entity2dManager->renderEntities2d();
		mI->_render->endScene();
	}
 
	// ----- Indielib End -----
 
	mI->end();
 
	return 0;
}