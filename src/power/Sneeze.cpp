#include "Sneeze.h"

#include "../EntityManager.h"
#include "../render/RenderEntity.h"
#include "../physic/PhysicalEntity.h"
#include "../sound/SoundManager.h"

namespace Symp {

	void Sneeze::execute() {
		//activate power if trigger by random
		if(!isWarningSneeze() && !isSneezing()){
			if(time(NULL) - m_uiLastExecution >= m_uiTimeToTriggerRandomSneeze) {
				// With these values, the dino has sneeze 10 times in 2 minutes
				float random = rand() % 10000; // between 0 and 1
				float treshold = 10000 - (time(NULL) - m_uiLastExecution);
				if(random > treshold) {
					m_pTimer->start();
					setToWarningSneeze();
					//render
					EntityManager::getInstance()->setDinoRender(DinoAction::Sneezing);
				}
			}
		}
		//if warning and power activated since the 1/2 of the time animation
		else if(isWarningSneeze() && m_pTimer->getTicks() > AnimationLength::SneezeLength*0.5f){
			forceExecution();
		}
		//if real sneeze and power activated since at least the time animation
		//TODO : stop using *0.9f, patch for the animation repetition
		else if(isSneezing() && m_pTimer->getTicks() >= AnimationLength::SneezeLength*0.9f){
			forceExecution();
		}
	}

	void Sneeze::forceExecution() {
		//activate power if forced
		if(!isWarningSneeze() && !isSneezing()){
			m_pTimer->start();
			setToWarningSneeze();
			//render
			EntityManager::getInstance()->setDinoRender(DinoAction::Sneezing);
		}
		//if warning and power activated since the 1/2 of the time animation
		else if(isWarningSneeze() && m_pTimer->getTicks() > AnimationLength::SneezeLength*0.5f){
			activate();
			setToSneezing();

			PhysicalEntity* pDino = EntityManager::getInstance()->getPhysicalDino();
			float impulseX = pDino->getMass() * m_uiRepulsionStrength;
			float impulseY = -((pDino->getMass() * m_uiRepulsionStrength) / 2);

			// Check Dino way using the displayed image instead of the physical datas
			bool isWalkingToRight = !EntityManager::getInstance()->getRenderDino()[DinoAction::Walk]->isFlippedHorizontaly();
			if(isWalkingToRight) {
				impulseX = -impulseX;
			}

			//physical
			pDino->getb2Body()->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), pDino->getb2Body()->GetWorldCenter(), pDino->isAwake());
			m_uiLastExecution = time(NULL);
			//sound
			SoundManager::getInstance()->play(EntityManager::getInstance()->getSoundDino()[DinoAction::Sneezing]->getIndexSound());
		}
		//if real sneeze and power activated since at least the time animation
		//TODO : stop using *0.9f, patch for the animation repetition
		else if(isSneezing() && m_pTimer->getTicks() >= AnimationLength::SneezeLength*0.9f){
			m_pTimer->stop();
			deactivate();
			
			setToStopSneezing();
		}
	}
}