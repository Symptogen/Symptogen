#include "Sneeze.h"

#include "../EntityManager.h"
#include "../physic/PhysicalEntity.h"
#include "../sound/SoundManager.h"

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
		activate();
		PhysicalEntity* pDino = EntityManager::getInstance()->getPhysicalDino();
		float impulseX = pDino->getMass() * m_uiRepulsionStrength;
		if(pDino->getLinearVelocity().x > 0) {
			impulseX = -impulseX;
		}
		float impulseY = -((pDino->getMass() * m_uiRepulsionStrength) / 2);
		pDino->getb2Body()->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), pDino->getb2Body()->GetWorldCenter(), pDino->isAwake());
		m_uiLastExecution = time(NULL);

		std::vector<SoundEntity*> sDinoArray = EntityManager::getInstance()->getSoundDino();
		SoundManager::getInstance()->play(sDinoArray[DinoAction::Sneezing]->getIndexSound());
	}

	void Sneeze::updatePowerTimer(){
		if(isActivated()) m_pTimer->start();
		else if(m_pTimer->getTicks()>=750){
    		m_pTimer->stop();
   		 	deactivate();
 		}
	}
}