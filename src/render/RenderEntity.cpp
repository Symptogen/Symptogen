#include "RenderEntity.h"

#include <IND_Image.h>
#include <IND_ImageManager.h>

namespace Symp{

IND_ImageManager* 		RenderEntity::s_pImageManager = 	nullptr;
IND_SurfaceManager* 	RenderEntity::s_pSurfaceManager = 	nullptr;
IND_AnimationManager* 	RenderEntity::s_pAnimationManager = nullptr;

RenderEntity::RenderEntity(const char* filePath, RenderType renderType) {
	m_pEntity2d = IND_Entity2d::newEntity2d();
	if(renderType == Surface)
		setSurface(filePath);
	if(renderType == Animation)
		setAnimation(filePath);
	m_pTimer = new IND_Timer();
	m_bIsAnimationPlaying = false;
}

RenderEntity::~RenderEntity() {
    m_pEntity2d->destroy();
}

void RenderEntity::init(Render* pRender) {
	s_pImageManager = new IND_ImageManager();
	s_pSurfaceManager = new IND_SurfaceManager();
	s_pAnimationManager = new IND_AnimationManager();

	s_pImageManager->init();
	s_pSurfaceManager->init(s_pImageManager, pRender->getIND_Render());
	s_pAnimationManager->init(s_pImageManager, s_pSurfaceManager);
}

void RenderEntity::end() {
	s_pAnimationManager->end();
	s_pSurfaceManager->end();
	s_pImageManager->end();
	DISPOSE(s_pAnimationManager);
	DISPOSE(s_pSurfaceManager);
    DISPOSE(s_pImageManager);
}

int RenderEntity::getWidth() const {
	if(m_pEntity2d->getSurface())
		return m_pEntity2d->getSurface()->getWidth() * m_pEntity2d->getScaleX();
	else if(m_pEntity2d->getAnimation())
		return m_pEntity2d->getAnimation()->getHighWidth(0) * m_pEntity2d->getScaleX();
	else
		return -1;
}

int RenderEntity::getHeight() const {
	if(m_pEntity2d->getSurface())
		return m_pEntity2d->getSurface()->getHeight() * m_pEntity2d->getScaleY();
	else if(m_pEntity2d->getAnimation())
		return m_pEntity2d->getAnimation()->getHighHeight(0) * m_pEntity2d->getScaleY();
	else
		return -1;
}

void RenderEntity::setSurface(const char* filePath) {
	IND_Surface* pSurface = IND_Surface::newSurface();
	IND_Image* pImage = IND_Image::newImage();
	if(filePath != NULL) {
		bool checkError = s_pImageManager->add(pImage, filePath); //throw error if the file doesn't exist
		if(!checkError) {
			std::cerr << "Error when creating the Indielib image " << filePath << ". The program will close." << std::endl;
			exit(EXIT_FAILURE);
		}
		s_pSurfaceManager->add(pSurface, pImage, IND_OPAQUE, IND_32);
		s_pImageManager->remove(pImage);
		m_pEntity2d->setSurface(pSurface);
	}
}

void RenderEntity::setAnimation(const char* filePath) {
	IND_Animation* pAnimation = IND_Animation::newAnimation();
	if(filePath != NULL){
		bool checkError = s_pAnimationManager->addToSurface(pAnimation, filePath, IND_ALPHA, IND_32); //throw error if the file doesn't exist
		if(!checkError) {
			std::cerr << "Error when creating the Indielib animation " << filePath << ". The program will close." << std::endl;
			exit(EXIT_FAILURE);
		}
		m_pEntity2d->setAnimation(pAnimation);
	}
}

void RenderEntity::playDeathAnimation(bool flag){
	if(!isAnimationPlaying()){
		m_bIsAnimationPlaying = flag;
		m_pTimer->start();
	}
	updateAnimationTimer();
}

void RenderEntity::updateAnimationTimer(){
	if(m_pTimer->getTicks()>=getAnimationLength()){
    	m_pTimer->stop();
   			m_bIsAnimationPlaying = false;
 	}
}

}