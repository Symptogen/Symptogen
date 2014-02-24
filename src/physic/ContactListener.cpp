#include "ContactListener.h"
#include "../EntityManager.h"

namespace Symp {
	
void ContactListener::BeginContact(b2Contact* contact) {
	//check fixture A
	void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	if(userDataA){
		PhysicalEntity* pPhysicalEntity = static_cast<PhysicalEntity*>(userDataA);
		if(pPhysicalEntity){
			pPhysicalEntity->startContact();
		}
	}

	//check fixture B
	void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	if(userDataB){
		PhysicalEntity* pPhysicalEntity = static_cast<PhysicalEntity*>(userDataB);
		if(pPhysicalEntity){
			pPhysicalEntity->startContact();
		}
	}
}

void ContactListener::EndContact(b2Contact* contact) {
	//check fixture A
	void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	if(userDataA){
		PhysicalEntity* pPhysicalEntity = static_cast<PhysicalEntity*>(userDataA);
		if(pPhysicalEntity){
			pPhysicalEntity->endContact();
		}
	}

	//check fixture B
	void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	if(userDataB){
		PhysicalEntity* pPhysicalEntity = static_cast<PhysicalEntity*>(userDataB);
		if(pPhysicalEntity){
			pPhysicalEntity->endContact();
		}
	}
}

}