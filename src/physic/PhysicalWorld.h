#ifndef _H_SYMPTOGEN_PHYSIC_PHYSICAL_WORLD_H_
#define _H_SYMPTOGEN_PHYSIC_PHYSICAL_WORLD_H_

#include <Box2D/Box2D.h>

namespace Symp {

/**
	The PhysicalWorldwill hold and simulate the rigid bodies.
*/
class PhysicalWorld {
public:
	PhysicalWorld();
	~PhysicalWorld();

	void updatePhysics();

	//getters
	b2World* getWorld(){return m_pWorld;}

private:
	b2Vec2				m_gravity;
	b2World*			m_pWorld;
	
	const float 		m_fTimeStep;
	const unsigned int 	m_uiVelocityIterations;
	const unsigned int 	m_uiPositionIterations;
};

}

#endif //_H_SYMPTOGEN_PHYSIC_PHYSICAL_WORLD_H_