#include "Text.h"

/** @namespace Symp */
namespace Symp {

/**
* @brief Text class constructor inherits the GuiComponent class
* Responsible for the initialization of the only class private attribute.
* @param text is the text that will be displayed by the #Text
* @param color is the color that will fill the #Text following the #iWeight attributes
* @see Text
* @see ~Text()
* @see Color
* @see Layout
* @see GuiComponent
*/
Text::Text(std::string text, Symp::Color color, float iPosX, float iPosY)
	: GuiComponent(), m_color(color.r, color.g, color.b) {
	
	loadFont();
	m_sText = text;
	m_pEntity2d->setFont(m_pFontBig);
	m_pEntity2d->setText(m_sText.c_str());
	m_pEntity2d->setLineSpacing	(18);
	m_pEntity2d->setCharSpacing	(-8);
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setAlign(IND_CENTER);

	enable();

}

/**
* @brief Text update fonction
* Refresh the display of the #Text.
* @see Text
* @see ~Text()
* @see GuiComponent
*/
void Text::update(){
	// Update size and position
	m_pEntity2d->setPosition(getPosX(), getPosY(), 0);
	m_pEntity2d->setRectangle((int)getPosX(), (int)getPosY(), (int)getPosX() + getWidth(), (int)getPosY() + getHeight());
}

/**
* @brief fill Text's background function
* @see Text
* @see ~Text()
* @see Color
* @see GuiComponent
*/
void Text::fill(Symp::Color color){
	m_pEntity2d->setTint(color.r, color.g, color.b);
	m_pEntity2d->setTransparency(color.a);
}

}