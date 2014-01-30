#ifndef _H_SYMPTOGEN_MENU_BUTTON_H_
#define _H_SYMPTOGEN_MENU_BUTTON_H_

#include "GuiComponent.h"

namespace Symp {

class Button : public GuiComponent {
public:
	Button(const char* filePath);
	Button(std::string text, Symp::Color color, int iWeight=0);
	Button(Symp::Color color, int fPosX, int fPosY, int fWidth, int fHeight);
	~Button();
	virtual void update();
	void fill(Symp::Color color);

	//Setters
	void setText(std::string text);

	//Getters 
	std::string getText() const {return m_sText;}

private:

	std::string m_sText;
	Color m_color ;
};

}

#endif //_H_SYMPTOGEN_MENU_BUTTON_H_