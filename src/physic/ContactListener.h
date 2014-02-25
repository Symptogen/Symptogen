#ifndef _H_SYMPTOGEN_PHYSIC_CONTACT_LISTENER_H_
#define _H_SYMPTOGEN_PHYSIC_CONTACT_LISTENER_H_

#include <Box2D/Box2D.h>
#include "PhysicalEntity.h"

namespace Symp {

/* *************************************************************************************** */
/* CLASS DEFINITION */
/* *************************************************************************************** */
/**
*	This subclass of b2ContactListener contains functions called by the PhysicalWorld (more precisely the b2World) 
*	when there is any contact between physical entities (more precisely b2Body).
*/
class ContactListener : public b2ContactListener {

public:

	/**
	* Called when two fixtures begin to touch.
	*/
	void BeginContact (b2Contact *contact);

	/**
	* Called when two fixtures cease to touch.
	*/
	void EndContact (b2Contact *contact);

private:

	/**
	* Functions to know what is the type of a physical entity.
	*/
	bool isDino(PhysicalEntity* pPhysicalEntity) const {return (pPhysicalEntity->getType() == PhysicalType::Dino) ? true : false;}
	bool isGround(PhysicalEntity* pPhysicalEntity) const {return (pPhysicalEntity->getType() == PhysicalType::Ground) ? true : false;}
	bool isFlower(PhysicalEntity* pPhysicalEntity) const {return (pPhysicalEntity->getType() == PhysicalType::Flower) ? true : false;}
	bool isMovableObject(PhysicalEntity* pPhysicalEntity) const {return (pPhysicalEntity->getType() == PhysicalType::MovableObject) ? true : false;}
};

}

#endif //_H_SYMPTOGEN_PHYSIC_CONTACT_LISTENER_H_