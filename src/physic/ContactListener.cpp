#include "ContactListener.h"
#include "../EntityManager.h"
#include "../power/Sneeze.h"

namespace Symp {
	
void ContactListener::BeginContact(b2Contact* contact) {
	// Check fixture A
	PhysicalEntity* pPhysicalEntityA;
	void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	if(userDataA){
		pPhysicalEntityA = static_cast<PhysicalEntity*>(userDataA);
		if(pPhysicalEntityA){
			pPhysicalEntityA->startContact();
		}
	}

	// Check fixture B
	PhysicalEntity* pPhysicalEntityB;
	void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	if(userDataB){
		pPhysicalEntityB = static_cast<PhysicalEntity*>(userDataB);
		if(pPhysicalEntityB){
			pPhysicalEntityB->startContact();
		}
	}
	
	// Manage contacts of dino
	if(pPhysicalEntityB->getType() == PhysicalType::Dino){
		setContactSides(pPhysicalEntityB,pPhysicalEntityA);
	}
	else if(pPhysicalEntityA->getType() == PhysicalType::Dino){
		setContactSides(pPhysicalEntityA,pPhysicalEntityB);
	}

	// BeginContact between dino and a flower
	if(EntityManager::getInstance()->isPowerExisting(PowerType::SneezeType)){
		bool dinoAndFlower = false;
		size_t flowerIndex = 0;
		if(isDino(pPhysicalEntityA) 
			&& isFlower(pPhysicalEntityB)) {			
			// Get the index corresponding to the entity
			flowerIndex = getIndexEntity(pPhysicalEntityB);
			dinoAndFlower = true;
		}


		else if(isFlower(pPhysicalEntityA) 
				&& isDino(pPhysicalEntityB)) {
			// Get the index corresponding to the entity
			flowerIndex = getIndexEntity(pPhysicalEntityA);
			dinoAndFlower = true;
			
		}
		if(dinoAndFlower) {

			// Show animation
			EntityManager::getInstance()->getRenderEntity(flowerIndex)[FlowerAction::Normal]->setShow(false);
			EntityManager::getInstance()->getRenderEntity(flowerIndex)[FlowerAction::CollideDino]->setShow(true);

			// Launch sneeze
			dynamic_cast<Sneeze*>(EntityManager::getInstance()->getPower(PowerType::SneezeType))->forceExecution();
		}
	}

	// BeginContact between dino and spikes
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
	
	int count = 0;
	for(std::vector<PhysicalEntity*>::iterator it = EntityManager::getInstance()->getPhysicalEntityArray().begin(); it != EntityManager::getInstance()->getPhysicalEntityArray().end(); ++it) {
		if(*it == pPhysicalEntity) {
			return count;
		}
		count++;
	}


	return 0;
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