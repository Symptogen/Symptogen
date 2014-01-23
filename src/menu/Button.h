#ifndef _H_SYMPTOGEN_MENU_BUTTON_H_
#define _H_SYMPTOGEN_MENU_BUTTON_H_

#include "GuiComponent.h"

namespace Symp {

class Button : public GuiComponent {
public:
	Button(const char* filePath);
	Button(std::string text, Symp::Color color);
	Button(Symp::Color color, float iPosX, float iPosY, float iWidth, float iHeight);
	~Button();

	void fill(Symp::Color color);

	//Setters
	void enable();
	void disable();
	void setText(std::string text);
	virtual void update();

	//Getters 
	bool isEnabled() const {return m_bIsEnabled;}
	std::string getText() const {return m_sText;}

private:
	bool m_bIsEnabled;
	std::string m_sText;
	Color m_color ;

};

}

#endif //_H_SYMPTOGEN_MENU_BUTTON_H_