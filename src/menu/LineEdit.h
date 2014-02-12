#ifndef _H_SYMPTOGEN_MENU_LINE_EDIT_H_
#define _H_SYMPTOGEN_MENU_LINE_EDIT_H_

#include "GuiComponent.h"

namespace Symp {

class LineEdit : public GuiComponent {
public:
	LineEdit(char* text=0);
	LineEdit(float iPosX, float iPosY, int iWidth, int iHeight, char* text=0);
	~LineEdit() {}
	virtual void update();

};

}

#endif //_H_SYMPTOGEN_MENU_LINE_EDIT_H_