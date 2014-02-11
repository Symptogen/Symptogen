#ifndef _H_SYMPTOGEN_MENU_IMAGE_H_
#define _H_SYMPTOGEN_MENU_IMAGE_H_

#include "GuiComponent.h"

namespace Symp {

enum AspectRatio {
	IGNORE_ASPECT_RATIO,
	KEEP_ASPECT_RATIO,
	EXPAND_ASPECT_RATIO
};


class Image : public GuiComponent {
public:
	Image( const char* filePath, float iPosX=0, float iPosY=0, float iScale = 1.f);
	~Image();
	virtual void update();

	void show();
	void hide();

	//Getter
	AspectRatio getAspectRatio() const {return m_ratio;}

	//Setter
	void setAspectRatio(AspectRatio ratio) { m_ratio = ratio;}

private:
	AspectRatio m_ratio;

};

}

#endif //_H_SYMPTOGEN_MENU_IMAGE_H_