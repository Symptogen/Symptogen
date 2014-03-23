#include "Headache.h"
#include "../GameManager.h"
#include "../sound/SoundManager.h"

namespace Symp {

void Headache::execute(){

	if(time(NULL) - m_uiLastExecution >= m_uiTimeToTriggerRandomHeadache && !isActivated()) {
		// With these values, the the rotation occures 10 times in 2 minutes
		float random = rand() % 1000; // between 0 and 9999
		float treshold = 1000 - (time(NULL) - m_uiLastExecution);
		if(random > treshold) {
			m_iRotationAngle = rand() % m_iMaxRotationAngle + m_iMinRotationAngle;
			activate();

			// Render Invisible Platforms visible
			for(size_t i = 0; i < EntityManager::getInstance()->getPhysicalEntityArray().size(); i++) {

				PhysicalEntity* pEntity = EntityManager::getInstance()->getPhysicalEntity(i);
				if(pEntity != nullptr
					&& pEntity->getType() == PhysicalType::InvisibleObject) {

					std::vector<RenderEntity*> rEntityArray = EntityManager::getInstance()->getRenderEntityArray().at(i);
					rEntityArray.at(0)->setShow(true);
				}

			}

			// Sound
			SoundManager::getInstance()->playSound(EntityManager::getInstance()->getSoundDino()[DinoAction::HeadacheAction]);
		}
	}
	if(isActivated()){
		forceExecution();
	}

}

void Headache::forceExecution(){

	// Increase the step
	if(m_iRotationAngle>0) m_iInterpolateAngle += m_uiStep;
	else m_iInterpolateAngle -= m_uiStep;

	if(abs(m_iInterpolateAngle)>abs(m_iRotationAngle) && m_bChangeSens){ // if the interpolate angle is bigger than the rotation angle
		m_iRotationNewAngle=abs(m_iRotationAngle)-abs(m_iRotationAngle*0.20); // Compute new angle 
		m_bChangeSens = false;
		m_iRotationAngle*=-1; // change the direction of rotation
	}
	else if(abs(m_iInterpolateAngle)<=abs(m_iRotationNewAngle) && m_iRotationNewAngle!=0){
		if(m_iRotationAngle<0) m_iRotationAngle = -m_iRotationNewAngle;
		else m_iRotationAngle = m_iRotationNewAngle;
		m_bChangeSens = true;
	}

	// Change the step value
	if(abs(m_iInterpolateAngle)>abs(m_iRotationAngle)*0.20 && abs(m_iInterpolateAngle)<abs(m_iRotationAngle)*0.80) m_uiStep = 5;
	else m_uiStep = 1;

	if(abs(m_iRotationAngle)<5){
		deactivate();

		// Render Invisible Platforms
		for(size_t i = 0; i < EntityManager::getInstance()->getPhysicalEntityArray().size(); i++) {

			PhysicalEntity* pEntity = EntityManager::getInstance()->getPhysicalEntity(i);
			if(pEntity != nullptr
				&& pEntity->getType() == PhysicalType::InvisibleObject) {

				std::vector<RenderEntity*> rEntityArray = EntityManager::getInstance()->getRenderEntityArray().at(i);
				rEntityArray.at(0)->setShow(false);
			}

		}

		// Sound
		SoundManager::getInstance()->stopSound(EntityManager::getInstance()->getSoundDino()[DinoAction::HeadacheAction]);

		m_uiLastExecution = time(NULL);
		// Reset the camera
		m_iInterpolateAngle = 0;
		m_iRotationNewAngle = 0;
		m_bChangeSens = true;
		m_uiStep = 1;

		// Increase the minimum rotation angle if necessary
		if(m_iMinRotationAngle-30<m_iMaxRotationAngle) m_iMinRotationAngle += 10;
	}

	// Rotate the camera
	GameManager::getInstance()->getRender()->setCameraAngle(m_iInterpolateAngle);

}

}