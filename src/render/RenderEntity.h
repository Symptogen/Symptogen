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
* This enum is used to know if we want the render entity to be a surface, or an animation.
*/
enum RenderType{
	Surface, 
	Animation
};

/* *************************************************************************************** */
/* CLASS DEFINITION */
/* *************************************************************************************** */
/**
*	Facade of IND_Entity2d.
*	This class corresponds to all our render elements, visible in the game.
*	They are store in the array m_renderEntityArray, contains in the EntityManager.
*	Warning : in the menu, we use GuiComponent (an other class, and an other facade of IND_Entity2d).
*	It provides tools to : 
*		- Set a surface
*		- Set an animation
*	@see EntityManager
*/
class RenderEntity {

public:

	/**
	* @brief Create the render entity.
	* Create a render entity means creating an IND_Entity2d, set a surface or an animtion to it (depend on the renderType parameter), 
	* add the surface or the animation to the right manager (IND_SurfaceManager, or IND_AnimationManager).
	* Warning : this function closes the application if the filePath is wrong.
	*/
	RenderEntity(const char* filePath, RenderType renderType);

	/**
	* @brief Set the render environment, needed to create render entities.
	* Initialize an IND_ImageManager, a IND_SurfaceManager, and an IND_AnimationManager.
	*/
	static void init(Render* pRender);

	/**
	* @brief Delete the render entity.
	*/
	~RenderEntity();

	/**
	* @brief Deallocate the render environment.
	* Deallocate the IND_ImageManager, the IND_SurfaceManager, and the IND_AnimationManager.
	*/
	static void end();

	/**
	* Getters
	*/
	IND_Entity2d* getIND_Entity2d() const {return m_pEntity2d;}
	IND_Surface* getSurface() const {return m_pEntity2d->getSurface();}
	IND_Animation* getAnimation() const {return m_pEntity2d->getAnimation();}
	/**
	* Get the center X of the render entity.
	*/
	float getPosX() const {return m_pEntity2d->getPosX();}
	/**
	* Get the center Y of the render entity.
	*/
	float getPosY() const {return m_pEntity2d->getPosY();}
	/**
	* Get the real size of the render entity (includes the scale factor). 
	* Warning : if the render entity is an animtion, return the width of sequence 0.
	*/
	int getWidth() const;
	/**
	* Get the real size of the render entity (includes the scale factor). 
	* Warning : if the render entity is an animtion, return the height of sequence 0.
	*/
	int getHeight() const;

	/**
	* Setters
	*/
	void setSurface(const char* filePath);
	void setAnimation(const char* filePath);
	void setSequence(unsigned int pSequence){m_pEntity2d->setSequence(pSequence);}
	void setPosition(float pX,float pY){m_pEntity2d->setPosition(pX, pY, 0);}
	void setScale(float pSx, float pSy){m_pEntity2d->setScale(pSx, pSy);}
	/**
	* The hot spot is the center of the possible rotation of the render entity.
	*/
	bool setHotSpot(float pX, float pY){return m_pEntity2d->setHotSpot(pX, pY);}

private:
	IND_Entity2d*					m_pEntity2d;

	static IND_ImageManager* 		s_pImageManager;
	static IND_SurfaceManager* 		s_pSurfaceManager;
	static IND_AnimationManager*	s_pAnimationManager;
};

}

#endif //_H_SYMPTOGEN_RENDER_RENDER_ENTITY_H_