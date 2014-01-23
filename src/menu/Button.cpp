#include "Button.h"

namespace Symp {

Button::Button(std::string name, const char* filePath)
	: GuiComponent(name) {
	setSurface(filePath);
	enable();
}

Button::Button(std::string name, Symp::Color color, float iPosX, float iPosY, float iWidth, float iHeight)
	: GuiComponent(name) {

	color = color;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_pEntity2d->setPrimitive2d(IND_FILL_RECTANGLE);
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setRectangle(iPosX, iPosY, iPosX+iWidth, iPosY+iHeight);
	fill(color);

	enable();
}

void Button::enable(){
	if (!m_bIsEnabled){
		//Redraw the entity
		//TODO
		m_bIsEnabled = true;
	}
}

void Button::disable(){
	if (m_bIsEnabled){
		//Redraw the entity
		//TODO
		m_bIsEnabled = false;
	}
}

void Button::fill(Symp::Color color){
	m_pEntity2d->setTint(color.r, color.g, color.b);
	m_pEntity2d->setTransparency(color.a);
}

void Button::setText(std::string text){
	m_pEntity2d->setText(text.c_str());
}

}