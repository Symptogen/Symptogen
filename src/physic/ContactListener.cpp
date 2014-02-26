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

	// size_t indexEntityA = getIndexEntity(pPhysicalEntityA);
	// size_t indexEntityB = getIndexEntity(pPhysicalEntityB);

	//BeginContact between dino and a flower
	if((isDino(pPhysicalEntityA) && isFlower(pPhysicalEntityB)) || (isFlower(pPhysicalEntityA) && isDino(pPhysicalEntityB))) {
		dynamic_cast<Sneeze*>(EntityManager::getInstance()->getPower(PowerType::SneezeType))->forceExecution();
	}

	//BeginContact between dino and spikes
	if((isDino(pPhysicalEntityA) && isSpikes(pPhysicalEntityB)) || (isSpikes(pPhysicalEntityA) && isDino(pPhysicalEntityB))) {
		EntityManager::getInstance()->updateDinoRender(DinoAction::Die);
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

	//EndContact between dino and spikes
	if((isDino(pPhysicalEntityA) && isSpikes(pPhysicalEntityB)) || (isSpikes(pPhysicalEntityA) && isDino(pPhysicalEntityB))) {
		EntityManager::getInstance()->updateDinoRender(DinoAction::Stop);
	}
}

size_t ContactListener::getIndexEntity(PhysicalEntity* pPhysicalEntity) const {
	std::vector<PhysicalEntity*>::iterator itEntity = std::find (
		EntityManager::getInstance()->getPhysicalEntityArray().begin(), 
		EntityManager::getInstance()->getPhysicalEntityArray().end(), 
		pPhysicalEntity);
	return std::distance(EntityManager::getInstance()->getPhysicalEntityArray().begin(), itEntity);
}

}