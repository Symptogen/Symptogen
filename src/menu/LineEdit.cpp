#include "LineEdit.h"

namespace Symp {

LineEdit::LineEdit(char* text) 
	: GuiComponent()
{

}

LineEdit::LineEdit(float iPosX, float iPosY, int iWidth, int iHeight, char* text)
	: GuiComponent()
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_pEntity2d->setPrimitive2d(IND_RECTANGLE);
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setRectangle((int)iPosX, (int)iPosY, (int)iPosX+iWidth, (int)iPosY+iHeight);
}

void LineEdit::update(){
	
}
}