#include "Layout.h"

/** @namespace Symp */
namespace Symp {

/**
* @brief Layout constructor 
* Responsible for the initialization of the private attributes of #Layout class. This simple constructor is meant to
* be used in the case you want to add a #Layout in an other #Layout, so the #Layout container will manage this #Layout
* size and position.
* @see Color
* @see GuiComponent
* @see Layout
* @see Layout()
* @see ~Layout()
*/
Layout::Layout()
	: GuiComponent(), m_color(0,0,0) {

	m_iColumns = m_iRows = 1;
	m_iVerticalMargin = m_iHorizontalMargin = 5;
}


/**
* @brief Layout constructor 
* Responsible for the initialization of the private attributes of #Layout class. This is the complete version of the 
* #Layout constructor. If this #Layout is meant to be used in an other #Layout, please use the simple constructor 
* instead of this one.
* @param iPosX the x coordinate of the upper-left corner of the #Layout in pixels
* @param iPosY the y coordinate of the upper-left corner of the #Layout in pixels
* @param iWidth the width of the #Layout in pixels
* @param iHeight the height of the #Layout in pixels
* @param color the color of the #Layout that will be rendered following the #LayoutFillAttribute, white by default
* @param LayoutFillAttribute the type of customization wanted for the #Layout
* @see Color
* @see GuiComponent
* @see LayoutFillAttributes
* @see Layout()
* @see ~Layout()
*/
Layout::Layout(float iPosX, float iPosY, int iWidth, int iHeight, Color color, LayoutFillAttribute fillAttribute)
	: GuiComponent(), m_color(color.r, color.g, color.b, color.a){

	//Default options
	m_iColumns = m_iRows = 1;
	m_iVerticalMargin = m_iHorizontalMargin = 5;
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	// Choose the primitive to draw following the LayoutFillAttribute
	if (fillAttribute == BACKGROUND){
		m_pEntity2d->setPrimitive2d(IND_FILL_RECTANGLE);
	}else if (fillAttribute == BORDER) {
		m_pEntity2d->setPrimitive2d(IND_RECTANGLE);
	}
	m_pEntity2d->setPosition(iPosX, iPosY, 0);
	m_pEntity2d->setRectangle((int)iPosX, (int)iPosY, (int)iPosX+iWidth, (int)iPosY+iHeight);
	
	fill(m_color);
}

/**
* @brief Compute the characteristic of the #Layout grid
* Given a couple of column / row indexes reference, if they are non existing yet, this function determine the standard 
* size of a #GuiComponent into the grid. This function is called whenever a #GuiComponent is added to the grid. 
* The couple column/row is the position of the #GuiComponent, so if a component is placed in a new column or in a new 
* row, the grid adjusts itself and all the components in it. 
* @param iColumn the column index reference
* @param iRow the row index reference
* @see resizeComponents()
* @see addComponent()
* @see GuiComponent
* @see Layout()
* @see ~Layout()
*/
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

/**
* @brief Resize one by one the #GuiComponent contained in the #Layout grid
* Using the reference width and height calculated by the #computeGrid() function, this function retrieve the 
* #GuiComponent of the #Layout and following their position in the grid, update them with the right size and position.
* @see computeGrid()
* @see addComponent()
* @see GuiComponent
* @see Layout()
* @see ~Layout()
*/
void Layout::resizeComponents(){
	//Retrieve all the components
	for (std::map<GuiComponent*,std::pair<int, int> >::iterator it=m_cellsMap.begin(); it != m_cellsMap.end(); ++it){

		// Determine size and position for the compponent
		int w = m_iComponentWidth - 2*m_iHorizontalMargin;
		int h = m_iComponentHeight - 2*m_iVerticalMargin;
		float x = getPosX() + w * it->second.first + ((it->second.first + 1)*2 -1) * m_iHorizontalMargin;
		float y = getPosY() + h * it->second.second + ((it->second.second +1)*2 -1) * m_iVerticalMargin;
 		
		//Update the component
		it->first->setPosition(x, y, 0);
		it->first->setWidth(w);
		it->first->setHeight(h);
		it->first->update();
	}
}

/**
* @brief Add a #GuiComponent into the grid to the given row and column indexes
* When a new component is added to the grid, the grid is re-computed and all the components are updated to the 
* new standard component's size. When a #GuiComponent is added to a #Layout, there is no need to render it to also add
* the #GuiComponent to the #MenuManager : the #Layout is responsible of adding its components to the #MenuManager. If 
* the #GuiComponent is added to the #Layout with no need to manage its size and its position, them the attribute
* resizable need to be set to false, so the grid will not manage this component, true by default.
* @param pComponent the reference to the #GuiComponent to add
* @param iColumn the column index of the component
* @param iRow the row index of the component
* @param bResizable wether the grid needs to manage this component size and position or not (default = true )
* @see computeGrid()
* @see resizeComponent()
* @see GuiComponent
* @see Layout()
* @see ~Layout()
*/
void Layout::addComponent(GuiComponent* pComponent, int iColumn, int iRow, bool bResizable){
	if (bResizable){
		computeGrid(iColumn, iRow);
		m_components.push_back(pComponent);
		m_cellsMap.insert(std::make_pair(pComponent, std::make_pair(iColumn, iRow)));
		resizeComponents();
	} else {
		m_components.push_back(pComponent);
		m_cellsMap.insert(std::make_pair(pComponent, std::make_pair(iColumn, iRow)));
	}
}

/**
* @brief Insert a blanck space the size of a standard grid component into the #Layout grid
* It is possible to add a space wherever into the grid, but its size as every component of the grid will be 
* standard.
* @param iColumn the column index of the component
* @param iRow the row index of the component
* @see addComponent()
* @see GuiComponent
* @see Layout()
* @see ~Layout()
*/
void Layout::insertSpace(int iColumn, int iRow) {
	computeGrid(iColumn, iRow);
	resizeComponents();
}

/**
* @brief fill #Layout's background or border following the #LayoutFillAttribute
* @see Layout
* @see ~Layout()
* @see Color
* @see GuiComponent
*/
void Layout::fill(Symp::Color color){
	m_pEntity2d->setTint(color.r, color.g, color.b);
	m_pEntity2d->setTransparency(color.a);
}

}