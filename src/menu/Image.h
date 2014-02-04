#ifndef _H_SYMPTOGEN_MENU_IMAGE_H_
#define _H_SYMPTOGEN_MENU_IMAGE_H_

#include "GuiComponent.h"

namespace Symp {

class Image : public GuiComponent {
public:
	Image( const char* filePath, float iPosX=0, float iPosY=0, float iScale = 1.f);
	~Image();
	virtual void update();

};

}

#endif //_H_SYMPTOGEN_MENU_IMAGE_H_