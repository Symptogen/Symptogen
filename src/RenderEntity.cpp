#include "RenderEntity.h"

#include <IND_Image.h>
#include <IND_ImageManager.h>

IND_ImageManager* 		RenderEntity::s_pImageManager = 	new IND_ImageManager();
IND_SurfaceManager* 	RenderEntity::s_pSurfaceManager = 	new IND_SurfaceManager();
IND_AnimationManager* 	RenderEntity::s_pAnimationManager = new IND_AnimationManager();
IND_Entity2dManager* 	RenderEntity::s_pEntity2dManager = 	new IND_Entity2dManager();

RenderEntity::RenderEntity(const char* filePath, RenderType renderType){
	m_pEntity2d = IND_Entity2d::newEntity2d();
	if(renderType == Surface)
		setSurface(filePath);
	if(renderType == Animation)
		setAnimation(filePath);
}

void RenderEntity::init(Render* pRender){
	s_pImageManager->init();
	s_pSurfaceManager->init(s_pImageManager, pRender->getIND_Render());
	s_pAnimationManager->init(s_pImageManager, s_pSurfaceManager);
	s_pEntity2dManager->init(pRender->getIND_Render());
}

void RenderEntity::end(){
	s_pAnimationManager->end();
	s_pSurfaceManager->end();
	s_pImageManager->end();
	s_pEntity2dManager->end();
    DISPOSE(s_pEntity2dManager);
	DISPOSE(s_pAnimationManager);
	DISPOSE(s_pSurfaceManager);
    DISPOSE(s_pImageManager);
}

void RenderEntity::setSurface(const char* filePath){
	IND_Surface* pSurface = IND_Surface::newSurface();
	IND_Image* pImage = IND_Image::newImage();
	if(filePath != NULL){
		s_pImageManager->add(pImage, filePath); //throw error if the file doesn't exist
		s_pSurfaceManager->add(pSurface, pImage, IND_OPAQUE, IND_32);
		s_pImageManager->remove(pImage);
		m_pEntity2d->setSurface(pSurface);
		s_pEntity2dManager->add(m_pEntity2d);
	}
}

void RenderEntity::setAnimation(const char* filePath){
	IND_Animation* pAnimation = IND_Animation::newAnimation();
	if(filePath != NULL){
		s_pAnimationManager->addToSurface(pAnimation, filePath, IND_ALPHA, IND_32); //throw error if the file doesn't exist
		m_pEntity2d->setAnimation(pAnimation);
		s_pEntity2dManager->add(m_pEntity2d);
	}
}