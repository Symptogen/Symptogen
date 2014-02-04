#include "Slider.h"

namespace Symp {

Slider::Slider( int iPercentage, float iPosX, float iPosY, int iWidth, int iHeight)
	: GuiComponent() {

	m_iPercentage = iPercentage;
	m_pEntity2d->setPrimitive2d(IND_FILL_RECTANGLE);
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setRectangle((int)iPosX, (int)iPosY, (int)iPosX+iWidth, (int)iPosY+iHeight);
	m_pEntity2d->setRegion((int)getPosX(), (int)getPosY(), (int)getPosX() + (int)(m_iPercentage/100)*getWidth(), (int)getPosY() + (int)(m_iPercentage/100)*getHeight());
	
	fill(Symp::Color::BLACK);
	m_bIsEnabled = true;
}

void Slider::update() {
	m_pEntity2d->setPosition(getPosX(), getPosY(), 0);
	m_pEntity2d->setRectangle((int)getPosX(), (int)getPosY(), (int)getPosX() + getWidth(), (int)getPosY() + getHeight());
	m_pEntity2d->setRegion((int)getPosX(), (int)getPosY(), (int)getPosX() + (int)(m_iPercentage/100)*getWidth(), (int)getPosY() + (int)(m_iPercentage/100)*getHeight());
}

void Slider::fill(Symp::Color color){
	m_pEntity2d->setTint(color.r, color.g, color.b);
	m_pEntity2d->setTransparency(color.a);
}

}