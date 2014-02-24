#include "Slider.h"
#include <cmath>

/** @namespace Symp */
namespace Symp {

/**
* @brief Slider constructor
* Responsible for the initialization of the private attributes of the #Slider class. 
* @param fPercentage the size of the foreground #Image
* @param iPosX the x coordinate of the upper-left corner of the #Slider in pixels
* @param iPosY the y coordinate of the upper-left corner of the #Slider in pixels
* @param iWidth the width of the #Slider in pixels
* @param iHeight the height of the #Slider in pixels
* @see Player
* @see ScrollArea
* @see MenuManager
* @see GuiComponent
* @see init()
* @see ~Slider()
*/
Slider::Slider( float fPercentage, float iPosX, float iPosY, int iWidth, int iHeight)
	: GuiComponent() {

	// Init the main attributes
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_fPercentage = fPercentage;
	m_pEntity2d->setPrimitive2d(IND_FILL_RECTANGLE);
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setRectangle((int)iPosX, (int)iPosY, (int)iPosX+iWidth, (int)iPosY+iHeight);
	fill(Symp::Color::WHITE);


	// Init the foreground texture
	m_pImage = new Image("../assets/menu/slider.png", iPosX, iPosY);
	m_pImage->setAspectRatio(AspectRatio::IGNORE_ASPECT_RATIO);
	m_pImage->getIND_Entity2d()->setPosition(getPosX(), getPosY(), 0);
	m_pImage->setWidth(m_iWidth);
	m_pImage->setHeight(m_iHeight);

	// By default a #Slider can receive events
	m_bIsEnabled = true;
	
}

/**
* @brief Slider update fonction
* Refresh the display of the #Slider.
* @see Slider
* @see ~Slider()
* @see GuiComponent
*/
void Slider::update() {
	//Update the background
	m_pEntity2d->setPosition(getPosX(), getPosY(), 0);
	m_pEntity2d->setRectangle((int)getPosX(), (int)getPosY(), (int)getPosX() + getWidth(), (int)getPosY() + getHeight());
 
	//Update the foreground 
	if (m_pImage->getIND_Entity2d()->getSurface()->getWidth() != getWidth() || m_pImage->getIND_Entity2d()->getSurface()->getHeight() != getHeight()){
		m_pImage->getIND_Entity2d()->setPosition((int)getPosX(), (int)getPosY(), 0);
		m_pImage->getIND_Entity2d()->setRegion(0, 0, (m_fPercentage)* m_pImage->getIND_Entity2d()->getSurface()->getWidth(), m_pImage->getIND_Entity2d()->getSurface()->getHeight());
	}
	m_pImage->update();
}

/**
* @brief fill Slider's background function
* @see Slider
* @see ~Slider()
* @see Color
* @see GuiComponent
*/
void Slider::fill(Symp::Color color){
	m_pEntity2d->setTint(color.r, color.g, color.b);
	m_pEntity2d->setTransparency(color.a);
}

}