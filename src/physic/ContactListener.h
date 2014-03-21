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
	inline bool isDino(PhysicalEntity* pPhysicalEntity) const {return (pPhysicalEntity->getType() == PhysicalType::Dino) ? true : false;}
	inline bool isGround(PhysicalEntity* pPhysicalEntity) const {return (pPhysicalEntity->getType() == PhysicalType::Ground) ? true : false;}
	inline bool isFlower(PhysicalEntity* pPhysicalEntity) const {return (pPhysicalEntity->getType() == PhysicalType::Flower) ? true : false;}
	inline bool isMovableObject(PhysicalEntity* pPhysicalEntity) const {return (pPhysicalEntity->getType() == PhysicalType::MovableObject) ? true : false;}
	inline bool isDestructibleObject(PhysicalEntity* pPhysicalEntity) const {return (pPhysicalEntity->getType() == PhysicalType::DestructibleObject) ? true : false;}
	inline bool isInvisibleObject(PhysicalEntity* pPhysicalEntity) const {return (pPhysicalEntity->getType() == PhysicalType::InvisibleObject) ? true : false;}
	inline bool isSpikes(PhysicalEntity* pPhysicalEntity) const {return (pPhysicalEntity->getType() == PhysicalType::Spikes) ? true : false;}
	inline bool isFlames(PhysicalEntity* pPhysicalEntity) const {return (pPhysicalEntity->getType() == PhysicalType::Flames) ? true : false;}
	inline bool isHotZone(PhysicalEntity* pPhysicalEntity) const {return (pPhysicalEntity->getType() == PhysicalType::HotZone) ? true : false;}
	inline bool isColdZone(PhysicalEntity* pPhysicalEntity) const {return (pPhysicalEntity->getType() == PhysicalType::ColdZone) ? true : false;}
	
	size_t getIndexEntity(PhysicalEntity* pPhysicalEntity) const;

	/**
	* Update contact's flags of the 2 PhysicalEntities.
	*/
	void setContactSides(PhysicalEntity*, PhysicalEntity*);
};

}

#endif //_H_SYMPTOGEN_PHYSIC_CONTACT_LISTENER_H_