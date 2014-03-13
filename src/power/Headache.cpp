#include "Headache.h"
#include "../GameManager.h"

namespace Symp {

void Headache::execute(){
	if(time(NULL) - m_uiLastExecution >= m_uiTimeToTriggerRandomHeadache && !isActivated()) {
		// With these values, the the rotation occures 10 times in 2 minutes
		float random = rand() % 10000; // between 0 and 9999
		float treshold = 10000 - (time(NULL) - m_uiLastExecution);
		if(random > treshold) {
			m_iRotationAngle = rand() % m_iMaxRotationAngle + m_iMinRotationAngle;
			activate();
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