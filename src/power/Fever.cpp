#include "Fever.h"

#include "../EntityManager.h"

namespace Symp {

Fever::Fever() : m_iMaxTemperature(1000) , m_iMinTemperature(-1000) {
	m_fCurrentTemperature = -1.f;
	m_iHotRange = 600;
	m_iColdRange = -600;
	m_iSpitFireRange = 800;
	m_iShiveringRange = -800;
	m_fTemperatureVariation = 1.f;
	m_isInHotZone = false;
	m_isInColdZone = false;
	m_iZoneVariationFactor = 4;
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
		EntityManager::getInstance()->setIsDinoShivering(true);
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
		EntityManager::getInstance()->setIsDinoShivering(false);
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

void Fever::shiverBackground() {

	clock_t t = clock();

	// Get all physical entities near from Dinos
	for(size_t i = 0; i < EntityManager::getInstance()->getPhysicalEntityArray().size(); i++) {

		PhysicalEntity* pEntity = EntityManager::getInstance()->getPhysicalEntityArray().at(i);

		if(pEntity != nullptr && pEntity != EntityManager::getInstance()->getPhysicalDino()) {

			b2Vec2 dinoPosition = EntityManager::getInstance()->getPhysicalDino()->getPosition();	 	
		 	b2Vec2 position = pEntity->getPosition();
		 	b2Vec2 distance = position - dinoPosition;

			if(sqrt(pow(distance.x, 2) + pow(distance.y, 2)) < 100) {

				// Get render entity
				RenderEntity* rEntity = EntityManager::getInstance()->getRenderEntityArray().at(EntityManager::getInstance()->getIndexEntity(pEntity)).at(0);
				rEntity->setAngleXYZ(0, 0, 1.5*cos(t));
			}

			// If the physical entity is 

		}
	}
}

}