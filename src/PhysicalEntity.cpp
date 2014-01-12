#include "PhysicalEntity.h"

PhysicalEntity::PhysicalEntity(b2World* world, float posX, float posY){
	m_pBodyDef = new b2BodyDef();
	m_pBodyDef->type = b2_dynamicBody;
	m_pBodyDef->position.Set(posX, posY);
	m_pBody = world->CreateBody(m_pBodyDef);
}

PhysicalEntity::~PhysicalEntity(){

}

void PhysicalEntity::setHitBox(float hx, float hy, float density, float friction){
	m_pShape = new b2PolygonShape();
	m_pShape->SetAsBox(hx, hy);

	m_pFixtureDef = new b2FixtureDef();
	m_pFixtureDef->shape = m_pShape;
	m_pFixtureDef->density = density; //density = 0 => static body
	m_pFixtureDef->friction = friction;
	m_pBody->CreateFixture(m_pFixtureDef);
}

void PhysicalEntity::display(){
	m_pBody->Dump();
}