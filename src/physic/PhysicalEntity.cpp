#include "PhysicalEntity.h"
#include "../render/Render.h"

namespace Symp{

std::vector<PhysicalEntity*> PhysicalEntity::m_movableObjectArray = std::vector<PhysicalEntity*>();

PhysicalEntity::PhysicalEntity(b2World* world, const b2Vec2 origin, const b2Vec2 hitBoxDimensions, const PhysicalType physicalType) {
	m_bHasToBeDestroyed = false;

	m_iNumContacts = 0;
	m_type = physicalType;

	m_bHasContactBelow = false;
	m_bHasContactAbove = false;
	m_bHasContactRight = false;
	m_bHasContactLeft = false;

	/*********/
	/* body  */
	/*********/
	b2BodyDef bodyDef;
	bodyDef.position = origin;
	m_pBody = world->CreateBody(&bodyDef);

	/**********/
	/* hitbox */
	/**********/
	//TODO : call setCustomHitbox depend on the PhysicalType (for Dino, Flower...).
	if(physicalType == PhysicalType::Ground){
		setCustomGroundHitbox(hitBoxDimensions);
		m_fHitboxWidth = hitBoxDimensions.x;
		m_fHitboxHeight = hitBoxDimensions.y/1.5f;
	}
	else{
		setDefaultHitbox(hitBoxDimensions);
		m_fHitboxWidth = hitBoxDimensions.x;
		m_fHitboxHeight = hitBoxDimensions.y;
	}

	/************/
	/* fixture  */
	/************/ 
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
	//A sensor shape collects contact information but never generates a collision response.
	fixtureDef.isSensor = false;
	switch(physicalType){
		case Flower:
		case Flames:
		case HotZone:
		case ColdZone:
			//the hitbox doesn't affect the movement of other physical entities.
			fixtureDef.isSensor = true;
			break;
		case MovableObject:
			m_movableObjectArray.push_back(this);
			break;
		default:
			break;
	}
	m_pBody->CreateFixture(&fixtureDef);
	m_type = physicalType;

	// The physical entity is stored in the b2Body's user data. 
	// This tool of box2D was created to store the application specific data.
	m_pBody->SetUserData(this);
}

PhysicalEntity::~PhysicalEntity() {
	//when the b2World is deleted, all the memory reserved for bodies, fixtures, and joints is freed.
	//This is done to improve performance and make our life easier !
}

void PhysicalEntity::setDefaultHitbox(const b2Vec2 hitBoxDimensions){
	b2PolygonShape* polygon = new b2PolygonShape();
	polygon->SetAsBox(hitBoxDimensions.x/2, hitBoxDimensions.y/2);
 	m_pShape = polygon->Clone(new b2BlockAllocator()); //memory leak ?
 	delete polygon;
}

void PhysicalEntity::setCustomHitbox(const b2Vec2* vertexArray, size_t vertexCount){
	b2ChainShape* chain = new b2ChainShape();
	chain->CreateLoop(vertexArray, vertexCount);
	m_pShape = chain->Clone(new b2BlockAllocator()); //memory leak ?
	delete chain;
}

void PhysicalEntity::setCustomGroundHitbox(const b2Vec2 hitBoxDimensions){
	b2PolygonShape* polygon = new b2PolygonShape();
	polygon->SetAsBox(hitBoxDimensions.x/2, hitBoxDimensions.y/3);
 	m_pShape = polygon->Clone(new b2BlockAllocator()); //memory leak ?
 	delete polygon;
}

void PhysicalEntity::setMass(float mass, float inertia) {
	b2MassData massData;
	//The mass in generally in kg.
	massData.mass = mass;
	//The rotational inertia of the shape about the local origin.
	massData.I = inertia;

	if(mass == 0)
		m_pBody->SetType(b2_staticBody);
	else
		m_pBody->SetType(b2_dynamicBody);

	m_pBody->SetMassData(&massData);
}

void PhysicalEntity::resetVelocities() {
	setLinearVelocity(b2Vec2(0,0));
	setAngularVelocity(0);
}

void PhysicalEntity::setMovableObjectDynamic(){
	for(std::vector<PhysicalEntity*>::iterator it = m_movableObjectArray.begin(); it != m_movableObjectArray.end(); ++it){
		(*it)->getb2Body()->SetType(b2_dynamicBody);
	}
}

void PhysicalEntity::setMovableObjectStatic(){
	for(std::vector<PhysicalEntity*>::iterator it = m_movableObjectArray.begin(); it != m_movableObjectArray.end(); ++it){
		(*it)->getb2Body()->SetType(b2_staticBody);
	}
}

void PhysicalEntity::checkMovableObject(){
	for(std::vector<PhysicalEntity*>::iterator it = m_movableObjectArray.begin(); it != m_movableObjectArray.end(); ++it){
		if((*it)->hasContactingBelow())
			(*it)->getb2Body()->SetType(b2_staticBody);
		else
			(*it)->getb2Body()->SetType(b2_dynamicBody);
	}
}

}