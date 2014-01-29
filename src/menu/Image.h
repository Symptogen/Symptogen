#ifndef _H_SYMPTOGEN_MENU_IMAGE_H_
#define _H_SYMPTOGEN_MENU_IMAGE_H_

#include "GuiComponent.h"

namespace Symp {

class Image : public GuiComponent {
public:
	Image( const char* filePath, int iPosX, int iPosY);
	Image( const char* filePath, int iPosX, int iPosY, int iScale);
	~Image();
	virtual void update();

};

}

#endif //_H_SYMPTOGEN_MENU_IMAGE_H_