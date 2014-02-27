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
	if(m_iCurrentTemperature >= m_iMaxTemperature) {
		EntityManager::getInstance()->killDino(DinoAction::DieByHot);
	}

	// Death by cold
	if(m_iCurrentTemperature <= m_iMinTemperature) {
		EntityManager::getInstance()->killDino(DinoAction::DieByFreeze);
	}
}


}