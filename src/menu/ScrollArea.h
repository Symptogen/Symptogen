#ifndef _H_SYMPTOGEN_MENU_SCROLL_AREA_H_
#define _H_SYMPTOGEN_MENU_SCROLL_AREA_H_

#include "GuiComponent.h"

/** @namespace Symp */
namespace Symp {

/**
* @class ScrollArea class inherits the #GuiComponent class
* The #ScrollArea class is part of the menu graphical components and can be used only in the menu context. A #ScrollArea
* can contain #GuiComponent and hide or show only a region of them. The #ScrollArea is connected to a #Slider for 
* displacing the region shown.
* @todo do the class
* @see MenuManager
* @see Slider
* @see GuiComponent
* @see ScrollArea()
* @see ~ScrollArea()
*/
class ScrollArea : public GuiComponent {
public:
	ScrollArea();
	ScrollArea(float iPosX, float iPosY, int iWidth, int iHeight);
	~ScrollArea(){}
	
	virtual void update(){}

};

}

#endif //_H_SYMPTOGEN_MENU_SCROLL_AREA_H_