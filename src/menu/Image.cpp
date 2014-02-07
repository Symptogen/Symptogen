#include "Image.h"

namespace Symp {

Image::Image( const char* filePath, float iPosX, float iPosY, float iScale)
	: GuiComponent() {

	setSurface(filePath);
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setScale(iScale, iScale);
	m_iWidth = m_pEntity2d->getSurface()->getWidth() * iScale;
	m_iHeight = m_pEntity2d->getSurface()->getHeight() * iScale;
	m_bIsEnabled = true;
}

void Image::update() {
	if(m_pEntity2d->getSurface() != nullptr){
		m_pEntity2d->setPosition(getPosX(), getPosY(), 0);
		float surfaceWidth = (float)m_pEntity2d->getSurface()->getWidth();
		float surfaceHeight = (float)m_pEntity2d->getSurface()->getHeight();
		float scale;
		if ( (getWidth() != 0 && getHeight() != 0 ) && (getWidth() != surfaceWidth || getHeight() != surfaceHeight) ){
			
			if (getWidth() < getHeight()){
				if (getWidth() < surfaceWidth) {
					scale = getWidth() / surfaceWidth;
				}else {
					scale = surfaceWidth / getWidth() ;
				}
			}else {
				if (getHeight() < surfaceHeight){
					scale = getHeight() / surfaceHeight;
				} else {
					scale = surfaceHeight / getHeight();
				} 
			}
			m_pEntity2d->setScale(scale, scale);
			setWidth(scale * getWidth());
			setHeight(scale * getHeight());
		}

	}
}

}