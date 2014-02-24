#include "PhysicalEntity.h"
#include "../render/Render.h"

namespace Symp{

PhysicalEntity::PhysicalEntity(b2World* world, b2Vec2 origin, b2Vec2 hitBoxDimensions, PhysicalType physicalType) {
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
	//To make objects bounce [0, 1]
	fixtureDef.restitution = 0.01f;
	//Used to compute the mass properties of the parent body. 
	//You should generally use similar densities for all your fixtures. 
	//This will improve stacking stability. 
	fixtureDef.density = 0.f;
	//Used to make objects slide along each other realistically.
	fixtureDef.friction = 0.4f;
	//A sensor shape collects contact information but never generates a collision response/
	fixtureDef.isSensor = false;
	m_pBody->CreateFixture(&fixtureDef);

	m_bContacting = false;
	m_type = physicalType;

	// The physical entity is stored in the b2Body's user data. 
	// This tool of box2D was created to store the application specific data.
	m_pBody->SetUserData(this);
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