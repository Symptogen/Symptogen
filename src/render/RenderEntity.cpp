#include "RenderEntity.h"

#include <IND_Image.h>
#include <IND_ImageManager.h>
#include <string>

namespace Symp{

IND_ImageManager* 		RenderEntity::s_pImageManager = 	nullptr;
IND_SurfaceManager* 	RenderEntity::s_pSurfaceManager = 	nullptr;
IND_AnimationManager* 	RenderEntity::s_pAnimationManager = nullptr;

std::map<std::string, IND_Surface*> 	RenderEntity::s_surfaceMap = std::map<std::string, IND_Surface*>();
std::map<std::string, IND_Animation*> 	RenderEntity::s_animationMap = std::map<std::string, IND_Animation*>();

RenderEntity::RenderEntity(const char* filePath, RenderType renderType) {
	m_pEntity2d = IND_Entity2d::newEntity2d();
	if(renderType == Surface)
		setSurface(filePath);
	if(renderType == Animation)
		setAnimation(filePath);
	m_pTimer = new IND_Timer();
	m_bIsAnimationPlaying = false;
	m_bIsAnimationFinish = false;
}

RenderEntity::RenderEntity() {
	m_pEntity2d = IND_Entity2d::newEntity2d();
	m_pTimer = new IND_Timer();
	m_bIsAnimationPlaying = false;
	m_bIsAnimationFinish = false;
}

RenderEntity::~RenderEntity() {
    m_pEntity2d->destroy();
    delete m_pTimer;
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
    delete s_pAnimationManager;
	delete s_pSurfaceManager;
	delete s_pImageManager;
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

	if(filePath != nullptr) {

		std::string sFilePath(filePath);

		// Check if the element doesn't exist in the map of surfaces
		if(s_surfaceMap.count(sFilePath) == 0) {

			bool checkError = s_pImageManager->add(pImage, filePath); //throw error if the file doesn't exist
			if(!checkError) {
				std::cerr << "Error when creating the Indielib image " << filePath << " !" << std::endl;
				//exit(EXIT_FAILURE);
			}
			s_pSurfaceManager->add(pSurface, pImage, IND_OPAQUE, IND_32);
			s_pImageManager->remove(pImage);
			m_pEntity2d->setSurface(pSurface);
			s_surfaceMap.insert(std::make_pair(sFilePath, pSurface));
			s_surfaceMap.insert(std::make_pair(sFilePath, pSurface));
		}
		else{
			m_pEntity2d->setSurface((*(s_surfaceMap.find(sFilePath))).second);
		}

	}
}

void RenderEntity::setAnimation(const char* filePath) {
	IND_Animation* pAnimation = IND_Animation::newAnimation();
	if(filePath != NULL){
		std::string sFilePath(filePath);
		//if the element doesn't exist in the map of animations
		if(s_animationMap.count(sFilePath) == 0){
			bool checkError = s_pAnimationManager->addToSurface(pAnimation, filePath, IND_ALPHA, IND_32); //throw error if the file doesn't exist
			if(!checkError) {
				std::cerr << "Error when creating the Indielib animation " << filePath << " !" << std::endl;
				//exit(EXIT_FAILURE);
			}
			m_pEntity2d->setAnimation(pAnimation);
			s_animationMap.insert(std::pair<std::string, IND_Animation*>(sFilePath, pAnimation));
		}
		//if the element exists in the map of animations
		else{
			m_pEntity2d->setAnimation((*(s_animationMap.find(sFilePath))).second);
		}
	}
}

void RenderEntity::manageAnimationTimer(AnimationLength length){
	if(!isAnimationPlaying()){
		m_bIsAnimationPlaying = true;
		m_bIsAnimationFinish = false;
		m_pTimer->start();
	}
	else if(m_pTimer->getTicks() >= length){
    	m_pTimer->stop();
   		m_bIsAnimationPlaying = false;
   		m_bIsAnimationFinish = true;
 	}
}

}