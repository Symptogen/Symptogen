#ifndef _H_SYMPTOGEN_MENU_LAYOUT_H_
#define _H_SYMPTOGEN_MENU_LAYOUT_H_

#include "GuiComponent.h"
#include <map>

namespace Symp {

enum LayoutFillAttribute {
	BORDER,
	BACKGROUND,
	NONE
};

class Layout : public GuiComponent {
public:
	Layout();
	Layout(float iPosX, float iPosY, int iWidth, int iHeight, LayoutFillAttribute fillAttribute = LayoutFillAttribute::NONE);
	Layout(float iPosX, float iPosY, int iWidth, int iHeight, Color color, LayoutFillAttribute fillAttribute = LayoutFillAttribute::NONE);
	~Layout();
	void addComponent(GuiComponent* pComponent, int column, int row, bool resizable=true);
	void fill(Symp::Color color);
	void insertSpace(int iColumn, int iRow);
	void computeGrid(int iColumn, int iRow);
	void resizeComponents();
	void relocateComponents();
	GuiComponent* getPreviousRowComponent(int row);
	GuiComponent* getPreviousColumnComponent(int column);
	virtual void update(){}

	//Getters
	int getVerticalMargin() const {return m_iVerticalMargin;}
	int getHorizontalMargin() const {return m_iHorizontalMargin;}
	std::vector<GuiComponent*> getComponents() const {return m_components;}

	//Setters
	void setHorizontalMargin(int iMargin) {m_iHorizontalMargin = iMargin;}
	void setVerticalMargin(int iMargin) {m_iVerticalMargin = iMargin;}
	void setMargins(int iMargin) {m_iHorizontalMargin = m_iVerticalMargin = iMargin;}

private:
	std::vector<GuiComponent*> m_components;
	std::map<GuiComponent*, std::pair<int, int> > m_cellsMap;
	int m_iColumns;
	int m_iRows;
	int m_iComponentWidth;
	int m_iComponentHeight;
	int m_iHorizontalMargin;
	int m_iVerticalMargin;
	Color m_color ;

};

}

#endif //_H_SYMPTOGEN_MENU_LAYOUT_H_