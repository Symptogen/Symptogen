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
	Text(std::string text, Symp::Color color=Color(0,0,0), float iPosX=0, float iPosY=0,bool b_smallFont=false);
	~Text(){}

	virtual void update();

	void fill(Symp::Color color);
	void centerX(int yPos);
	void centerY(int xPos);

	//Getters 
	std::string getText() const {return m_sText;}
	void setText(std::string text) {m_sText = text; m_pEntity2d->setText(text.c_str());}

private:
	IND_Font* m_pFont; /**<store the font currently displayed by the #Text*/
	bool b_smallFont; /**< wether the small font need to be used. */
	std::string m_sText; /**< the text displayed on the #Text */
	Color m_color ; /**< the color that is displayed on the #Text */
};

}

#endif //_H_SYMPTOGEN_MENU_TEXT_H_