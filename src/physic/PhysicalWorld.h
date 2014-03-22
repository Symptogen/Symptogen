#ifndef _H_SYMPTOGEN_PHYSIC_PHYSICAL_WORLD_H_
#define _H_SYMPTOGEN_PHYSIC_PHYSICAL_WORLD_H_

#include <Box2D/Box2D.h>
#include "ContactListener.h"

namespace Symp {

/**
	The PhysicalWorld will hold and simulate the rigid bodies.
*/
class PhysicalWorld {
public:
	PhysicalWorld();
	~PhysicalWorld();

	void updatePhysics();
	void destroyPhysicalBody(const PhysicalEntity* pEntity);

	/**
	* Getters
	*/
	inline b2Vec2 		getGravity() const {return m_gravity;}
	inline b2World* 	getWorld()const {return m_pWorld;}

private:
	b2Vec2				m_gravity;
	b2World*			m_pWorld;
	ContactListener* 	m_pContactListener;
	
	const float 		m_fTimeStep;
	const unsigned int 	m_uiVelocityIterations;
	const unsigned int 	m_uiPositionIterations;
};

}

#endif //_H_SYMPTOGEN_PHYSIC_PHYSICAL_WORLD_H_