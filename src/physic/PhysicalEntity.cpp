#include "PhysicalEntity.h"

namespace Symp{

PhysicalEntity::PhysicalEntity(b2World* world, b2Vec2 dimensions){
	m_dimensions = dimensions;

	//create body
	b2BodyDef bodyDef;
	m_pBody = world->CreateBody(&bodyDef);

	//set hitbox
	m_pShape = new b2PolygonShape();
	m_pShape->SetAsBox(dimensions.x/2, dimensions.y/2, b2Vec2(dimensions.x/2, dimensions.y/2), 0);

	//create fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = m_pShape;
	fixtureDef.restitution = 0.1f;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.4f;
	m_pBody->CreateFixture(&fixtureDef);
}

PhysicalEntity::~PhysicalEntity(){
	//when the b2World is deleted, all the memory reserved for bodies, fixtures, and joints is freed.
	//This is done to improve performance and make our life easier !
}

void PhysicalEntity::setMass(float mass, float inertia){
	b2MassData massData;
	massData.mass = mass;
	massData.I = inertia;

	m_pBody->SetMassData(&massData);

	if(mass == 0)
		m_pBody->SetType(b2_staticBody);
	else
		m_pBody->SetType(b2_dynamicBody);
}

void PhysicalEntity::resetVelocities(){
	setLinearVelocity(b2Vec2(0,0));
	setAngularVelocity(0);
}

}