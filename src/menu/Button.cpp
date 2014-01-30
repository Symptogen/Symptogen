#include "Button.h"

namespace Symp {

Button::Button(const char* filePath)
	: GuiComponent(), m_color(0, 0, 0) {
	// TODO
	setSurface(filePath);
	enable();
}

Button::Button(std::string text, Symp::Color color, int iWeight)
	: GuiComponent(), m_color(color.r, color.g, color.b) {
	if (iWeight == 0) {
		m_pEntity2d->setPrimitive2d(IND_FILL_RECTANGLE);
	} else {
		m_pEntity2d->setPrimitive2d(IND_RECTANGLE);
	}
	enable();

}

Button::Button(Symp::Color color, int iPosX, int iPosY, int iWidth, int iHeight)
	: GuiComponent(), m_color(color.r, color.g, color.b) {

	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_pEntity2d->setPrimitive2d(IND_FILL_RECTANGLE);
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setRectangle(iPosX, iPosY, iPosX+iWidth, iPosY+iHeight);
	fill(m_color);

	enable();
}

void Button::update(){
	if(m_pEntity2d->getSurface() == nullptr){
		m_pEntity2d->setPosition(getPosX(), getPosY(), 0);
		m_pEntity2d->setRectangle(getPosX(), getPosY(), getPosX() + getWidth(), getPosY() + getHeight());
		if(m_bIsHovered){
			m_pEntity2d->setTint(100,100,100);
		}else {
			fill(m_color);
		}
	}
	else{
		m_pEntity2d->setPosition(getPosX(), getPosY(), 0);
		if(m_bIsHovered){
			m_pEntity2d->setTint(100,100,100);
		}
		else{
			m_pEntity2d->setTint(255, 255, 255);
		}
	}
}

void Button::fill(Symp::Color color){
	m_pEntity2d->setTint(color.r, color.g, color.b);
	m_pEntity2d->setTransparency(color.a);
}

void Button::setText(std::string text){
	//Load the standard font
	m_sText = text;
	if(!loadFont("../assets/fonts/arial.fnt"))
		std::cerr<< "Error while loading fonts." <<std::endl;
	m_pEntity2d->setText(text.c_str());
}

}