#include "Image.h"

#include <cmath>

namespace Symp {

Image::Image( const char* filePath, float iPosX, float iPosY, float iScale)
	: GuiComponent() {

	setSurface(filePath);
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setScale(iScale, iScale);
	m_iWidth = m_pEntity2d->getSurface()->getWidth() * iScale;
	m_iHeight = m_pEntity2d->getSurface()->getHeight() * iScale;
	m_bIsEnabled = true;
	m_ratio = AspectRatio::KEEP_ASPECT_RATIO;
}

void Image::show(){
	m_pEntity2d->setShow(true);
}

void Image::hide(){
	m_pEntity2d->setShow(false);
}

void Image::update() {
	if(m_pEntity2d->getSurface() != nullptr){
		m_pEntity2d->setPosition(getPosX(), getPosY(), 0);
		float surfaceWidth = (float)m_pEntity2d->getSurface()->getWidth();
		float surfaceHeight = (float)m_pEntity2d->getSurface()->getHeight();
		float scaleX, scaleY, scale;

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

			switch( m_ratio) {
				case KEEP_ASPECT_RATIO:
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