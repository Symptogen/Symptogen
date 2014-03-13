#include "Fever.h"

#include "../EntityManager.h"

namespace Symp {

Fever::Fever() : m_iMaxTemperature(1000) , m_iMinTemperature(-1000) {
	m_fCurrentTemperature = 1.f;
	m_iHotRange = 600;
	m_iColdRange = -600;
	m_iSpitFireRange = 800;
	m_iShiveringRange = -800;
	m_fTemperatureVariation = 1.f;
	m_isInHotZone = false;
	m_isInColdZone = false;
	m_iZoneVariationFactor = 2;
}

Fever::~Fever() {
	
}

void Fever::execute() {

	if(m_isInHotZone) {
		if(m_fCurrentTemperature >= 0)
			m_fCurrentTemperature += m_fTemperatureVariation * m_iZoneVariationFactor;
		else
			m_fCurrentTemperature += m_fTemperatureVariation;
	}
	else if(m_isInColdZone) {
		if(m_fCurrentTemperature < 0)
			m_fCurrentTemperature -= m_fTemperatureVariation * m_iZoneVariationFactor;
		else
			m_fCurrentTemperature -= m_fTemperatureVariation;
	}
	else {
		if(m_fCurrentTemperature >= 0)
			m_fCurrentTemperature += m_fTemperatureVariation;
		else
			m_fCurrentTemperature -= m_fTemperatureVariation;
	}
	

	// Exception : nothng happend if warning of sneeze or sneeze are in process
	if(m_fCurrentTemperature > m_iSpitFireRange && EntityManager::getInstance()->getCurrentPowerType() != PowerType::SneezeType) {
		activate(); //really useful for this power ?
		EntityManager::getInstance()->addFlames();
	}
	// Shivering power
	else if(m_fCurrentTemperature < m_iShiveringRange && EntityManager::getInstance()->getCurrentPowerType() != PowerType::SneezeType){
		activate();
		// Animation
		EntityManager::getInstance()->setDinoRender(DinoAction::WalkShivering);
		// Shiver
	}
	else{
		deactivate();
		//delete flames if necessary
		for(size_t indexEntity = 0; indexEntity < EntityManager::getInstance()->getPhysicalEntityArray().size(); ++indexEntity) {
			if(EntityManager::getInstance()->getPhysicalEntityArray().at(indexEntity) != nullptr){
				if(EntityManager::getInstance()->getPhysicalEntityArray().at(indexEntity)->getType() == PhysicalType::Flames){
					EntityManager::getInstance()->deleteEntity(indexEntity);
				}
			}
		}
	}

	// Death by hot
	if(m_fCurrentTemperature >= m_iMaxTemperature) {
		EntityManager::getInstance()->killDino(DinoAction::DeathFever);
	}

	// Death by cold
	if(m_fCurrentTemperature <= m_iMinTemperature) {
		EntityManager::getInstance()->killDino(DinoAction::DeathHypothermia);
	}
}

void Fever::forceExecution() {
	// Fever power
	if(m_fCurrentTemperature > 0) {
		EntityManager::getInstance()->addFlames();
	}
	// Shivering power
	else{

	}
}

}