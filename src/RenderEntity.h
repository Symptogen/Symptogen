#ifndef _RENDER_ENTITY_
#define _RENDER_ENTITY_

#include "IND_Entity2d.h"

/**
	Facade of IND_Entity2d.
*/
class RenderEntity {
public:
	RenderEntity(){
		init(NULL);
	}
	RenderEntity(IND_Surface* pNewSurface){
		init(pNewSurface);
	}

	IND_Entity2d* getEntity2d(){return m_pEntity2d;}
	IND_Surface* getSurface(){return m_pEntity2d->getSurface();}
	float getPosX(){return m_pEntity2d->getPosX();}
	float getPosY(){return m_pEntity2d->getPosY();}

	void setSurface(IND_Surface* pNewSurface){m_pEntity2d->setSurface(pNewSurface);}
	void setAnimation(IND_Animation *pNewAnimation){m_pEntity2d->setAnimation(pNewAnimation);}
	void setSequence(unsigned int pSequence){m_pEntity2d->setSequence(pSequence);}
	void setPosition(float pX, float pY, float pZ){m_pEntity2d->setPosition(pX, pY, pZ);}
	void setScale(float pSx, float pSy){m_pEntity2d->setScale(pSx, pSy);}
	bool setHotSpot(float pX, float pY){return m_pEntity2d->setHotSpot(pX, pY);}

private:
	void init(IND_Surface* pNewSurface){
		m_pEntity2d = IND_Entity2d::newEntity2d();
		setSurface(pNewSurface);
	}

	IND_Entity2d*	m_pEntity2d;
};

#endif