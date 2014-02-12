#ifndef _H_SYMPTOGEN_MENU_BUTTON_H_
#define _H_SYMPTOGEN_MENU_BUTTON_H_

#include "GuiComponent.h"

/** @namespace Symp */
namespace Symp {

/**
* @brief Button class.
* This class inherits the #GuiComponent class. A #Button is a clickable and selectable component that can display
* text, or texture, or be customized with border or background. The simpliest way to use #Button is with the #Layout
* class, that handle the position and the size of the #Button. The only paramters left are the color and the type
* of #Button that will be rendered. 
* @see MenuManager
* @see Button
* @see GuiComponent
* @see Layout
*/

class Button : public GuiComponent {
public:
	Button(const char* filePath);
	Button(std::string text, Symp::Color color=Color(0,0,0), int iWeight=0);
	Button(Symp::Color color, float fPosX, float fPosY, int fWidth, int fHeight);
	~Button();

	virtual void update();

	void fill(Symp::Color color);

	//Setters
	void setText(std::string text);

	//Getters 
	std::string getText() const {return m_sText;}

private:

	std::string m_sText; /**< the text displayed on the #Button */
	Color m_color ; /**< the color that is displayed on the #Button */
};

}

#endif //_H_SYMPTOGEN_MENU_BUTTON_H_