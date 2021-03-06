#include "ContactListener.h"
#include "../EntityManager.h"
#include "../power/Sneeze.h"
#include "../power/Fever.h"

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
	
	if(pPhysicalEntityA && pPhysicalEntityB) {

		setContactSides(pPhysicalEntityB, pPhysicalEntityA);

		/*************************/
		/*     Flower Contact    */
		/*************************/

		if(EntityManager::getInstance()->isPowerExisting(PowerType::SneezeType)) {
			bool dinoAndFlower = false;
			size_t flowerIndex = 0;

			if(isDino(pPhysicalEntityA) && isFlower(pPhysicalEntityB)) {			
				flowerIndex = getIndexEntity(pPhysicalEntityB);
				dinoAndFlower = true;
			}
			else if(isFlower(pPhysicalEntityA) && isDino(pPhysicalEntityB)) {
				flowerIndex = getIndexEntity(pPhysicalEntityA);
				dinoAndFlower = true;
			}

			//The sneeze has no effect if the fever is activated.
			if(EntityManager::getInstance()->isPowerExisting(PowerType::FeverType) 
				&& EntityManager::getInstance()->getPower(PowerType::FeverType)->isActivated())
				dinoAndFlower = false;

			if(dinoAndFlower) {
				EntityManager::getInstance()->setFlowerRender(flowerIndex, FlowerAction::CollideDino);
				EntityManager::getInstance()->getPower(PowerType::SneezeType)->forceExecution();
			}
		}
		
		/***********************************/
		/*     Fever relatives contacts    */
		/***********************************/
		
		if(EntityManager::getInstance()->isPowerExisting(PowerType::FeverType)) {
			
			/*****************/
			/*    Flames     */
			/*****************/

			if(isFlames(pPhysicalEntityA) && isDestructibleObject(pPhysicalEntityB)) {
				if(isDestructibleObject(pPhysicalEntityB)){
					size_t indexDestructableObject = getIndexEntity(pPhysicalEntityB);
					if(EntityManager::getInstance()->getRenderEntity(indexDestructableObject).at(DestructibleObjectAction::NormalBox)->isShow())
						EntityManager::getInstance()->setDestructibleObjectRender(indexDestructableObject, DestructibleObjectAction::ByFlames);
				}
			}

			else if(isFlames(pPhysicalEntityB) && isDestructibleObject(pPhysicalEntityA)) {
				if(isDestructibleObject(pPhysicalEntityA)){
					size_t indexDestructableObject = getIndexEntity(pPhysicalEntityA);
					if(EntityManager::getInstance()->getRenderEntity(indexDestructableObject).at(DestructibleObjectAction::NormalBox)->isShow())
						EntityManager::getInstance()->setDestructibleObjectRender(indexDestructableObject, DestructibleObjectAction::ByFlames);
				}
			}
			
			/*******************/
			/*    Hot zone     */
			/*******************/
			
			if( (isDino(pPhysicalEntityA) && isHotZone(pPhysicalEntityB)) || (isDino(pPhysicalEntityB) && isHotZone(pPhysicalEntityA))) {
				Fever* pFever = dynamic_cast<Fever*>(EntityManager::getInstance()->getPower(PowerType::FeverType));
				pFever->isInHotZone(true);
			}
			
			/*******************/
			/*    Cold zone    */
			/*******************/

			else if( (isDino(pPhysicalEntityA) && isColdZone(pPhysicalEntityB)) || (isDino(pPhysicalEntityB) && isColdZone(pPhysicalEntityA))) {
				Fever* pFever = dynamic_cast<Fever*>(EntityManager::getInstance()->getPower(PowerType::FeverType));
				pFever->isInColdZone(true);
			}
		}

		/*****************/
		/*     Spikes    */
		/*****************/
		
		if((isDino(pPhysicalEntityA) && isSpikes(pPhysicalEntityB)) || (isSpikes(pPhysicalEntityA) && isDino(pPhysicalEntityB))) {
			EntityManager::getInstance()->killDino();
		}
	}
}

