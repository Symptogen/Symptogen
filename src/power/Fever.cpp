#include "Fever.h"

#include "../EntityManager.h"

namespace Symp {

Fever::Fever() : m_iMaxTemperature(1000) , m_iMinTemperature(-1000) {
	m_iCurrentTemperature = 1;
	m_uiHotRange = 700;
	m_uiColdRange = -700;
	m_fTemperatureVariation = 1;
	m_isInHotZone = false;
	m_isInColdZone = false;
	m_iZoneVariationFactor = 4;

}

Fever::~Fever() {
	
}

void Fever::execute() {

	if(m_isInHotZone) {
		if(m_iCurrentTemperature >= 0)
			m_iCurrentTemperature += m_fTemperatureVariation * m_iZoneVariationFactor;
		else
			m_iCurrentTemperature += m_fTemperatureVariation;
	}
	else if(m_isInColdZone) {
		if(m_iCurrentTemperature < 0)
			m_iCurrentTemperature -= m_fTemperatureVariation * m_iZoneVariationFactor;
		else
			m_iCurrentTemperature -= m_fTemperatureVariation;
	}
	else {
		if(m_iCurrentTemperature >= 0)
			m_iCurrentTemperature += m_fTemperatureVariation;
		else
			m_iCurrentTemperature -= m_fTemperatureVariation;
	}
	

	// Fever power
	if(m_iCurrentTemperature > m_uiHotRange) {
		activate();
		EntityManager::getInstance()->addFlames();
	}

	// Shivering power
	else if(m_iCurrentTemperature < m_uiColdRange) {
		activate();
		// Animation

		// Shiver
	}
	else{
		deactivate();
	}

	// Death by hot
	if(m_iCurrentTemperature >= m_iMaxTemperature) {
		EntityManager::getInstance()->killDino(DinoAction::DeathFever);
	}

	// Death by cold
	if(m_iCurrentTemperature <= m_iMinTemperature) {
		EntityManager::getInstance()->killDino(DinoAction::DeathHypothermia);
	}
}

void Fever::forceExecution() {
	// Fever power
	if(m_iCurrentTemperature > 0) {
		EntityManager::getInstance()->addFlames();
	}
	// Shivering power
	else{

	}
}

}