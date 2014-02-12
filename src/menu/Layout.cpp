#include "Layout.h"

namespace Symp {

Layout::Layout()
	: GuiComponent(), m_color(0,0,0) {

	m_iColumns = m_iRows = 1;
	m_iVerticalMargin = m_iHorizontalMargin = 5;
}

Layout::Layout(float iPosX, float iPosY, int iWidth, int iHeight, LayoutFillAttribute fillAttribute)
	: GuiComponent(), m_color(0, 0, 0){

	//Default options
	m_iColumns = m_iRows = 1;
	m_iVerticalMargin = m_iHorizontalMargin = 5;

	m_iWidth = iWidth;
	m_iHeight = iHeight;
	if (fillAttribute == BACKGROUND){
		m_pEntity2d->setPrimitive2d(IND_FILL_RECTANGLE);
	}else if (fillAttribute == BORDER) {
		m_pEntity2d->setPrimitive2d(IND_RECTANGLE);
	}
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setRectangle((int)iPosX, (int)iPosY, (int)iPosX+iWidth, (int)iPosY+iHeight);
}

Layout::Layout(float iPosX, float iPosY, int iWidth, int iHeight, Color color, LayoutFillAttribute fillAttribute)
	: GuiComponent(), m_color(color.r, color.g, color.b, color.a) {

	//Default options
	m_iColumns = m_iRows = 1;
	m_iVerticalMargin = m_iHorizontalMargin = 5;

	m_iWidth = iWidth;
	m_iHeight = iHeight;
	if (fillAttribute == BACKGROUND){
		m_pEntity2d->setPrimitive2d(IND_FILL_RECTANGLE);
	}else if (fillAttribute == BORDER) {
		m_pEntity2d->setPrimitive2d(IND_RECTANGLE);
	}
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setRectangle((int)iPosX, (int)iPosY, (int)iPosX+iWidth, (int)iPosY+iHeight);
	fill(color);
}

void Layout::computeGrid(int iColumn, int iRow){
	//If need to create extras columns
	if (iColumn > m_iColumns - 1){
		m_iColumns = iColumn + 1;
	}
	//If need to create extras rows
	if (iRow > m_iRows - 1){
		m_iRows = iRow + 1;
	}
	m_iComponentWidth = m_iWidth / m_iColumns;
	m_iComponentHeight = m_iHeight / m_iRows;
}

void Layout::resizeComponents(){
	for (std::map<GuiComponent*,std::pair<int, int>>::iterator it=m_cellsMap.begin(); it != m_cellsMap.end(); ++it){

		int w = m_iComponentWidth - 2*m_iHorizontalMargin;
		int h = m_iComponentHeight - 2*m_iVerticalMargin;
		float x = getPosX() + w * it->second.first + ((it->second.first + 1)*2 -1) * m_iHorizontalMargin;
		float y = getPosY() + h * it->second.second + ((it->second.second +1)*2 -1) * m_iVerticalMargin;

		it->first->setPosition(x, y, 0);
		it->first->setWidth(w);
		it->first->setHeight(h);
		it->first->update();
	}
}

GuiComponent* Layout::getPreviousRowComponent(int row){
	GuiComponent* component;
	for (std::map<GuiComponent*,std::pair<int, int>>::iterator it=m_cellsMap.begin(); it != m_cellsMap.end(); ++it){
		if (it->second.second == row - 1){
			component = it->first;
		}
	}
	return component;
}

GuiComponent* Layout::getPreviousColumnComponent(int column){
	GuiComponent* component;
	for (std::map<GuiComponent*,std::pair<int, int>>::iterator it=m_cellsMap.begin(); it != m_cellsMap.end(); ++it){
		if (it->second.first == (column - 1)){
			component = it->first;
		}
	}
	return component;
}

//CAREFUL : not fully implemented
void Layout::relocateComponents(){
	for (std::map<GuiComponent*,std::pair<int, int>>::iterator it=m_cellsMap.begin(); it != m_cellsMap.end(); ++it){

		if (it == m_cellsMap.begin()){
			it->first->setPosition(getPosX() + m_iHorizontalMargin, getPosY() + m_iVerticalMargin, 0);
			if (it->first->getWidth() > it->first->getHeight()){
				it->first->setWidth(getWidth() - 2*m_iHorizontalMargin);
			}else{
				it->first->setHeight(getHeight() - 2*m_iVerticalMargin);
			}
			it->first->update();
		} else {

			GuiComponent* previousColumn = getPreviousColumnComponent(it->second.first);
			GuiComponent* previousRow = getPreviousRowComponent(it->second.second);
			int previousWidth = previousColumn->getWidth();
			int previousHeight = previousRow->getHeight();
			float previousX = previousColumn->getPosX();
			float previousY = previousRow->getPosY();
	 
			it->first->setPosition(previousX + (float)previousWidth + 2*(float)m_iHorizontalMargin, previousY + previousY + 2*(float)m_iVerticalMargin, 0);

			if (it == m_cellsMap.end()) {
				it->first->setWidth((int)(getPosX() + getWidth() - (previousX + previousWidth) - m_iHorizontalMargin));
				it->first->setHeight((int)(getPosY() + getHeight() - (previousY + previousHeight) - m_iVerticalMargin));
			}

			it->first->update();
		}
	}
}

//Note : les objets ajoutés à un layout sont automatiquement ajoutés au entity2DManager
void Layout::addComponent(GuiComponent* pComponent, int iColumn, int iRow, bool resizable){
	if (resizable){
		computeGrid(iColumn, iRow);
		m_components.push_back(pComponent);
		m_cellsMap.insert(std::make_pair(pComponent, std::make_pair(iColumn, iRow)));
		resizeComponents();
	} else {

		m_components.push_back(pComponent);
		m_cellsMap.insert(std::make_pair(pComponent, std::make_pair(iColumn, iRow)));
		//relocateComponents();
	}
}

void Layout::insertSpace(int iColumn, int iRow) {
	computeGrid(iColumn, iRow);
	resizeComponents();
}

void Layout::fill(Symp::Color color){
	m_pEntity2d->setTint(color.r, color.g, color.b);
	m_pEntity2d->setTransparency(color.a);
}

}