void ContactListener::EndContact(b2Contact* contact) {
	// Check fixture A
	PhysicalEntity* pPhysicalEntityA;
	void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	if(userDataA){
		pPhysicalEntityA = static_cast<PhysicalEntity*>(userDataA);
		pPhysicalEntityA->endContact();
	}

	// Check fixture B
	PhysicalEntity* pPhysicalEntityB;
	void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	if(userDataB){
		pPhysicalEntityB = static_cast<PhysicalEntity*>(userDataB);
		pPhysicalEntityB->endContact();
	}

	if(pPhysicalEntityA && pPhysicalEntityB) {
		setContactSides(pPhysicalEntityB, pPhysicalEntityA);

		/*************************/
		/*     Flower Contact    */
		/*************************/
		if(EntityManager::getInstance()->isPowerExisting(PowerType::SneezeType)){
			bool dinoAndFlower = false;
			size_t flowerIndex = 0;
			
			if(isDino(pPhysicalEntityA) && isFlower(pPhysicalEntityB)) {			
				flowerIndex = getIndexEntity(pPhysicalEntityB);
				dinoAndFlower = true;
			}
			else if(isFlower(pPhysicalEntityA) && isDino(pPhysicalEntityB)) {
				flowerIndex = getIndexEntity(pPhysicalEntityA);
				dinoAndFlower = true;
			}

			//The sneeze has no effect if the fever is activated.
			if(EntityManager::getInstance()->isPowerExisting(PowerType::FeverType) && EntityManager::getInstance()->getPower(PowerType::FeverType)->isActivated())
				dinoAndFlower = false;

			if(dinoAndFlower) {
				EntityManager::getInstance()->setFlowerRender(flowerIndex, FlowerAction::Normal);
				EntityManager::getInstance()->getPower(PowerType::SneezeType)->forceExecution();
			}
		}
		
		/***********************************/
		/*     Fever relatives contacts    */
		/***********************************/

		if(EntityManager::getInstance()->isPowerExisting(PowerType::FeverType)) {
			
			/*******************/
			/*    Hot zone     */
			/*******************/
			
			if( (isDino(pPhysicalEntityA) && isHotZone(pPhysicalEntityB)) || (isDino(pPhysicalEntityB) && isHotZone(pPhysicalEntityA))) {
				Fever* pFever = dynamic_cast<Fever*>(EntityManager::getInstance()->getPower(PowerType::FeverType));
				pFever->isInHotZone(false);
			}
			
			/*******************/
			/*    Cold zone    */
			/*******************/
			
			else if( (isDino(pPhysicalEntityA) && isColdZone(pPhysicalEntityB)) || (isDino(pPhysicalEntityB) && isColdZone(pPhysicalEntityA))) {
				Fever* pFever = dynamic_cast<Fever*>(EntityManager::getInstance()->getPower(PowerType::FeverType));
				pFever->isInColdZone(false);
			}
		}
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

void ContactListener::setContactSides(PhysicalEntity* A, PhysicalEntity* B) {
	// Warning : use the skin of the PhysicalEntity only for below and above position.
	int distanceBelow = (A->getPosition().y+A->getSkin()+A->getHeight()*0.5)-(B->getPosition().y-B->getSkin()-B->getHeight()*0.5);
	int distanceAbove = (A->getPosition().y-A->getSkin()-A->getHeight()*0.5)-(B->getPosition().y+B->getSkin()+B->getHeight()*0.5);
	int distanceRight = (A->getPosition().x+A->getWidth()*0.5)-(B->getPosition().x-B->getWidth()*0.5);
	int distanceLeft = (A->getPosition().x-A->getWidth()*0.5)-(B->getPosition().x+B->getWidth()*0.5);

	if((distanceLeft == 0 || distanceLeft == 1)){//Contact from left
		A->hasContactLeft(true);
		B->hasContactRight(true);
	}
	else{
		A->hasContactLeft(false);
		B->hasContactRight(false);
	}
	
	if((distanceRight == 0 || distanceRight == 1)){//Contact from right
		A->hasContactRight(true);
		B->hasContactLeft(true);
	}
	else{
		A->hasContactRight(false);
		B->hasContactLeft(false);
	}

	if(distanceBelow == 0 || distanceBelow == 1){//Contact from below
		A->hasContactBelow(true);
		B->hasContactAbove(true); 
	}
	else{
		A->hasContactBelow(false);
		B->hasContactAbove(false);
	}

	if(distanceAbove == 0 || distanceAbove == 1){//Contact from above
		A->hasContactAbove(true); 
		B->hasContactBelow(true);
	}
	else{
		A->hasContactAbove(false);
		B->hasContactBelow(false);
	}
}


}