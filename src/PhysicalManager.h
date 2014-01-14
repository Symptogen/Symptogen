#ifndef _H_SYMPTOGEN_PHYSICAL_PHYSICAL_MANAGER_H_
#define _H_SYMPTOGEN_PHYSICAL_PHYSICAL_MANAGER_H_

#include <Box2D/Box2D.h>

namespace Symp{

/**
	The PhysicalManager will hold and simulate the rigid bodies.
*/
class PhysicalManager {
public:
	PhysicalManager(float x, float y);
	~PhysicalManager();

	void updatePhysics();

	//getters
	b2World* getWorld(){return m_pWorld;}

private:
	b2Vec2			m_gravity;
	b2World*		m_pWorld;
	
	float 			m_fTimeStep;
	unsigned int 	m_uiVelocityIterations;
	unsigned int 	m_uiPositionIterations;
};

}

#endif //_H_SYMPTOGEN_PHYSICAL_PHYSICAL_MANAGER_H_