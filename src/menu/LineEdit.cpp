#include "LineEdit.h"

/** @namespace Symp */
namespace Symp {

/**
* @brief LineEdit constructor
* Responsible for the initialization of the private attributes of #LineEdit class
* @param text default none
* @see MenuManager
* @see GuiComponent
* @see ~LineEdit()
*/
LineEdit::LineEdit(char* text) 
	: GuiComponent()
{

}

/**
* @brief LineEdit constructor
* Responsible for the initialization of the private attributes of #LineEdit class
* @param iPosX the x coordinate of the upper-left corner of the #LineEdit in pixels
* @param iPosY the y coordinate of the upper-left corner of the #LineEdit in pixels
* @param iWidth the width of the #LineEdit in pixels
* @param iHeight the height of the #LineEdit in pixels
* @param text default none
* @see MenuManager
* @see GuiComponent
* @see ~LineEdit()
*/
LineEdit::LineEdit(float iPosX, float iPosY, int iWidth, int iHeight, char* text)
	: GuiComponent()
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_pEntity2d->setPrimitive2d(IND_RECTANGLE);
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setRectangle((int)iPosX, (int)iPosY, (int)iPosX+iWidth, (int)iPosY+iHeight);
}

}