#ifndef _H_SYMPTOGEN_MENU_LAYOUT_H_
#define _H_SYMPTOGEN_MENU_LAYOUT_H_

#include "GuiComponent.h"
#include <map>

namespace Symp {

class Layout : public GuiComponent {
public:
	Layout(float iPosX, float iPosY, int iWidth, int iHeight);
	Layout(float iPosX, float iPosY, int iWidth, int iHeight, Color color);
	~Layout();
	void addComponent(GuiComponent* pComponent, int column, int row);
	void fill(Symp::Color color);
	void insertSpace(int iColumn, int iRow);
	void computeGrid(int iColumn, int iRow);
	void resizeComponents();
	virtual void update(){}

	//Getters
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