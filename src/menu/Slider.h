#ifndef _H_SYMPTOGEN_MENU_SLIDER_H_
#define _H_SYMPTOGEN_MENU_SLIDER_H_

#include "GuiComponent.h"

#include "Image.h"

namespace Symp {

class Slider : public GuiComponent {
public:
	Slider( float percentage, float iPosX=0, float iPosY=0, int iWidth = 0, int iHeight =0);
	~Slider();
	virtual void update();

	void fill(Color color);

	//Setter
	void setPercentage(int fPercentage) { m_fPercentage = fPercentage;}

	//Getter
	Image* getImage() const {return m_pImage;}
	float getPercentage() const {return m_fPercentage;}

private:
	float m_fPercentage;
	Image* m_pImage;

};

}

#endif //_H_SYMPTOGEN_MENU_SLIDER_H_