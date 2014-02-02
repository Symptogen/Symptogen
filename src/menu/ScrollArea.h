#ifndef _H_SYMPTOGEN_MENU_SCROLL_AREA_H_
#define _H_SYMPTOGEN_MENU_SCROLL_AREA_H_

#include "GuiComponent.h"

namespace Symp {

class ScrollArea : public GuiComponent {
public:
	ScrollArea();
	ScrollArea(float iPosX, float iPosY, int iWidth, int iHeight);
	~ScrollArea(){}
	virtual void update(){}

};

}

#endif //_H_SYMPTOGEN_MENU_SCROLL_AREA_H_