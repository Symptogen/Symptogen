#include "Slider.h"
#include <cmath>

namespace Symp {

Slider::Slider( float fPercentage, float iPosX, float iPosY, int iWidth, int iHeight)
	: GuiComponent() {

	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_fPercentage = fPercentage;
	m_pEntity2d->setPrimitive2d(IND_FILL_RECTANGLE);
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setRectangle((int)iPosX, (int)iPosY, (int)iPosX+iWidth, (int)iPosY+iHeight);
	
	fill(Symp::Color::WHITE);
	m_bIsEnabled = true;

	m_pImage = new Image("../assets/slider_test.png", iPosX, iPosY);
	m_pImage->setAspectRatio(AspectRatio::IGNORE_ASPECT_RATIO);
	m_pImage->getIND_Entity2d()->setPosition(getPosX(), getPosY(), 0);
	m_pImage->setWidth(m_iWidth);
	m_pImage->setHeight(m_iHeight);
	
}

void Slider::update() {
	m_pEntity2d->setPosition(getPosX(), getPosY(), 0);
	m_pEntity2d->setRectangle((int)getPosX(), (int)getPosY(), (int)getPosX() + getWidth(), (int)getPosY() + getHeight());

	if (m_pImage->getIND_Entity2d()->getSurface()->getWidth() != getWidth() || m_pImage->getIND_Entity2d()->getSurface()->getHeight() != getHeight()){
		m_pImage->getIND_Entity2d()->setPosition((int)getPosX(), (int)getPosY(), 0);
		m_pImage->getIND_Entity2d()->setRegion(0, 0, (m_fPercentage)* m_pImage->getIND_Entity2d()->getSurface()->getWidth(), m_pImage->getIND_Entity2d()->getSurface()->getHeight());
	}
	m_pImage->update();
}

void Slider::fill(Symp::Color color){
	m_pEntity2d->setTint(color.r, color.g, color.b);
	m_pEntity2d->setTransparency(color.a);
}

}