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

	//Contact between dino and a flower
	if((pPhysicalEntityA->getType() == PhysicalType::Dino && pPhysicalEntityB->getType() == PhysicalType::Flower)
		|| (pPhysicalEntityA->getType() == PhysicalType::Flower && pPhysicalEntityB->getType() == PhysicalType::Dino)){
		dynamic_cast<Sneeze*>(EntityManager::getInstance()->getPower(PowerType::SneezeType))->forceExecution();
	}

	if(pPhysicalEntityB->getType() == PhysicalType::Dino){
		setContactSides(pPhysicalEntityB,pPhysicalEntityA);
	}
	else if(pPhysicalEntityA->getType() == PhysicalType::Dino){
		setContactSides(pPhysicalEntityA,pPhysicalEntityB);
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

	if(pPhysicalEntityB->getType() == PhysicalType::Dino){
		setContactSides(pPhysicalEntityB,pPhysicalEntityA);
	}
	else if(pPhysicalEntityA->getType() == PhysicalType::Dino){
		setContactSides(pPhysicalEntityA,pPhysicalEntityB);
	}
}

void ContactListener::setContactSides(PhysicalEntity* dino, PhysicalEntity* obstacle){
	int distanceBelow = (dino->getPosition().y+dino->getHeight()*0.5)-(obstacle->getPosition().y-obstacle->getHeight()*0.5);
	int distanceAbove = (dino->getPosition().y-dino->getHeight()*0.5)-(obstacle->getPosition().y+obstacle->getHeight()*0.5);
	int distanceRight = (dino->getPosition().x+dino->getWidth()*0.5)-(obstacle->getPosition().x-obstacle->getWidth()*0.5);
	int distanceLeft = (dino->getPosition().x-dino->getWidth()*0.5)-(obstacle->getPosition().x+obstacle->getWidth()*0.5);

	if(distanceLeft == 0 && distanceBelow > 0) dino->hasContactLeft(true); //Contact from left
	else dino->hasContactLeft(false);
	if(distanceRight == 0 && distanceBelow > 0) dino->hasContactRight(true); //Contact from right
	else dino->hasContactRight(false);
	if(distanceBelow == 0) dino->hasContactBelow(true); //Contact from below
	else dino->hasContactBelow(false);
	if(distanceAbove == 0) dino->hasContactAbove(true); //Contact from above
	else dino->hasContactAbove(false);
	/*std::cout<<"distance bas "<<distanceBelow<<" contact bas "<<dino->isContactingBelow()<<std::endl;
	std::cout<<"distance haut "<<distanceAbove<<" contact haut "<<dino->isContactingAbove()<<std::endl;
	std::cout<<"distance droite "<<distanceRight<<" contact droit "<<dino->isContactingRight()<<std::endl;
	std::cout<<"distance gauche "<<distanceLeft<<" contact gauche "<<dino->isContactingLeft()<<std::endl;

	std::cout<<"contact bas "<<dino->isContactingBelow()<<std::endl;
	std::cout<<"contact haut "<<dino->isContactingAbove()<<std::endl;
	std::cout<<"contact droit "<<dino->isContactingRight()<<std::endl;
	std::cout<<"contact gauche "<<dino->isContactingLeft()<<std::endl;*/

}

}