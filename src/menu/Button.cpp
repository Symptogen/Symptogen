#include "Button.h"

/** @namespace Symp */
namespace Symp {

/**
* @brief Button class constructor inherits the GuiComponent class
* Responsible for the initialization of the only class private attribute. This constructor will associate a texture
* to its Indielib IND_Entity2d. See other constructor for drawing #Button with text or color.
* @param filePath is the path to the image file
* @see Button
* @see ~Button()
* @see GuiComponent
*/
Button::Button(const char* filePath)
	: GuiComponent(), m_color(0, 0, 0) {

	// As a texture is given in parameter, then create a surface to display it on the Button
	setSurface(filePath);

	// Initialize the size paramters to the real size of the image
	m_iWidth = m_pEntity2d->getSurface()->getWidth();
	m_iHeight = m_pEntity2d->getSurface()->getHeight();

	m_pTextEntity = nullptr;

	// By default a Button is enabled
	enable();
}

/**
* @brief Button class constructor inherits the GuiComponent class
* Responsible for the initialization of the only class private attribute. This constructor will display a text 
* on the #Button and can draw a colored background or a colored border to the #Button. The #color parameter and
* the #iWeight parameter have default values that creates a default #Button with white background.
* @b Note : why aren't they any position/size parameters for the creation of the #Button ? Because, most of time, this
* constructor will be used with a #Layout. The #Layout will be responsible for the #Button size and position. If this
* constructor is not used with a #Layout, then the size and position attributes must be set manually following the
* #GuiComponent functions.
* @param text is the text that will be displayed by the #Button
* @param color is the color that will fill the #Button following the #iWeight attributes
* @param iWeight decide wether the #Button will have background (iWeight = 0) or border (iWieght != 0)
* @see Button
* @see ~Button()
* @see Color
* @see Layout
* @see GuiComponent
*/
Button::Button(std::string text, Symp::Color color, int iWeight)
	: GuiComponent(), m_color(color.r, color.g, color.b) {
	// The weight define wether the color will be displayed in a background or in a border
	if (iWeight == 0) {
		// Background 
		m_pEntity2d->setPrimitive2d(IND_FILL_RECTANGLE);
	} else {
		//Border
		m_pEntity2d->setPrimitive2d(IND_RECTANGLE);
	}
	setText(text);
	// By default a Button is enabled
	enable();

}

/**
* @brief Button class constructor inherits the GuiComponent class
* Responsible for the initialization of the only class private attribute. This constructor only draw one type 
* of #Button : a filled one. For drawing a border instead of a background, please see other #Button constructors.
* @b Note : this function was meant to be used in a standalone "way". If you use this constructor with a #Layout,
* then prefer other constructor, because there will redondancy of size and position informations otherwise.
* @param color is the color that will fill the #Button
* @param fPosX is the x position of the up-left corner of the #Button (why a float ? because Indielib needs it)
* @param fPosY is the y position of the up-left corner of the #Button (why a float ? because Indielib needs it)
* @see Button
* @see ~Button()
* @see Color
* @see GuiComponent
*/
Button::Button(Symp::Color color, float iPosX, float iPosY, int iWidth, int iHeight)
	: GuiComponent(), m_color(color.r, color.g, color.b) {

	m_iWidth = iWidth;
	m_iHeight = iHeight;

	// By default, this constructor draw a filled button
	m_pEntity2d->setPrimitive2d(IND_FILL_RECTANGLE);
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setRectangle((int)iPosX, (int)iPosY, (int)iPosX+iWidth, (int)iPosY+iHeight);
	fill(m_color);

	m_pTextEntity = nullptr;

	// By default a Button is enabled
	enable();
}

/**
* @brief Button update fonction
* Refresh the display of the #Button.
* @see Button
* @see ~Button()
* @see GuiComponent
*/
void Button::update(){
	//Check if the Button has a texture or not
	if(m_pEntity2d->getSurface() == nullptr){
		// Update size and position
		m_pEntity2d->setPosition(getPosX(), getPosY(), 0);
		m_pEntity2d->setRectangle((int)getPosX(), (int)getPosY(), (int)getPosX() + getWidth(), (int)getPosY() + getHeight());
		
		if(m_pTextEntity != nullptr){
			m_pTextEntity->setPosition(getPosX() + getWidth()/2, getPosY() + getHeight()/6, 0);
			m_pTextEntity->update();
		}	

		//Handle events
		if(m_bIsHovered && m_bIsEnabled){
			//Color the button on the mouse hover
			m_pEntity2d->setTint(100,100,100);
		}else if (!m_bIsHovered && m_bIsEnabled){
			//Restore the Button color
			fill(m_color);
		}else{
			// Color the Button is it is disabled
			m_pEntity2d->setTint(50,50,50);
		}
	}
	else{
		//Update the position
		m_pEntity2d->setPosition(getPosX(), getPosY(), 0);

		//Handle events
		if(m_bIsHovered){
			//Color the button on the mouse hover, on the texture
			m_pEntity2d->setTint(100,100,100);
		}
		else{
			//Restore the Button texture
			m_pEntity2d->setTint(255, 255, 255);
		}
	}
}

/**
* @brief fill Button's background function
* @see Button
* @see ~Button()
* @see Color
* @see GuiComponent
*/
void Button::fill(Symp::Color color){
	m_pEntity2d->setTint(color.r, color.g, color.b);
	m_pEntity2d->setTransparency(color.a);
}

/**
* @brief set the Button's text
* @bug /!\ NOT WORKING YET /!\
* @see Button
* @see ~Button()
* @see GuiComponent
*/
void Button::setText(std::string text){
	m_pTextEntity = new Text(text, Symp::Color::WHITE);
}

}