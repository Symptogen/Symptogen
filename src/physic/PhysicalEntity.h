#ifndef _H_SYMPTOGEN_PHYSIC_PHYSICAL_ENTITY_H_
#define _H_SYMPTOGEN_PHYSIC_PHYSICAL_ENTITY_H_

#include <Box2D/Box2D.h>
#include "../render/Render.h"

namespace Symp {

/**
	Facade of Box2D entity.
*/
class PhysicalEntity {
public:
	PhysicalEntity(b2World* world, b2Vec2 dimensions);
	~PhysicalEntity();

	//getters
	b2Body* 	getb2Body() const {return m_pBody;}
	b2Vec2 		getPosition() const {return m_pBody->GetPosition();}
	float 		getAngle() const {return m_pBody->GetAngle();}
	float 		getMass() const {return m_pBody->GetMass();}
	bool		isAwake() const {return m_pBody->IsAwake();}

	//setters
	void 		setActive(bool flag){m_pBody->SetActive(flag);}
	void 		setPosition(float pX, float pY){m_pBody->SetTransform(b2Vec2(pX, pY), m_pBody->GetAngle());}
	void 		setRotation(float angle){m_pBody->SetTransform(m_pBody->GetPosition(), angle);} //the angle is in randian
	void 		setMass(float mass, float inertia);

	//tools for physics
	void 		resetForces();

private:
	b2Body*			m_pBody;
	//TODO : make it more general (all hitBox are not like a polygon)
	b2PolygonShape*	m_pShape;
	b2Vec2			m_dimensions;
};

}

#endif //_H_SYMPTOGEN_PHYSIC_PHYSICAL_ENTITY_H_