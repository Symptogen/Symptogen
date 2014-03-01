#include "Fever.h"

#include "../EntityManager.h"

namespace Symp {

Fever::Fever() : m_iMaxTemperature(1000) , m_iMinTemperature(-1000) {
	m_iCurrentTemperature = 0;
	m_uiHotRange = 700;
	m_uiColdRange = -700;
	m_fTemperatureVariation = 1;
}

Fever::~Fever() {
	
}

void Fever::execute() {
	
	// If the temperature is near the temperature max
	if(m_iCurrentTemperature >= 0) {
		m_iCurrentTemperature += m_fTemperatureVariation;
	}

	// If the temperature is near the temperature min
	else {
		m_iCurrentTemperature -= m_fTemperatureVariation;
	
	}

	// Fever power
	if(m_iCurrentTemperature > m_uiHotRange) {
		// Animation

		// Destroy objects
	}

	// Shivering power
	if(m_iCurrentTemperature < m_uiColdRange) {
		// Animation

		// Shiver
	}

	// Death by hot
	if(m_iCurrentTemperature >= m_iMaxTemperature && !EntityManager::getInstance()->getRenderDino().at(DinoAction::Die)->isAnimationPlaying()) {
		EntityManager::getInstance()->killDino(DinoAction::Die);
	}

	// Death by cold
	if(m_iCurrentTemperature <= m_iMinTemperature) {
		EntityManager::getInstance()->killDino(DinoAction::Die);
	}
}

size_t Fever::getThermometerStep() const {
	//die by hot
	if(m_iCurrentTemperature >= m_iMaxTemperature)
		return 7;
	//die by cold
	else if(m_iCurrentTemperature <= m_iMinTemperature)
		return 0;
	//hot power
	else if(m_iCurrentTemperature > m_uiHotRange)
		return 6;
	//cold power
	else if(m_iCurrentTemperature < m_uiColdRange)
		return 1;
	//neutral zone
	else{
		int rangeNeutralZone = m_uiHotRange - m_uiColdRange;
		int oneStepInNeutralZone = rangeNeutralZone / 4;
		if(m_iCurrentTemperature < (m_uiColdRange + oneStepInNeutralZone))
			return 2;
		else if(m_iCurrentTemperature > (m_uiColdRange + oneStepInNeutralZone) && m_iCurrentTemperature < (m_uiColdRange + 2*oneStepInNeutralZone))
			return 3;
		else if(m_iCurrentTemperature > (m_uiColdRange + 2*oneStepInNeutralZone) && m_iCurrentTemperature < (m_uiColdRange + 3*oneStepInNeutralZone))
			return 4;
		else
			return 5;
	}
}

}