#include "GuiComponent.h"

#include <IND_Image.h>
#include <IND_ImageManager.h>

namespace Symp{

IND_ImageManager* 	GuiComponent::s_pImageManager = new IND_ImageManager();
IND_SurfaceManager* GuiComponent::s_pSurfaceManager = new IND_SurfaceManager();
IND_FontManager* GuiComponent::s_pFontManager = new IND_FontManager();

const Color Color::BLUE = Color(0, 0, 255);
const Color Color::RED = Color(255, 0, 0);
const Color Color::GREEN = Color(0, 255, 0);
const Color Color::GREY = Color(180, 180, 180);
const Color Color::BLACK = Color(255, 255, 255);

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
	s_pFontManager->init(s_pImageManager, s_pSurfaceManager);

	//Load the standard font
	//loadFont("../assets/");
}

void GuiComponent::end(){
	s_pSurfaceManager->end();
	s_pImageManager->end();
	s_pFontManager->end();
	DISPOSE(s_pSurfaceManager);
    DISPOSE(s_pImageManager);
    DISPOSE(s_pFontManager);
}

int GuiComponent::loadFont(const char* filePath){
	IND_Font* pfont = IND_Font::newFont();
	return s_pFontManager->addAngelcodeFont(pfont, filePath, IND_ALPHA, IND_32);
}

float GuiComponent::getWidth(){
	if (m_pEntity2d->getSurface() != nullptr){
		return m_pEntity2d->getSurface()->getWidth();
	}
	else{
		return m_iWidth;
	}
}

float GuiComponent::getHeight(){
	if (m_pEntity2d->getSurface() != nullptr){
		return m_pEntity2d->getSurface()->getHeight();
	}
	else{
		return m_iHeight;
	}
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