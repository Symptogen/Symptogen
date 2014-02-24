#include "GuiComponent.h"

#include <IND_Image.h>
#include <IND_ImageManager.h>

/** @namespace Symp */
namespace Symp{

IND_ImageManager* 	GuiComponent::s_pImageManager = nullptr;
IND_SurfaceManager* GuiComponent::s_pSurfaceManager = nullptr;
IND_FontManager* GuiComponent::s_pFontManager = nullptr;

const Color Color::BLUE = Color(0, 0, 255);
const Color Color::RED = Color(255, 0, 0);
const Color Color::GREEN = Color(0, 255, 0);
const Color Color::GREY = Color(180, 180, 180);
const Color Color::BLACK = Color(0, 0, 0);
const Color Color::WHITE = Color(255, 255, 255);

/**
* @brief GuiComponent constructor. 
* Responsible for the initialization of the private attributes #m_iWidth and #m_iHeight, and for the creation of an 
* IND_Entity2d. The static function #init() is called only on the creation of the MenuManager and allow the management
* of a IND_Entity2d by several Indielib managers.
* @see init()
* @see ~GuiComponent()
*/
GuiComponent::GuiComponent()
	: m_iWidth(0), m_iHeight(0), m_bIsHovered(false) {
	m_pEntity2d = IND_Entity2d::newEntity2d();
}

/**
* @brief GuiComponent destructor. 
* The only attribute to destroy is the Entity2d.
* @see GuiComponent()
*/
GuiComponent::~GuiComponent(){
	m_pEntity2d->destroy();
}

/**
* @brief GuiComponent static initializer 
* This static function is called by the #MenuManager at its creation. It started all the Indielib services 
* that need IND_Entity2d to be managed and rendered.
* @param pRender the reference to the #Render class
* @see MenuManager
* @see end()
* @see GuiComponent()
* @see ~GuiComponent()
*/
void GuiComponent::init(Render* pRender){
	s_pImageManager = new IND_ImageManager();
	s_pSurfaceManager = new IND_SurfaceManager();
	s_pFontManager = new IND_FontManager();
	s_pImageManager->init();
	s_pSurfaceManager->init(s_pImageManager, pRender->getIND_Render());
	s_pFontManager->init(s_pImageManager, s_pSurfaceManager);
}

/**
* @brief GuiComponent static destroyer
* This static function is called by the #MenuManager at its destruction. It kill all the Indielib services.
* @see init()
* @see MenuManager
* @see GuiComponent()
* @see ~GuiComponent()
*/
void GuiComponent::end(){
	s_pSurfaceManager->end();
	s_pImageManager->end();
	s_pFontManager->end();
	DISPOSE(s_pSurfaceManager);
    DISPOSE(s_pImageManager);
    DISPOSE(s_pFontManager);
}

/**
* @brief Enable a component
* When a #GuiComponent is enabled, it can receive any kind of event, and following its type, it can change its
* shape or color, in a ergonomic way. To make a #GuiComponent disabled, call the #disable() function.
* @see m_bIsEnabled
* @see update()
* @see disable()
* @see GuiComponent()
*/
void GuiComponent::enable(){
	if (!m_bIsEnabled){
		m_bIsEnabled = true;
		update();
	}
}

/**
* @brief Disable a component
* When a #GuiComponent is disabled, it cant receive any kind of event, and following its type, it can change its
* shape or color, in a ergonomic way. To make a #GuiComponent enabled, call the #enable() function.
* @see m_bIsEnabled
* @see update()
* @see enable()
* @see GuiComponent()
*/
void GuiComponent::disable(){
	if (m_bIsEnabled){
		m_bIsEnabled = false;
		update();
	}
}

/**
* @brief Load a font file into the application
* @bug /!\ NOT READY YET /!\ This function need the newest version of Indielib to load AngelCode font. 
* @param filePath the path to the font file
* @see GuiComponent()
*/
bool GuiComponent::loadFont(){
	bool errorValue;
	//IND_Font* pFontSmall = IND_Font::newFont();
	//Reference to an old version of Indielib
	//if (!s_pFontManager->add(&mFontSmall, "..\\assets\\fonts\\font_small.png", "..\\assets\\fonts\\\\font_small.xml", IND_ALPHA, IND_32)){
		//Reference to a newer version of Indielib
		//if(!s_pFontManager->addMudFont(pFontSmall,"../assets/fonts/font_small.png", "../assets/fonts/font_small.xml", IND_ALPHA, IND_32))
		//	errorValue = 1;
		//else
		//	errorValue = 0;
	//}else {
	//	errorValue = 1;
	//}
	//return errorValue;
	return 0;
}

/**
* @brief Check if the component is under the mouse
* This function is used in both clic event and mouse over event. Only a #GuiComponent is responsible for checking if the 
* mouse is colliding its shape.
* @param mouseX the x coordinate of the mouse position in pixels
* @param mouseY the y coordinate of the mouse position in pixels
* @see GuiComponent()
* @see MenuManager::handleMouseClic()
* @see MenuManager::handleMouseHover()
*/
bool GuiComponent::isTargetedByMouse(int mouseX, int mouseY){
	bool value = false;
	if ( (mouseX > getPosX() && mouseX < getPosX() + getWidth()) &&
			(mouseY > getPosY() && mouseY < getPosY() + getHeight()) ){
			value = true;
	}
	return value;

}

/**
* @brief Apply a texture from an image file to a #GuiComponent
* For displaying a texture on an IND_Entity2d, Indielib library needs to have a IND_Surface object, and itself needs
* a IND_Image object. We initialize these objects by using static function of the Indielib managers initialized by
* #init().
* @param filePath the path to the image file
* @see GuiComponent()
* @see init()
* @see end()
*/
void GuiComponent::setSurface(const char* filePath) {
	IND_Surface* pSurface = IND_Surface::newSurface();
	IND_Image* pImage = IND_Image::newImage();

	// First security check
	if(filePath != NULL) {
		//If the file does not exists, then throw an error
		int result = s_pImageManager->add(pImage, filePath);
		if(result != 1) {
			std::cerr << "Error when creating the Indielib image " << filePath << ". The program will close." << std::endl;
			exit(EXIT_FAILURE);
		}
		//Indielib functions
		s_pSurfaceManager->add(pSurface, pImage, IND_ALPHA, IND_32);
		s_pImageManager->remove(pImage);
		m_pEntity2d->setSurface(pSurface);
	}
}

}