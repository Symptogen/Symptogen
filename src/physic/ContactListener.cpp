#include "ContactListener.h"
#include "../EntityManager.h"
#include "../power/Sneeze.h"

namespace Symp {
	
void ContactListener::BeginContact(b2Contact* contact) {
	//check fixture A
	PhysicalEntity* pPhysicalEntityA;
	void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	if(userDataA){
		pPhysicalEntityA = static_cast<PhysicalEntity*>(userDataA);
		if(pPhysicalEntityA){
			pPhysicalEntityA->startContact();
		}
	}

	//check fixture B
	PhysicalEntity* pPhysicalEntityB;
	void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	if(userDataB){
		pPhysicalEntityB = static_cast<PhysicalEntity*>(userDataB);
		if(pPhysicalEntityB){
			pPhysicalEntityB->startContact();
		}
	}
	
	//manage contacts of dino
	if(pPhysicalEntityB->getType() == PhysicalType::Dino){
		setContactSides(pPhysicalEntityB,pPhysicalEntityA);
	}
	else if(pPhysicalEntityA->getType() == PhysicalType::Dino){
		setContactSides(pPhysicalEntityA,pPhysicalEntityB);
	}

	//BeginContact between dino and a flower
	if(EntityManager::getInstance()->isPowerExisting(PowerType::SneezeType)){
		if((isDino(pPhysicalEntityA) && isFlower(pPhysicalEntityB)) || (isFlower(pPhysicalEntityA) && isDino(pPhysicalEntityB))) {
			dynamic_cast<Sneeze*>(EntityManager::getInstance()->getPower(PowerType::SneezeType))->forceExecution();
			EntityManager::getInstance()->setDinoRender(DinoAction::Sneezing);
		}
	}

	//BeginContact between dino and spikes
	if((isDino(pPhysicalEntityA) && isSpikes(pPhysicalEntityB)) || (isSpikes(pPhysicalEntityA))) {
		EntityManager::getInstance()->killDino();
	}
}

void ContactListener::EndContact(b2Contact* contact) {
	//check fixture A
	PhysicalEntity* pPhysicalEntityA;
	void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	if(userDataA){
		pPhysicalEntityA = static_cast<PhysicalEntity*>(userDataA);
		if(pPhysicalEntityA){
			pPhysicalEntityA->endContact();
		}
	}

	//check fixture B
	PhysicalEntity* pPhysicalEntityB;
	void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	if(userDataB){
		pPhysicalEntityB = static_cast<PhysicalEntity*>(userDataB);
		if(pPhysicalEntityB){
			pPhysicalEntityB->endContact();
		}
	}

	//manage contacts of dino
	if(pPhysicalEntityB->getType() == PhysicalType::Dino){
		setContactSides(pPhysicalEntityB,pPhysicalEntityA);
	}
	else if(pPhysicalEntityA->getType() == PhysicalType::Dino){
		setContactSides(pPhysicalEntityA,pPhysicalEntityB);
	}

	//EndContact between dino and spikes
	if((isDino(pPhysicalEntityA) && isSpikes(pPhysicalEntityB)) || (isSpikes(pPhysicalEntityA) && isDino(pPhysicalEntityB))) {
		EntityManager::getInstance()->setDinoRender(DinoAction::NormalStop);
	}
}

size_t ContactListener::getIndexEntity(PhysicalEntity* pPhysicalEntity) const {
	std::vector<PhysicalEntity*>::iterator itEntity = std::find (
		EntityManager::getInstance()->getPhysicalEntityArray().begin(), 
		EntityManager::getInstance()->getPhysicalEntityArray().end(), 
		pPhysicalEntity);
	return std::distance(EntityManager::getInstance()->getPhysicalEntityArray().begin(), itEntity);
}

void ContactListener::setContactSides(PhysicalEntity* dino, PhysicalEntity* obstacle){
	int distanceBelow = (dino->getPosition().y+dino->getHeight()*0.5)-(obstacle->getPosition().y-obstacle->getHeight()*0.5);
	int distanceAbove = (dino->getPosition().y-dino->getHeight()*0.5)-(obstacle->getPosition().y+obstacle->getHeight()*0.5);
	int distanceRight = (dino->getPosition().x+dino->getWidth()*0.5)-(obstacle->getPosition().x-obstacle->getWidth()*0.5);
	int distanceLeft = (dino->getPosition().x-dino->getWidth()*0.5)-(obstacle->getPosition().x+obstacle->getWidth()*0.5);

	if((distanceLeft == 0 || distanceLeft == 1) && distanceBelow > 0) dino->hasContactLeft(true); //Contact from left
	else dino->hasContactLeft(false);
	if((distanceRight == 0 || distanceRight == 1) && distanceBelow > 0) dino->hasContactRight(true); //Contact from right
	else dino->hasContactRight(false);
	if(distanceBelow == 0 || distanceBelow == 1) dino->hasContactBelow(true); //Contact from below
	else dino->hasContactBelow(false);
	if(distanceAbove == 0 || distanceAbove == 1) dino->hasContactAbove(true); //Contact from above
	else dino->hasContactAbove(false);
}
}