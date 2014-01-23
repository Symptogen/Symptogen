#ifndef _H_SYMPTOGEN_MENU_BUTTON_H_
#define _H_SYMPTOGEN_MENU_BUTTON_H_

#include "GuiComponent.h"

namespace Symp {

class Button : public GuiComponent {
public:
	Button(std::string name, const char* filePath);
	~Button();

private:


};

}

#endif //_H_SYMPTOGEN_MENU_BUTTON_H_