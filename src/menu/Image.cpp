#include "Image.h"

#include <cmath>

/** @namespace Symp */
namespace Symp {

/**
* @brief Image class constructor inherits the GuiComponent class
* Responsible for the initialization of the class private attributes. This constructor will associate a texture
* to its Indielib IND_Entity2d. The position and scale atributes have default values that can be left for using the 
* #Image with a #Layout.
* @param filePath is the path to the image file
* @param iPosX the x coordinate of the upper-left corner of the #Image in pixels (default = 0)
* @param iPosY the y coordinate of the upper-left corner of the #Image in pixels (default = 0)
* @param iScale the uniform scale that will be applied for both width and height between 0 and 1 (default no scale = 1)
* @see Image
* @see ~Image()
* @see GuiComponent
*/
Image::Image( const char* filePath, float iPosX, float iPosY, float iScale)
	: GuiComponent() {

	// As a texture is given in parameter, then create a surface to display
	setSurface(filePath);
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setScale(iScale, iScale);

	// Initialize the size paramters to the real size of the image
	m_iWidth = m_pEntity2d->getSurface()->getWidth() * iScale;
	m_iHeight = m_pEntity2d->getSurface()->getHeight() * iScale;
	
	// By default the Image is enabled and as the keep aspect ratio attribute
	m_bIsEnabled = true;
	m_ratio = AspectRatio::KEEP_ASPECT_RATIO;
}

/**
* @brief Show the Image
* The #show function doesn't affect attributes of the #Image class.
* @see Image
* @see hide()
* @see GuiComponent
*/
void Image::show(){
	m_pEntity2d->setShow(true);
}

/**
* @brief Hide the Image
* The #hide function doesn't affect attributes of the #Image class.
* @see Image
* @see show()
* @see GuiComponent
*/
void Image::hide(){
	m_pEntity2d->setShow(false);
}


/**
* @brief Image update fonction
* Refresh the display of the #Image.
* @see Image
* @see ~Image()
* @see GuiComponent
*/
void Image::update() {
	//Check if a texture is present or not
	if(m_pEntity2d->getSurface() != nullptr){
		//Update the position of the #Image
		m_pEntity2d->setPosition(getPosX(), getPosY(), 0);

		//Retrieve attributes from the texture itself
		float surfaceWidth = (float)m_pEntity2d->getSurface()->getWidth();
		float surfaceHeight = (float)m_pEntity2d->getSurface()->getHeight();
		float scaleX, scaleY, scale;

		// Compute the scale value
		if ( getWidth() != 0 && getHeight() != 0 ){
		
			if (getWidth() < surfaceWidth) {
				scaleX = getWidth() / surfaceWidth;
			}else {
				scaleX = surfaceWidth / getWidth() ;
			}
			if (getHeight() < surfaceHeight){
				scaleY = getHeight() / surfaceHeight;
			} else {
				scaleY = surfaceHeight / getHeight();
			} 

			// Following the aspect ratio, scale the #Image and update its size attributes
			switch( m_ratio) {
				case KEEP_ASPECT_RATIO:
					// Update only if there is a significant change to make
					if( abs(scaleX-scaleY) > 0.01){
						scale = fmin(scaleX, scaleY);
						m_pEntity2d->setScale(scale, scale);
						setWidth(scale * surfaceWidth);
						setHeight(scale * surfaceHeight);
					}
				break;
				case IGNORE_ASPECT_RATIO:
					m_pEntity2d->setScale(scaleX, scaleY);
					setWidth(scaleX * surfaceWidth);
					setHeight(scaleY * surfaceHeight);
				break;
				case EXPAND_ASPECT_RATIO:
					scale = fmax(scaleX, scaleY);
					m_pEntity2d->setScale(scale, scale);
					setWidth(scale * surfaceWidth);
					setHeight(scale * surfaceHeight);
				break;

			}
		}

	}
}

}