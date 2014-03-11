#include "Headache.h"
#include "../GameManager.h"

namespace Symp {

void Headache::execute(){
	if(time(NULL) - m_uiLastExecution >= m_uiTimeToTriggerRandomHeadache && !isActivated()) {
		// With these values, the dino has sneeze 10 times in 2 minutes
		float random = rand() % 10000; // between 0 and 1
		float treshold = 10000 - (time(NULL) - m_uiLastExecution);
		if(random > treshold) {
			activate();
		}
	}
	if(isActivated()) 
		forceExecution();
}

void Headache::forceExecution(){

	if(m_iRotationAngle>0){
		m_iInterpolateAngle += m_uiStep;
	}
	else{
		m_iInterpolateAngle -= m_uiStep;
	}

	if(abs(m_iInterpolateAngle)>=abs(m_iRotationAngle)){
		//m_iRotationAngle-=abs(m_iRotationAngle*0.20);
		m_iRotationAngle*=-1;
	}

	if(abs(m_iInterpolateAngle)>abs(m_iRotationAngle)*0.20 && abs(m_iInterpolateAngle)<abs(m_iRotationAngle)*0.80) m_uiStep = 5;
	else m_uiStep = 1;

	if(abs(m_iRotationAngle)<5){
		deactivate();
		m_uiLastExecution = time(NULL);
	}
	GameManager::getInstance()->getRender()->setCameraAngle(m_iInterpolateAngle);
}

}