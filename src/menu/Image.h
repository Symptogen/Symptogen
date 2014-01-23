#ifndef _H_SYMPTOGEN_MENU_IMAGE_H_
#define _H_SYMPTOGEN_MENU_IMAGE_H_

#include "GuiComponent.h"

namespace Symp {

class Image : public GuiComponent {
public:
	Image( const char* filePath);
	~Image();

};

}

#endif //_H_SYMPTOGEN_MENU_IMAGE_H_