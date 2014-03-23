#include "LineEdit.h"
#include "MenuManager.h"

/** @namespace Symp */
namespace Symp {

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
LineEdit::LineEdit(float iPosX, float iPosY, int iWidth, int iHeight)
	: GuiComponent()
{
	m_iCursorWidth = 3;
	m_iCursorFreq = 1;
	m_iOffset = 8;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_pTimer = new IND_Timer();
	m_pEntity2d->setPrimitive2d(IND_FILL_RECTANGLE);
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setRectangle((int)iPosX, (int)iPosY, (int)iPosX+iWidth, (int)iPosY+iHeight);
	fill(Symp::Color::WHITE);

	// Init the foreground texture
	m_pCursor = new Image("../assets/menu/slider.png", iPosX  + m_iOffset , iPosY);
	m_pCursor->setAspectRatio(AspectRatio::IGNORE_ASPECT_RATIO);
	m_pCursor->getIND_Entity2d()->setPosition(getPosX() + m_iOffset, getPosY(), 0);
	m_pCursor->setWidth(m_iCursorWidth);
	m_pCursor->setHeight(m_iHeight - 5);
	m_pCursor->getIND_Entity2d()->setShow(false);

	// Initialize the text
	m_text = new Text("", Color::WHITE, getPosX(), getPosY());
	m_text->getIND_Entity2d()->setAlign(IND_LEFT);
	m_iCursorPos = 0;

	m_bHasFocus = false;
	enable();
}

/**
* @brief LineEdit update fonction
* Refresh the display of the #LineEdit.
* @see LineEdit
* @see ~LineEdit()
* @see GuiComponent
*/
void LineEdit::update(){
	m_pEntity2d->setPosition(getPosX(), getPosY(), 0);
	m_pEntity2d->setRectangle((int)getPosX(), (int)getPosY(), (int)getPosX() + getWidth(), (int)getPosY() + getHeight());

	m_pCursor->setPosition(m_text->getPosX() + m_iCursorPos*22, m_text->getPosY(), 0);

	//Handle events
	if(m_bHasFocus){
		//Impossible to make the cursor blinks because the update is not done regularly
		fill(Color(180, 180, 180));
	}else{
		fill(Symp::Color::WHITE);
	}

	//Update the cursor
	if (m_pCursor->getIND_Entity2d()->getSurface()->getWidth() != m_iCursorWidth || m_pCursor->getIND_Entity2d()->getSurface()->getHeight() != getHeight() - 5){
		//m_pCursor->getIND_Entity2d()->setPosition((int)getPosX(), (int)getPosY(), 0);
		m_pCursor->getIND_Entity2d()->setRegion(0, 0, m_pCursor->getIND_Entity2d()->getSurface()->getWidth(), m_pCursor->getIND_Entity2d()->getSurface()->getHeight());
	}
	m_pCursor->update();
}

/**
* @brief fill LineEdit's background function
* @see LineEdit
* @see ~LineEdit()
* @see Color
* @see GuiComponent
*/
void LineEdit::fill(Symp::Color color){
	m_pEntity2d->setTint(color.r, color.g, color.b);
	m_pEntity2d->setTransparency(color.a);
}

/**
* @brief trigger the focus on the LineEdit
* @see LineEdit
* @see ~LineEdit()
* @see GuiComponent
*/
void LineEdit::triggerFocus(){
	if (m_bHasFocus){
		m_bHasFocus = false;
		m_pCursor->getIND_Entity2d()->setShow(false);
		MenuManager::getInstance()->setHasLineEditFocus(false);
		m_pTimer->stop();
		update();
	} 
	else {
		m_bHasFocus = true;

		m_pCursor->getIND_Entity2d()->setShow(true);
		MenuManager::getInstance()->setHasLineEditFocus(true);
		m_pTimer->start();
		update();
	}
}

void LineEdit::setText(std::string text){
	m_text->setText(text);
	m_iCursorPos = text.size();
	update();
}

void LineEdit::moveCursorLeft(){
	if (m_iCursorPos != 0){
		m_iCursorPos = m_iCursorPos - 1;
	}
	update();
}

void LineEdit::moveCursorRight(){
	if (m_iCursorPos != getText().size()){
		m_iCursorPos = m_iCursorPos + 1;
	}
	update();
}

void LineEdit::erasePreviousToCursor(){
	std::string text = getText();
	int currentCursorPos = m_iCursorPos;
	if(m_iCursorPos-1 < text.size()) {
		text.erase(text.begin() + m_iCursorPos-1);
		setText(text);
		m_iCursorPos = currentCursorPos-1;
	}
}

void LineEdit::eraseNextToCursor(){
	std::string text = getText();
	int currentCursorPos = m_iCursorPos;
	if(m_iCursorPos < text.size()) {
		text.erase(text.begin() + m_iCursorPos);
		setText(text);
		m_iCursorPos = currentCursorPos;
	}
}


}