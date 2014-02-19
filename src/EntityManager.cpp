#include "EntityManager.h"

namespace Symp {

EntityManager::EntityManager() {
	EntityManager::m_pEntity2dManager = new IND_Entity2dManager();
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
	int32 numEntity = 0;
	std::vector<RenderEntity*>::iterator it;
	for(it = m_renderEntityArray.begin(); it != m_renderEntityArray.end(); ++it) {
		PhysicalEntity* tmpPhysicalEntity = m_physicalEntityArray.at(numEntity);
		RenderEntity* tmpRenderEntity = *it;
		// Commented because of an error (cf. Issue #20)
		// try {
		// 	tmpPhysicalEntity = m_physicalEntityArray.at(numEntity);
		// 	if(tmpPhysicalEntity != NULL && tmpRenderEntity != NULL) {
		// 		tmpRenderEntity->setPosition(tmpPhysicalEntity->getPosition().x, tmpPhysicalEntity->getPosition().y);
		// 	}
		// }
		// catch(const std::out_of_range&) {
		// 	std::cerr << "EntityManager::updateEntities function : out_of_range exception. The physical entity will not be update" << std::endl;
		// }
		if(tmpPhysicalEntity != NULL && tmpRenderEntity != NULL) {
			tmpRenderEntity->setPosition(tmpPhysicalEntity->getPosition().x, tmpPhysicalEntity->getPosition().y);
		}
		numEntity++;
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

void EntityManager::addDino() {
	RenderEntity *pRenderDino = new RenderEntity("../assets/dino/dinoHeadache.png", Symp::Surface);
	pRenderDino->setScale(0.3f, 0.3f);
	
	float width = pRenderDino->getWidth()*0.3f;
	float height = pRenderDino->getHeight()*0.3f;
 	PhysicalEntity* pPhysicalDino = new PhysicalEntity(m_pPhysicalWorld->getWorld(), b2Vec2(width, height));
	pPhysicalDino->setMass(50.f, 1.f);
	pPhysicalDino->setPosition(-620.f, 0.f);
	addEntity(pRenderDino, 63, pPhysicalDino, nullptr);
}

RenderEntity* EntityManager::getRenderDino() const {
	return m_renderEntityArray[0];
}

PhysicalEntity* EntityManager::getPhysicalDino() const {
	return m_physicalEntityArray[0];

}

// Initialize singleton
EntityManager*			EntityManager::m_instance = NULL;
IND_Entity2dManager*	EntityManager::m_pEntity2dManager = NULL;
}