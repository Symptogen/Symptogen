#include "Headache.h"
#include "../GameManager.h"
#include "../sound/SoundManager.h"

namespace Symp {

Headache::Headache(){
		m_uiLastExecution = time(NULL);
		m_iRotationAngle = 0;
		m_iMaxRotationAngle = 180;
		m_iMinRotationAngle = 10;
		m_iInterpolateAngle = 0;
		m_uiStep = 0;
		m_iRotationNewAngle = 0;
		m_bChangeSens = true;
		m_bFirstLoop = true;
		m_fVolume = 0.0;
}

void Headache::execute(){

	if(m_bFirstLoop){
		m_bFirstLoop = false;
		// Sound
		SoundManager::getInstance()->playSound(SoundType::HEADACHE);	
	}
	SoundManager::getInstance()->setVolume(SoundType::HEADACHE, m_fVolume);
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
		}
	}
	if(isActivated()){
		forceExecution();
	}

}

void Headache::forceExecution(){
	m_fVolume += 0.05;

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
		m_fVolume = 0.0;
		
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