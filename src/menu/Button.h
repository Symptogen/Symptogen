#ifndef _H_SYMPTOGEN_MENU_BUTTON_H_
#define _H_SYMPTOGEN_MENU_BUTTON_H_

#include "GuiComponent.h"

namespace Symp {

class Button : public GuiComponent {
public:
	Button(std::string name, const char* filePath);
	Button(std::string name, Symp::Color color, float iPosX, float iPosY, float iWidth, float iHeight);
	~Button();

	void fill(Symp::Color color);
	void setText(std::string text);

	//Setters
	void enable();
	void disable();

	//Getters 
	bool isEnabled() const {return m_bIsEnabled;}

private:
	bool m_bIsEnabled;
	//Symp::Color color;

};

}

#endif //_H_SYMPTOGEN_MENU_BUTTON_H_