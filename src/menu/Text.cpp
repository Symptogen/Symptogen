#include "Text.h"

/** @namespace Symp */
namespace Symp {

extern int g_WindowHeight;
extern int g_WindowWidth;

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
Text::Text(std::string text, Symp::Color color, float iPosX, float iPosY, bool b_smallFont)
	: GuiComponent(), m_color(color.r, color.g, color.b) {
	
	loadFont();
	m_sText = text;
	m_color = color;
	if (b_smallFont){
		m_pFont = m_pFontSmall; 
	}else{
		m_pFont = m_pFontBig; 
	}
	m_pEntity2d->setFont(m_pFont);
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
	m_pEntity2d->setPosition(getPosX(), getPosY(), 0);
	m_pEntity2d->setRectangle(getPosX(), getPosY(), getPosX()+getWidth(), getPosY()+getHeight());

	//Handle events
	if(m_bIsHovered && m_bIsEnabled){
		fill(Symp::Color::RED);
	}else if (!m_bIsHovered && m_bIsEnabled){
		
		fill(m_color);
	}else{
		m_pEntity2d->setTint(50, 50, 50);	
	}
}

/**
* @brief Center the Text in the x axis
* Refresh the display of the #Text.
* @param yPos the y position of the text
* @see Text
* @see ~Text()
* @see GuiComponent
*/
void Text::centerX(int yPos){
	m_pEntity2d->setPosition(g_WindowWidth/2, yPos, 0);
	update();
}

/**
* @brief Center the Text in the y axis
* Refresh the display of the #Text.
* @param xPos the x position of the text
* @see Text
* @see ~Text()
* @see GuiComponent
*/
void Text::centerY(int xPos){
	m_pEntity2d->setPosition(xPos, g_WindowHeight/2, 0);
	update();
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