#ifndef _H_SYMPTOGEN_PHYSICAL_PHYSICAL_ENTITY_H_
#define _H_SYMPTOGEN_PHYSICAL_PHYSICAL_ENTITY_H_

#include <Box2D/Box2D.h>

namespace Symp{

/**
	Facade of Box2D entity.
*/
class PhysicalEntity {
public:
	PhysicalEntity(b2World* world, b2Vec2 dimensions);
	~PhysicalEntity();

	//getters
	b2Vec2 getPosition(){return m_pBody->GetPosition();}
	float getAngle(){return m_pBody->GetAngle();}

	//setters
	void setActive(bool flag){m_pBody->SetActive(flag);}
	void setPosition(float pX, float pY){m_pBody->SetTransform(b2Vec2(pX, pY), m_pBody->GetAngle());}
	void setRotation(float angle){m_pBody->SetTransform(m_pBody->GetPosition(), angle);} //the angle is in randian
	void setMass(float mass, float inertia);

	//for tests
	void display();

private:
	b2Body*			m_pBody;
	//TODO : make it more general (all hitBox are not like a polygon)
	b2PolygonShape*	m_pShape;
	b2Vec2			m_dimensions;
};

}

#endif //_H_SYMPTOGEN_PHYSICAL_PHYSICAL_ENTITY_H_