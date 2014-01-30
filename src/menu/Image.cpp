#include "Image.h"

namespace Symp {

Image::Image( const char* filePath, int iPosX, int iPosY)
	: GuiComponent() {

	setSurface(filePath);
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_bIsEnabled = true;
}

Image::Image ( const char* filePath, int iPosX, int iPosY, int iScale)
	: GuiComponent() {
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setScale(iScale, iScale);
	m_bIsEnabled = true;
}

void Image::update() {

}

}