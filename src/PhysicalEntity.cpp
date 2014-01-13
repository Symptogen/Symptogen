#include "PhysicalEntity.h"

namespace Symp{

PhysicalEntity::PhysicalEntity(b2World* world, float posX, float posY){
	m_bodyDef = b2BodyDef();
	m_bodyDef.type = b2_dynamicBody;//in parameters !
	m_bodyDef.position.Set(posX, posY);
	m_pBody = world->CreateBody(&m_bodyDef);
}

PhysicalEntity::~PhysicalEntity(){
	//when the b2World is deleted, all the memory reserved for bodies, fixtures, and joints is freed.
	//This is done to improve performance and make our life easier !
}

void PhysicalEntity::setHitBox(float hx, float hy, float density, float friction){
	m_pShape = new b2PolygonShape();
	m_pShape->SetAsBox(hx, hy);

	m_fixtureDef = b2FixtureDef();
	m_fixtureDef.shape = m_pShape;
	m_fixtureDef.density = density; //density = 0 => static body
	m_fixtureDef.friction = friction;
	m_pBody->CreateFixture(&m_fixtureDef);
}

void PhysicalEntity::display(){
	m_pBody->Dump();
}

}