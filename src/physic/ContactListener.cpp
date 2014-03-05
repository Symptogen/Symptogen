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
		pPhysicalEntityA->startContact();
	}

	// Check fixture B
	PhysicalEntity* pPhysicalEntityB;
	void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	if(userDataB){
		pPhysicalEntityB = static_cast<PhysicalEntity*>(userDataB);
		pPhysicalEntityB->startContact();
	}
	
	if(pPhysicalEntityA && pPhysicalEntityB){
		
		setContactSides(pPhysicalEntityB, pPhysicalEntityA);

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

			// BeginContact between dino and spikes
			if((isDino(pPhysicalEntityA) && isSpikes(pPhysicalEntityB)) || (isSpikes(pPhysicalEntityA)))
				EntityManager::getInstance()->killDino();
		}
	}
}

void ContactListener::EndContact(b2Contact* contact) {
	//check fixture A
	PhysicalEntity* pPhysicalEntityA;
	void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	if(userDataA){
		pPhysicalEntityA = static_cast<PhysicalEntity*>(userDataA);
		pPhysicalEntityA->endContact();
	}

	//check fixture B
	PhysicalEntity* pPhysicalEntityB;
	void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	if(userDataB){
		pPhysicalEntityB = static_cast<PhysicalEntity*>(userDataB);
		pPhysicalEntityB->endContact();
	}

	if(pPhysicalEntityA && pPhysicalEntityB) {
		setContactSides(pPhysicalEntityB, pPhysicalEntityA);

		//EndContact between dino and spikes
		if((isDino(pPhysicalEntityA) && isSpikes(pPhysicalEntityB)) || (isSpikes(pPhysicalEntityA) && isDino(pPhysicalEntityB)))
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

void ContactListener::setContactSides(PhysicalEntity* A, PhysicalEntity* B){
	int distanceBelow = (A->getPosition().y+A->getHeight()*0.5)-(B->getPosition().y-B->getHeight()*0.5);
	int distanceAbove = (A->getPosition().y-A->getHeight()*0.5)-(B->getPosition().y+B->getHeight()*0.5);
	int distanceRight = (A->getPosition().x+A->getWidth()*0.5)-(B->getPosition().x-B->getWidth()*0.5);
	int distanceLeft = (A->getPosition().x-A->getWidth()*0.5)-(B->getPosition().x+B->getWidth()*0.5);

	if((distanceLeft == 0 || distanceLeft == 1) && distanceBelow < 0)//Contact from left
		A->hasContactLeft(true);
	else
		A->hasContactLeft(false);
	
	if((distanceRight == 0 || distanceRight == 1) && distanceBelow < 0)//Contact from right
		A->hasContactRight(true);
	else
		A->hasContactRight(false);

	if(distanceBelow == 0 || distanceBelow == 1)//Contact from below
		A->hasContactBelow(true);
	else
		A->hasContactBelow(false);

	if(distanceAbove == 0 || distanceAbove == 1)//Contact from above
		A->hasContactAbove(true); 
	else 
		A->hasContactAbove(false);

	/*if(isDino(A)){
		std::cout<<"gauche "<<A->isContactingLeft()<<std::endl;
		std::cout<<"droite "<<A->isContactingRight()<<std::endl;
		std::cout<<"enHaut "<<A->isContactingAbove()<<std::endl;
		std::cout<<".enBas "<<A->isContactingBelow()<<std::endl;
	}*/
}


}