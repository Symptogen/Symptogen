#ifndef _H_SYMPTOGEN_MENU_GUI_COMPONENT_H_
#define _H_SYMPTOGEN_MENU_GUI_COMPONENT_H_

#include <Indie.h>
#include <IND_Entity2d.h>
#include <IND_Entity2dManager.h>
#include <IND_Surface.h>
#include <IND_SurfaceManager.h>

#include "../Render.h"

namespace Symp {

/**
	Facade of IND_Entity2d for the Menu elements only
*/
class GuiComponent {
public:
	GuiComponent(std::string name);
	static void init(Render* pRender);
	~GuiComponent();
	static void end();
	bool isTargetedByMouse(int mouseX, int mouseY);

	//getters
	std::string getName() { return m_sName;}
	IND_Entity2d* getIND_Entity2d(){return m_pEntity2d;}
	float getPosX(){return m_pEntity2d->getPosX();}
	float getPosY(){return m_pEntity2d->getPosY();}
	float getWidth(){return m_pEntity2d->getSurface()->getWidth();}
	float getHeight(){return m_pEntity2d->getSurface()->getHeight();}

	//setters
	void setSurface(const char* filePath);
	void setPosition(float pX, float pY, float pZ){m_pEntity2d->setPosition(pX, pY, pZ);}
	void setScale(float pSx, float pSy){m_pEntity2d->setScale(pSx, pSy);}
	bool setHotSpot(float pX, float pY){return m_pEntity2d->setHotSpot(pX, pY);}

private:
	IND_Entity2d* 				m_pEntity2d;
	std::string 				m_sName;
	static IND_ImageManager* 	s_pImageManager;
	static IND_SurfaceManager* 	s_pSurfaceManager;

	//static IND_Entity2dManager*		s_pEntity2dManager;
};

}

#endif //_H_SYMPTOGEN_MENU_GUI_COMPONENT_H_