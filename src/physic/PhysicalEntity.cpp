#include "PhysicalEntity.h"
#include "../render/Render.h"

namespace Symp{

std::vector<PhysicalEntity*>				PhysicalEntity::m_movableObjectArray = 	std::vector<PhysicalEntity*>();
ParserCollision* 							PhysicalEntity::m_pParserCollision = 	new ParserCollision();
std::map<std::string, std::vector<b2Vec2>> 	PhysicalEntity::s_verticesMap = std::map<std::string, std::vector<b2Vec2>>();

PhysicalEntity::PhysicalEntity(b2World* world, const b2Vec2 origin, const b2Vec2 hitBoxDimensions, const PhysicalType physicalType) {
	m_bHasToBeDestroyed = false;

	m_type = physicalType;

	m_iNumContacts = 0;
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
	m_fHitboxWidth = hitBoxDimensions.x;
	m_fHitboxHeight = hitBoxDimensions.y;
	setDefaultHitbox(hitBoxDimensions);

	attachedFixture();

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

void PhysicalEntity::setCustomChainHitbox(const char* collisionFileName){
	// Delete the fixture attached to the PhysicalEntity
	detachedFixture();

	// Create a new shape
	std::string sCollisionFileName(collisionFileName);
	std::vector<b2Vec2> vertexArray;
	size_t vertexCount;
	b2ChainShape chain;
	//if the element doesn't exist in the map of vertices
	if(s_verticesMap.count(sCollisionFileName) == 0){
		vertexArray = m_pParserCollision->loadCollision(collisionFileName, b2Vec2(m_fHitboxWidth, m_fHitboxHeight));
		vertexCount = vertexArray.size();

		s_verticesMap.insert(std::pair<std::string, std::vector<b2Vec2>>(sCollisionFileName, vertexArray));
	}
	//if the element exists in the map of vertices
	else{
		vertexArray = (*(s_verticesMap.find(sCollisionFileName))).second;
		vertexCount = vertexArray.size();
	}
	
	chain.CreateLoop(&vertexArray[0], vertexCount);
	m_pShape = chain.Clone(new b2BlockAllocator()); //memory leak ?

	// Create a new fixture attached to the PhysicalEntity
	attachedFixture();
}

void PhysicalEntity::setCustomPolygonHitbox(const char* collisionFileName){
	// Delete the fixture attached to the PhysicalEntity
	detachedFixture();

	// Create a new shape
	std::string sCollisionFileName(collisionFileName);
	std::vector<b2Vec2> vertexArray;
	size_t vertexCount;
	b2PolygonShape polygon;
	//if the element doesn't exist in the map of vertices
	if(s_verticesMap.count(sCollisionFileName) == 0){
		vertexArray = m_pParserCollision->loadCollision(collisionFileName, b2Vec2(m_fHitboxWidth, m_fHitboxHeight));
		vertexCount = vertexArray.size();

		s_verticesMap.insert(std::pair<std::string, std::vector<b2Vec2>>(sCollisionFileName, vertexArray));
	}
	//if the element exists in the map of vertices
	else{
		vertexArray = (*(s_verticesMap.find(sCollisionFileName))).second;
		vertexCount = vertexArray.size();
	}
	
	polygon.Set(&vertexArray[0], vertexCount);
	m_pShape = polygon.Clone(new b2BlockAllocator()); //memory leak ?

	// Create a new fixture attached to the PhysicalEntity
	attachedFixture();
}

void PhysicalEntity::setMass(float mass, float inertia) {
	b2MassData massData;
	//The mass in generally in kg.
	massData.mass = mass;
	//The rotational inertia of the shape about the local origin.
	massData.I = inertia;

	if(mass == 0)
		m_pBody->SetType(b2_kinematicBody);
	else
		m_pBody->SetType(b2_dynamicBody);

	m_pBody->SetMassData(&massData);
}

void PhysicalEntity::resetVelocities() {
	setLinearVelocity(b2Vec2(0,0));
	setAngularVelocity(0);
}

void PhysicalEntity::checkMovableObject(bool sneezeActivate){
	for(std::vector<PhysicalEntity*>::iterator it = m_movableObjectArray.begin(); it != m_movableObjectArray.end(); ++it){		
		// The MovableObject are dynamic
		if(sneezeActivate || ((*it)->getLinearVelocity().y > 1.f)){
			if((*it)->getb2Body()->GetType() != b2_dynamicBody)
				(*it)->getb2Body()->SetType(b2_dynamicBody);
		}
		// The MovableObject are static
		else{
			if((*it)->getb2Body()->GetType() != b2_staticBody)
				(*it)->getb2Body()->SetType(b2_staticBody);
		}
	}
}

// Private function
void PhysicalEntity::attachedFixture(){
	b2FixtureDef fixtureDef;
	fixtureDef.shape = m_pShape;
	// To make objects bounce [0, 1]
	fixtureDef.restitution = 0.01f;
	// Used to compute the mass properties of the parent body. 
	// You should generally use similar densities for all your fixtures. 
	// This will improve stacking stability. 
	fixtureDef.density = 0.f;
	// Used to make objects slide along each other realistically.
	fixtureDef.friction = 0.2f;
	// A sensor shape collects contact information but never generates a collision response.
	fixtureDef.isSensor = false;
	
	// Update specific elements depend on the type of PhysicalEntity.
	switch(m_type){
		case Flower:
		case Flames:
		case HotZone:
		case ColdZone:
			// The hitbox doesn't affect the movement of other physical entities.
			fixtureDef.isSensor = true;
			break;
		case MovableObject:
			m_movableObjectArray.push_back(this);
			break;
		case Dino:
			// The radius creates a skin around the shape. 
			m_pShape->m_radius = 2.f;
			break;
		default:
			break;
	}

	m_pFixture = m_pBody->CreateFixture(&fixtureDef);
}

// Private function
void PhysicalEntity::detachedFixture(){
	m_pBody->DestroyFixture(m_pFixture);
}


}