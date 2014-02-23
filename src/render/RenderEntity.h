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

	IND_Entity2d* getIND_Entity2d() const {return m_pEntity2d;}
	IND_Surface* getSurface() const {return m_pEntity2d->getSurface();}
	IND_Animation* getAnimation() const {return m_pEntity2d->getAnimation();}
	float getPosX() const {return m_pEntity2d->getPosX();}
	float getPosY() const {return m_pEntity2d->getPosY();}
	int getWidth() const;
	int getHeight() const;

	//setters
	void setSurface(const char* filePath);
	void setAnimation(const char* filePath);
	void setSequence(unsigned int pSequence){m_pEntity2d->setSequence(pSequence);}
	void setPosition(float pX,float pY){m_pEntity2d->setPosition(pX, pY, 0);}
	void setScale(float pSx, float pSy){m_pEntity2d->setScale(pSx, pSy);}
	bool setHotSpot(float pX, float pY){return m_pEntity2d->setHotSpot(pX, pY);}

private:
	IND_Entity2d*					m_pEntity2d;

	static IND_ImageManager* 		s_pImageManager;
	static IND_SurfaceManager* 		s_pSurfaceManager;
	static IND_AnimationManager*	s_pAnimationManager;
};

}

#endif //_H_SYMPTOGEN_RENDER_RENDER_ENTITY_H_