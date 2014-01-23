#include "GuiComponent.h"

#include <IND_Image.h>
#include <IND_ImageManager.h>

namespace Symp{

IND_ImageManager* 	GuiComponent::s_pImageManager = new IND_ImageManager();
IND_SurfaceManager* GuiComponent::s_pSurfaceManager = new IND_SurfaceManager();

GuiComponent::GuiComponent(std::string name){
	m_sName = name;
	m_pEntity2d = IND_Entity2d::newEntity2d();
}

GuiComponent::~GuiComponent(){
	m_pEntity2d->destroy();
}

void GuiComponent::init(Render* pRender){
	s_pImageManager->init();
	s_pSurfaceManager->init(s_pImageManager, pRender->getIND_Render());
}

void GuiComponent::end(){
	s_pSurfaceManager->end();
	s_pImageManager->end();
	DISPOSE(s_pSurfaceManager);
    DISPOSE(s_pImageManager);
}

bool GuiComponent::isTargetedByMouse(int mouseX, int mouseY){
	bool value = false;
	std::cout << "mouse x = " << mouseX << " and mouse y = " << mouseY <<std::endl;
	std::cout << "pos x = " << getPosX() << " and pos y = " << getPosY() <<std::endl;
	std::cout << "width = " << getWidth() << " and height = " << getHeight() << std::endl;
	if ( (mouseX > getPosX() && mouseX < getPosX() + getWidth()) &&
			(mouseY > getPosY() && mouseY < getPosY() + getHeight()) ){
			value = true;
	}
	return value;

}

void GuiComponent::setSurface(const char* filePath) {
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

}