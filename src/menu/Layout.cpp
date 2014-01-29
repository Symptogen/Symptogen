#include "Layout.h"

namespace Symp {

Layout::Layout(float iPosX, float iPosY, float iWidth, float iHeight)
	: GuiComponent() {

	//Default options
	m_iColumns = m_iRows = 1;
	m_iVerticalMargin = m_iHorizontalMargin = 5;

	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_pEntity2d->setPrimitive2d(IND_RECTANGLE);
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setRectangle(iPosX, iPosY, iPosX+iWidth, iPosY+iHeight);
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

		int x = getPosX() + (m_iWidth / m_iColumns) * it->second.first + m_iColumns * m_iHorizontalMargin;
		int y = getPosY() + (m_iHeight / m_iRows) * it->second.second + m_iRows * m_iVerticalMargin;
		int w = m_iComponentWidth - m_iHorizontalMargin;
		int h = m_iComponentHeight - m_iVerticalMargin;

		it->first->setPosition(x, y, 0);
		it->first->setWidth(w);
		it->first->setHeight(h);
		it->first->update();
	}

}

//Note : les objets ajoutés à un layout sont automatiquement ajoutés au entity2DManager
void Layout::addComponent(GuiComponent* pComponent, int iColumn, int iRow){
	computeGrid(iColumn, iRow);
	m_components.push_back(pComponent);
	m_cellsMap.insert(std::make_pair(pComponent, std::make_pair(iColumn, iRow)));
	resizeComponents();
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