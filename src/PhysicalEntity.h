#ifndef _H_SYMPTOGEN_PHYSICAL_PHYSICAL_ENTITY_H_
#define _H_SYMPTOGEN_PHYSICAL_PHYSICAL_ENTITY_H_

#include <iostream>

#include <Box2D/Box2D.h>

/**
	Facade of Box2D entity.
*/
class PhysicalEntity {
public:
	PhysicalEntity(b2World* world, float posX, float posY);
	~PhysicalEntity();

	void setHitBox(float hx, float hy, float density, float friction);

	//for tests
	void display();

private:
	b2BodyDef*		m_pBodyDef;
	b2Body*			m_pBody;
	b2PolygonShape*	m_pShape; //other shapes exist in box2d...
	b2FixtureDef*	m_pFixtureDef;
};

#endif //_H_SYMPTOGEN_PHYSICAL_PHYSICAL_ENTITY_H_