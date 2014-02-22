#include "EntityManager.h"

namespace Symp {

EntityManager::EntityManager() {
	m_pEntity2dManager = new IND_Entity2dManager();
 	m_pPhysicalWorld = new PhysicalWorld();
}

EntityManager::~EntityManager(){
	m_pEntity2dManager->end();
    DISPOSE(m_pEntity2dManager);
	RenderEntity::end();
	delete m_pPhysicalWorld;
}

void EntityManager::initRender(Render* pRender) {
	m_pEntity2dManager->init(pRender->getIND_Render());
	RenderEntity::init(pRender);
}

bool EntityManager::addRenderEntity(RenderEntity* pRenderEntity, unsigned int layer){
	m_renderEntityArray.push_back(pRenderEntity);
	m_physicalEntityArray.push_back(NULL);
	return m_pEntity2dManager->add(layer, pRenderEntity->getIND_Entity2d());
}

bool EntityManager::addPhysicalEntity(PhysicalEntity* pPhysicalEntity){
	m_renderEntityArray.push_back(NULL);
	m_physicalEntityArray.push_back(pPhysicalEntity);
	return true;
}

bool EntityManager::addSoundEntity(SoundEntity*	pSoundEntity) {

	// Not implemented yet
	return false;
}

bool EntityManager::addEntity(RenderEntity* pRenderEntity, unsigned int layer, PhysicalEntity* pPhysicalEntity, SoundEntity* pSoundEntity){
	m_renderEntityArray.push_back(pRenderEntity);
	m_physicalEntityArray.push_back(pPhysicalEntity);	
	m_soundEntityArray.push_back(pSoundEntity);
	return m_pEntity2dManager->add(layer, pRenderEntity->getIND_Entity2d());
}

void EntityManager::renderEntities() {
	for(unsigned int layer = 0; layer < 64; ++layer)
		m_pEntity2dManager->renderEntities2d(layer);
	m_pEntity2dManager->renderEntities2d(1);
}

void EntityManager::updateEntities() {

	// Update Physical entities
	m_pPhysicalWorld->updatePhysics();

	// Update Render Entities
	for(size_t i = 0; i < m_renderEntityArray.size(); i++) {
		RenderEntity* rEntity = m_renderEntityArray.at(i);
		PhysicalEntity* pEntity = m_physicalEntityArray.at(i);
		if((pEntity != NULL) && (rEntity != NULL)) {
			rEntity->setPosition(pEntity->getPosition().x, pEntity->getPosition().y);
		}
	}
}

void EntityManager::deleteAllEntities() {
	m_physicalEntityArray.clear();
	m_renderEntityArray.clear();
}

bool EntityManager::deleteEntity(size_t index) {
	// Not implemented yet

	return false;
}

void EntityManager::addDino(int posX, int posY) {
	
	RenderEntity* rEntity = new RenderEntity("../assets/dino/dinoHeadache.png", Symp::Surface);
	rEntity->setHotSpot(0.5, 0.5); // TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	float scaleFactor = 0.3f;
	rEntity->setScale(scaleFactor, scaleFactor);
	
	float width = rEntity->getWidth() * scaleFactor;
	float height = rEntity->getHeight() * scaleFactor;

 	PhysicalEntity* pEntity = new PhysicalEntity(m_pPhysicalWorld->getWorld(), b2Vec2(posX, posY), b2Vec2(width, height));
	pEntity->setMass(50.f, 1.f);

	addEntity(rEntity, 63, pEntity, NULL);
}

RenderEntity* EntityManager::getRenderDino() const {
	return m_renderEntityArray[0];
}

PhysicalEntity* EntityManager::getPhysicalDino() const {
	return m_physicalEntityArray[0];

}

// Initialize singleton
IND_Entity2dManager*	EntityManager::m_pEntity2dManager = NULL;
}