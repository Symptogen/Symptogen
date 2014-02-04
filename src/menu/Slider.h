#ifndef _H_SYMPTOGEN_MENU_SLIDER_H_
#define _H_SYMPTOGEN_MENU_SLIDER_H_

#include "GuiComponent.h"

namespace Symp {

class Slider : public GuiComponent {
public:
	Slider( int percentage, float iPosX=0, float iPosY=0, int iWidth = 0, int iHeight =0);
	~Slider();
	virtual void update();

	void fill(Color color);

	//Setter
	void setPercentage(int iPercentage) { m_iPercentage = iPercentage;}

	//Getter
	int getPercentage() const {return m_iPercentage;}

private:
	int m_iPercentage;

};

}

#endif //_H_SYMPTOGEN_MENU_SLIDER_H_