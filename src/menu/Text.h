#ifndef _H_SYMPTOGEN_MENU_TEXT_H_
#define _H_SYMPTOGEN_MENU_TEXT_H_

#include "GuiComponent.h"

/** @namespace Symp */
namespace Symp {

/**
* @brief Text class.
* This class inherits the #GuiComponent class.
* @see MenuManager
* @see Text
* @see GuiComponent
* @see Layout
*/
class Text : public GuiComponent {
public:
	Text(std::string text, Symp::Color color=Color(0,0,0), float iPosX=0, float iPosY=0);
	~Text(){}

	virtual void update();

	void fill(Symp::Color color);

	//Getters 
	std::string getText() const {return m_sText;}

private:
	std::string m_sText; /**< the text displayed on the #Text */
	Color m_color ; /**< the color that is displayed on the #Text */
};

}

#endif //_H_SYMPTOGEN_MENU_TEXT_H_