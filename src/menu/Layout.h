#ifndef _H_SYMPTOGEN_MENU_LAYOUT_H_
#define _H_SYMPTOGEN_MENU_LAYOUT_H_

#include "GuiComponent.h"
#include <map>

/** @namespace Symp */
namespace Symp {

/**
* @enum LayoutFillAttribute enum 
* The #LayoutFillAttribute enum presents convenient fields for the choice of the type of #Layout that will be used.
* The customization of a #Layout can be made following these three types : make a border, or make a background, or
* do not show the #Layout. When a #Layout is used with the border or background customization type selected, then the
* unique #Color given in argument is used wether for the border, or the background. Impossible to use both.
* @see GuiComponent
* @see Layout
* @see Color
*/
enum LayoutFillAttribute {
	BORDER,  /*!< Choose to apply the color of the #Layout to its border */
	BACKGROUND,  /*!< Choose to apply the color of the #Layout to its background */
	NONE  /*!< Choose not to show the #Layout */
};

/**
* @class Layout class inherits the #GuiComponent class
* The #Layout class is part of the menu graphical components and can be used only in the menu context. A #Layout is
* meant to group other #GuiComponents and to organize their display in a regular grid. It is possible to use the #Layout 
* to group #GuiComponent and to manually set the components position, without organizing them automatically into a grid.
* A #Layout can be cutomized following the #LayoutFillAttribute. As every #GuiComponent, to be displayed, the #Layout 
* must be added to the #MenuManager. Moreover, a #Layout automatically add its components to the #MenuManager. A #Layout
* can contain other #Layout and manage them as if they are simple #GuiComponent.
* @see MenuManager
* @see GuiComponent
* @see LayoutFillAttribute
* @see Layout()
* @see ~Layout()
*/
class Layout : public GuiComponent {
public:
	Layout();
	Layout(float iPosX, float iPosY, int iWidth, int iHeight, Color color = Color(0, 0, 0), LayoutFillAttribute fillAttribute = LayoutFillAttribute::NONE);
	~Layout();

	virtual void update(){}

	void addComponent(GuiComponent* pComponent, int column, int row, bool bResizable=true);
	void fill(Symp::Color color);
	void insertSpace(int iColumn, int iRow);
	void computeGrid(int iColumn, int iRow);
	void resizeComponents();
	
	//Getters
	int getVerticalMargin() const {return m_iVerticalMargin;}
	int getHorizontalMargin() const {return m_iHorizontalMargin;}
	std::vector<GuiComponent*> getComponents() const {return m_components;}

	//Setters
	void setHorizontalMargin(int iMargin) {m_iHorizontalMargin = iMargin;}
	void setVerticalMargin(int iMargin) {m_iVerticalMargin = iMargin;}
	void setMargins(int iMargin) {m_iHorizontalMargin = m_iVerticalMargin = iMargin;}

private:
	std::vector<GuiComponent*> m_components; /**< the list of #GuiComponents contained into the #Layout */
	std::map<GuiComponent*, std::pair<int, int> > m_cellsMap; /**< associate a #GuiComponent to its position into the #Layout grid */
	int m_iColumns; /**< the number of columns that composed the #Layout grid */
	int m_iRows; /**< the number of rows that composed the #Layout grid */
	int m_iComponentWidth; /**< the standard width of a #GuiComponent contained in the #Layout grid in pixels */
	int m_iComponentHeight; /**< the standard width of a #GuiComponent contained in the #Layout grid in pixels */
	int m_iHorizontalMargin; /**< the horizontal margin between each #GuiComponent in the #Layout grid in pixels */
	int m_iVerticalMargin; /**< the vertical margin between each #GuiComponent in the #Layout grid in pixels  */
	Color m_color ; /**< the #Color of the #Layout */

};

}

#endif //_H_SYMPTOGEN_MENU_LAYOUT_H_