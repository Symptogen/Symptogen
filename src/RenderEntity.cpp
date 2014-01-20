#include "RenderEntity.h"

#include <IND_Image.h>
#include <IND_ImageManager.h>

namespace Symp{

IND_ImageManager* 		RenderEntity::s_pImageManager = 	new IND_ImageManager();
IND_SurfaceManager* 	RenderEntity::s_pSurfaceManager = 	new IND_SurfaceManager();
IND_AnimationManager* 	RenderEntity::s_pAnimationManager = new IND_AnimationManager();

RenderEntity::RenderEntity(const char* filePath, RenderType renderType){
	m_pEntity2d = IND_Entity2d::newEntity2d();
	if(renderType == Surface)
		setSurface(filePath);
	if(renderType == Animation)
		setAnimation(filePath);
}

RenderEntity::~RenderEntity(){
    m_pEntity2d->destroy();
}

void RenderEntity::init(Render* pRender){
	s_pImageManager->init();
	s_pSurfaceManager->init(s_pImageManager, pRender->getIND_Render());
	s_pAnimationManager->init(s_pImageManager, s_pSurfaceManager);
}

void RenderEntity::end(){
	s_pAnimationManager->end();
	s_pSurfaceManager->end();
	s_pImageManager->end();
	DISPOSE(s_pAnimationManager);
	DISPOSE(s_pSurfaceManager);
    DISPOSE(s_pImageManager);
}

void RenderEntity::setSurface(const char* filePath) {
	std::cerr << "Set surface for " << filePath << std::endl;
	IND_Surface* pSurface = IND_Surface::newSurface();
	IND_Image* pImage = IND_Image::newImage();
	if(filePath != NULL) {
		int result = s_pImageManager->add(pImage, filePath); //throw error if the file doesn't exist
		if(result != 1) {
			std::cerr << "Error when creating the Indielib image " << filePath << ". The program will close." << std::endl;
			exit(EXIT_FAILURE);
		}
		s_pSurfaceManager->add(pSurface, pImage, IND_OPAQUE, IND_32);
		s_pImageManager->remove(pImage);
		m_pEntity2d->setSurface(pSurface);
	}
}

void RenderEntity::setAnimation(const char* filePath){
	IND_Animation* pAnimation = IND_Animation::newAnimation();
	if(filePath != NULL){
		s_pAnimationManager->addToSurface(pAnimation, filePath, IND_ALPHA, IND_32); //throw error if the file doesn't exist
		m_pEntity2d->setAnimation(pAnimation);
	}
}

}