#include "PhysicalEntity.h"
#include "../render/Render.h"

namespace Symp{

PhysicalEntity::PhysicalEntity(b2World* world, b2Vec2 origin, b2Vec2 hitBoxDimensions) {
	//create body
	b2BodyDef bodyDef;
	bodyDef.position = origin;
	m_pBody = world->CreateBody(&bodyDef);

	//set hitbox
	m_pShape = new b2PolygonShape();
	m_pShape->SetAsBox(hitBoxDimensions.x/2, hitBoxDimensions.y/2);

	m_hitboxWidth = hitBoxDimensions.x;
	m_hitboxHeight = hitBoxDimensions.y;

	//create fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = m_pShape;
	fixtureDef.restitution = 0.01f;// to make objects bounce [0, 1]
	fixtureDef.density = 0.f;// used to compute the mass properties of the parent body
	fixtureDef.friction = 0.4f;//used to make objects slide along each other realistically
	fixtureDef.isSensor = false;//a sensor shape collects contact information but never generates a collision response
	m_pBody->CreateFixture(&fixtureDef);
}

PhysicalEntity::~PhysicalEntity() {
	//when the b2World is deleted, all the memory reserved for bodies, fixtures, and joints is freed.
	//This is done to improve performance and make our life easier !
}

void PhysicalEntity::setMass(float mass, float inertia) {
	b2MassData massData;
	massData.mass = mass;
	massData.I = inertia;

	m_pBody->SetMassData(&massData);

	if(mass == 0)
		m_pBody->SetType(b2_staticBody);
	else
		m_pBody->SetType(b2_dynamicBody);
}

void PhysicalEntity::resetVelocities() {
	setLinearVelocity(b2Vec2(0,0));
	setAngularVelocity(0);
}

}