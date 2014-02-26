#include "Sneeze.h"

#include "../EntityManager.h"
#include "../physic/PhysicalEntity.h"

namespace Symp {

	void Sneeze::execute() {
		if(time(NULL) - m_uiLastExecution >= m_uiTimeToTriggerRandomSneeze) {
			// With these values, the dino has sneeze 10 times in 2 minutes
			float random = rand() % 10000; // between 0 and 1
			float treshold = 10000 - (time(NULL) - m_uiLastExecution);
			if(random > treshold) {
				forceExecution();
			}
		}
	}

	void Sneeze::forceExecution() {
		PhysicalEntity* pDino = EntityManager::getInstance()->getPhysicalDino();
		float impulse = pDino->getMass() * m_uiRepulsionStrength;
		if(pDino->getLinearVelocity().x > 0) {
			impulse = -impulse;
		}
		pDino->getb2Body()->ApplyLinearImpulse(b2Vec2(impulse, 0.f), pDino->getb2Body()->GetWorldCenter(), pDino->isAwake());
		m_uiLastExecution = time(NULL);
	}

}