#ifndef _H_SYMPTOGEN_PHYSICAL_PHYSICAL_ENTITY_H_
#define _H_SYMPTOGEN_PHYSICAL_PHYSICAL_ENTITY_H_

#include <Box2D/Box2D.h>

namespace Symp{

/**
	Facade of Box2D entity.
*/
class PhysicalEntity {
public:
	PhysicalEntity(b2World* world, float posX, float posY);
	~PhysicalEntity();

	//getters
	float getPosX(){return m_pBody->GetPosition().x;}
	float getPosY(){return m_pBody->GetPosition().y;};

	//setters
	void setActive(bool flag){m_pBody->SetActive(flag);}
	//TODO : make it more general (all hitBox are not like a polygon)
	void setHitBox(float hx, float hy, float density, float friction);

	//for tests
	void display();

private:
	b2BodyDef		m_bodyDef;
	b2Body*			m_pBody;
	//TODO : make it more general (all hitBox are not like a polygon)
	b2PolygonShape*	m_pShape;
	b2FixtureDef	m_fixtureDef;
};

}

#endif //_H_SYMPTOGEN_PHYSICAL_PHYSICAL_ENTITY_H_