#include "Sneeze.h"
#include <time.h>

#include "../EntityManager.h"
#include "../physic/PhysicalEntity.h"

namespace Symp {

	void Sneeze::execute() {
		/*if(m_uiLastExecution >= m_uiTimeToTriggerRandomSneeze) {
			//srand(time(NULL));
			float random = rand() % 100; // between 0 and 1
			float treshold = 100 - m_uiLastExecution/100;
			fprintf(stderr, "\rtime spent = %f\n", m_uiLastExecution);
			if(random > treshold) {
				forceExecution();
			}
		}
		fprintf(stderr, "lastExec = %d\n", m_uiLastExecution);*/
	}

	void Sneeze::forceExecution() {
		PhysicalEntity* pDino = EntityManager::getInstance()->getPhysicalDino();
		float impulse = pDino->getMass() * m_uiRepulsionStrength;
		pDino->getb2Body()->ApplyLinearImpulse(b2Vec2(impulse, 0.f), pDino->getb2Body()->GetWorldCenter(), pDino->isAwake());
		m_uiLastExecution = 0;
	}

}