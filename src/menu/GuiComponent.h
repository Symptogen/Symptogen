#ifndef _H_SYMPTOGEN_MENU_GUI_COMPONENT_H_
#define _H_SYMPTOGEN_MENU_GUI_COMPONENT_H_

#include <Indie.h>
#include <IND_Entity2d.h>
#include <IND_Entity2dManager.h>
#include <IND_Surface.h>
#include <IND_SurfaceManager.h>
#include <IND_Font.h>
#include <IND_FontManager.h>

#include "../Render.h"

namespace Symp {

/**
* An enum.
* More detailed enum description.
*/
struct Color {
	Color(unsigned int rValue, unsigned int gValue, unsigned int bValue, unsigned int aValue=255){
		r = rValue;
		g = gValue; 
		b = bValue;
		a = aValue;
	}

	static const Color BLUE; 
	static const Color RED; 
	static const Color GREEN;
	static const Color BLACK;
	static const Color GREY;

	unsigned int r;
	unsigned int g;
	unsigned int b;
	unsigned int a;
};

/**
	Facade of IND_Entity2d for the Menu elements only
*/
class GuiComponent {
public:
	GuiComponent();
	static void init(Render* pRender);
	~GuiComponent();
	static void end();
	bool isTargetedByMouse(int mouseX, int mouseY);
	bool loadFont(const char* filePath);

	//getters
	IND_Entity2d* getIND_Entity2d(){return m_pEntity2d;}
	float getPosX(){return m_pEntity2d->getPosX();}
	float getPosY(){return m_pEntity2d->getPosY();}
	float getWidth();
	float getHeight();
	virtual void update() = 0;

	//setters
	void setSurface(const char* filePath);
	void setWidth(int width) {m_iWidth = width;}
	void setHeight(int height) {m_iHeight = height;}
	void setRectangle(int x, int y, int w, int h);
	void setPosition(float pX, float pY, float pZ){m_pEntity2d->setPosition(pX, pY, pZ);}
	void setScale(float pSx, float pSy){m_pEntity2d->setScale(pSx, pSy);}
	bool setHotSpot(float pX, float pY){return m_pEntity2d->setHotSpot(pX, pY);}

protected:
	IND_Entity2d* 				m_pEntity2d;
	static IND_ImageManager* 	s_pImageManager;
	static IND_SurfaceManager* 	s_pSurfaceManager;
	static IND_FontManager* 	s_pFontManager;
	int 						m_iWidth;
	int 						m_iHeight;

	//static IND_Entity2dManager*		s_pEntity2dManager;
};

}

#endif //_H_SYMPTOGEN_MENU_GUI_COMPONENT_H_