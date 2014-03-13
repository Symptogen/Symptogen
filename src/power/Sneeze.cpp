#include "Sneeze.h"

#include "../EntityManager.h"
#include "../render/RenderEntity.h"
#include "../physic/PhysicalEntity.h"
#include "../sound/SoundManager.h"
#include "Fever.h"

namespace Symp {

	void Sneeze::execute() {
		// Do not trigger sneeze if the fever power is activated
		if(EntityManager::getInstance()->isPowerExisting(PowerType::FeverType) && EntityManager::getInstance()->getPower(PowerType::FeverType)->isActivated())
			return;

		// Activate power if trigger by random
		if(!isWarningSneeze() && !isSneezing()){
			if(time(NULL) - m_uiLastExecution >= m_uiTimeToTriggerRandomSneeze) {
				// With these values, the dino has sneeze 10 times in 2 minutes
				float random = rand() % 10000; // between 0 and 1
				float treshold = 10000 - (time(NULL) - m_uiLastExecution);
				if(random > treshold) {
					m_pTimer->start();
					setToWarningSneeze();
					
					// Render Sneeze with Fever
					if(EntityManager::getInstance()->getCurrentDinoAction() == DinoAction::StopFever
						||	EntityManager::getInstance()->getCurrentDinoAction() == DinoAction::WalkFever) {
							//std::cout <<" Fever" << std::endl;
							EntityManager::getInstance()->setDinoRender(DinoAction::FeverSneezing);
					}

					// Render Sneeze with Cold
					else if(EntityManager::getInstance()->getCurrentDinoAction() == DinoAction::StopHypothermia
						||	EntityManager::getInstance()->getCurrentDinoAction() == DinoAction::WalkHypothermia) {
						//std::cout << "Cold" << std::endl;
						EntityManager::getInstance()->setDinoRender(DinoAction::ColdSneezing);
					}

					// Normal sneeze
					else {
						//std::cout <<" Normal" << std::endl;
						EntityManager::getInstance()->setDinoRender(DinoAction::Sneezing);
					}
				}
			}
		}

		// If warning and power activated since the 1/2 of the time animation
		else if(isWarningSneeze() && m_pTimer->getTicks() > AnimationLength::SneezeLength*0.5f){
			forceExecution();
		}

		// If real sneeze and power activated since at least the time animation
		else if(isSneezing() && m_pTimer->getTicks() >= AnimationLength::SneezeLength){
			forceExecution();
		}
	}

	void Sneeze::forceExecution() {

		//std::cout << "forceExecution" << std::endl;

		// Activate power if forced
		if(!isWarningSneeze() && !isSneezing()){
			m_pTimer->start();
			setToWarningSneeze();	


			//std::cout <<"  Activate power if forced" << std::endl;

			// Render Sneeze with Fever
			if(EntityManager::getInstance()->getCurrentDinoAction() == DinoAction::StopFever
				||	EntityManager::getInstance()->getCurrentDinoAction() == DinoAction::WalkFever) {
				//std::cout <<" Fever" << std::endl;
					EntityManager::getInstance()->setDinoRender(DinoAction::FeverSneezing);

			}

			// Render Sneeze with Cold
			else if(EntityManager::getInstance()->getCurrentDinoAction() == DinoAction::StopHypothermia
				||	EntityManager::getInstance()->getCurrentDinoAction() == DinoAction::WalkHypothermia) {
				//std::cout <<" Cold" << std::endl;
				EntityManager::getInstance()->setDinoRender(DinoAction::ColdSneezing);
			}

			// Normal sneeze
			else {
				//std::cout <<" Normal" << std::endl;
				EntityManager::getInstance()->setDinoRender(DinoAction::Sneezing);
			}
		}
		// If warning and power activated since the 1/2 of the time animation
		else if(isWarningSneeze() && m_pTimer->getTicks() > AnimationLength::SneezeLength*0.5f){
			//std::cout << "else if" << std::endl;
			activate();
			setToSneezing();

			PhysicalEntity* pDino = EntityManager::getInstance()->getPhysicalDino();
			float impulseX = pDino->getMass() * m_uiRepulsionStrength;
			float impulseY = -((pDino->getMass() * m_uiRepulsionStrength) / 2);

			// Check Dino way using the displayed image instead of the physical datas
			bool isWalkingToRight = !EntityManager::getInstance()->getRenderDino()[EntityManager::getInstance()->getRightWalk()]->isFlippedHorizontaly();
			if(isWalkingToRight) {
				impulseX = -impulseX;
			}

			// Physical
			pDino->getb2Body()->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), pDino->getb2Body()->GetWorldCenter(), pDino->isAwake());
			m_uiLastExecution = time(NULL);
			// Sound
			SoundManager::getInstance()->play(EntityManager::getInstance()->getSoundDino()[DinoAction::Sneezing]->getIndexSound());
		}
		// If real sneeze and power activated since at least the time animation
		else if(isSneezing() && m_pTimer->getTicks() >= AnimationLength::SneezeLength){
			//std::cout << "Coucou" << std::endl;
			m_pTimer->stop(); 
			deactivate();
			setToStopSneezing();
		}
	}
}