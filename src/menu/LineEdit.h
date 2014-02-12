#ifndef _H_SYMPTOGEN_MENU_LINE_EDIT_H_
#define _H_SYMPTOGEN_MENU_LINE_EDIT_H_

#include "GuiComponent.h"

/** @namespace Symp */
namespace Symp {

/**
* @class LineEdit class inherits the #GuiComponent class
* The #LineEdit class is part of the menu graphical components and can be used only in the menu context. A #LineEdit
* presents a convenient text input widget that can display the written text, and save it.
* @todo do the class with last version of Indielib font management
* @see MenuManager
* @see GuiComponent
* @see LineEdit()
* @see ~LineEdit()
*/
class LineEdit : public GuiComponent {
public:
	LineEdit(char* text=0);
	LineEdit(float iPosX, float iPosY, int iWidth, int iHeight, char* text=0);
	~LineEdit() {}

	virtual void update(){}

};

}

#endif //_H_SYMPTOGEN_MENU_LINE_EDIT_H_