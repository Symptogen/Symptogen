#ifndef _H_SYMPTOGEN_RENDER_RENDER_ENTITY_H_
#define _H_SYMPTOGEN_RENDER_RENDER_ENTITY_H_

#include <Indie.h>
#include <IND_Entity2d.h>
#include <IND_Entity2dManager.h>
#include <IND_Surface.h>
#include <IND_SurfaceManager.h>
#include <IND_Animation.h>
#include <IND_AnimationManager.h>

#include "Render.h"

namespace Symp {

/**
	Facade of IND_Entity2d.
*/
enum RenderType{Surface, Animation};

class RenderEntity {
public:
	RenderEntity(const char* filePath, RenderType renderType);
	static void init(Render* pRender);
	~RenderEntity();
	static void end();

	//getters
	//static IND_Entity2dManager* getEntity2dManager(){return s_pEntity2dManager;}

	IND_Entity2d* getIND_Entity2d(){return m_pEntity2d;}
	IND_Surface* getSurface(){return m_pEntity2d->getSurface();}
	IND_Animation* getAnimation(){return m_pEntity2d->getAnimation();}
	float getPosX(){return m_pEntity2d->getPosX();}
	float getPosY(){return m_pEntity2d->getPosY();}

	//setters
	void setSurface(const char* filePath);
	void setAnimation(const char* filePath);
	void setSequence(unsigned int pSequence){m_pEntity2d->setSequence(pSequence);}
	void setPosition(float pX,float pY, int pZ){m_pEntity2d->setPosition(pX, pY, pZ);}
	void setScale(float pSx, float pSy){m_pEntity2d->setScale(pSx, pSy);}
	bool setHotSpot(float pX, float pY){return m_pEntity2d->setHotSpot(pX, pY);}

private:
	IND_Entity2d*					m_pEntity2d;

	static IND_ImageManager* 		s_pImageManager;
	static IND_SurfaceManager* 		s_pSurfaceManager;
	static IND_AnimationManager*	s_pAnimationManager;
	//static IND_Entity2dManager*		s_pEntity2dManager;
};

}

#endif //_H_SYMPTOGEN_RENDER_RENDER_ENTITY_H_