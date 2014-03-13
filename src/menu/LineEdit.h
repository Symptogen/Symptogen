#ifndef _H_SYMPTOGEN_MENU_LINE_EDIT_H_
#define _H_SYMPTOGEN_MENU_LINE_EDIT_H_

#include "GuiComponent.h"
#include "Image.h"
#include "Text.h"

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
	LineEdit(float iPosX=0, float iPosY=0, int iWidth=0, int iHeight=0);
	~LineEdit() {}

	virtual void update();
	void fill(Symp::Color color);
	void triggerFocus();

	//Getter
	bool getHasFocus() const {return m_bHasFocus;}
	Text* getTextEntity() const {return m_text;}
	std::string getText() const {return m_text->getText();}
	Image* getCursor() const {return m_pCursor;}
	IND_Timer* getTimer() const {return m_pTimer;}

	void setText(std::string text);

private:
	Text* m_text; /** < the text displayed and saved in the #LineEdit */
	bool m_bHasFocus; /** < boolean that command the timer start and the cursor */
	Image* m_pCursor; /** < the reference to the #Image thatpresents the cursor */
	int m_iOffset; /** < the offset that represents a letter space for displaying the cursor */
	int m_iCursorWidth; /** < the width of the cursor */
	IND_Timer* m_pTimer; /** < the timer for making the cursor blinks */
	int m_iCursorFreq; /** < the time between each display of the cursor */

};

}

#endif //_H_SYMPTOGEN_MENU_LINE_EDIT_H_