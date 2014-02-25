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

bool EntityManager::addRenderEntity(std::vector<RenderEntity*> renderEntityArray, unsigned int layer) {
	m_renderEntityArray.push_back(renderEntityArray);
	m_physicalEntityArray.push_back(NULL);
	m_soundEntityArray.push_back(NULL);
	bool check = false;
	for(size_t indexRenderEntity = 0; indexRenderEntity < renderEntityArray.size(); ++indexRenderEntity)
		check = m_pEntity2dManager->add(layer, renderEntityArray[indexRenderEntity]->getIND_Entity2d());
	return check;
}

bool EntityManager::addPhysicalEntity(PhysicalEntity* pPhysicalEntity) {
	m_renderEntityArray.push_back(std::vector<RenderEntity*>());
	m_physicalEntityArray.push_back(pPhysicalEntity);
	m_soundEntityArray.push_back(NULL);
	return true;
}

bool EntityManager::addSoundEntity(SoundEntity*	pSoundEntity) {
	m_renderEntityArray.push_back(std::vector<RenderEntity*>());
	m_physicalEntityArray.push_back(NULL);
	m_soundEntityArray.push_back(pSoundEntity);
	return true;
}

bool EntityManager::addEntity(std::vector<RenderEntity*> renderEntityArray, unsigned int layer, PhysicalEntity* pPhysicalEntity, SoundEntity* pSoundEntity){
	m_renderEntityArray.push_back(renderEntityArray);
	m_physicalEntityArray.push_back(pPhysicalEntity);	
	m_soundEntityArray.push_back(pSoundEntity);
	bool check = false;
	for(size_t indexRenderEntity = 0; indexRenderEntity < renderEntityArray.size(); ++indexRenderEntity)
		check = m_pEntity2dManager->add(layer, renderEntityArray[indexRenderEntity]->getIND_Entity2d());
	return check;
}

bool EntityManager::addRenderEntityToExistingEntity(RenderEntity* renderEntity, size_t indexExistingEntity){
	m_renderEntityArray[indexExistingEntity].push_back(renderEntity);
	return true;
}

void EntityManager::renderEntities() {
	for(unsigned int layer = 0; layer < 64; ++layer)
		m_pEntity2dManager->renderEntities2d(layer);
	m_pEntity2dManager->renderEntities2d(1);
}

void EntityManager::updateEntities() {
	// Update Physical entities
	m_pPhysicalWorld->updatePhysics();

	if(getPhysicalDino()->getLinearVelocity().x == 0) {
		
	}

	// Update Render Entities
	for(size_t i = 0; i < m_renderEntityArray.size(); i++) {
		std::vector<RenderEntity*> rEntities = m_renderEntityArray.at(i);
		PhysicalEntity* pEntity = m_physicalEntityArray.at(i);
		if((rEntities.size() > 0) && (pEntity != nullptr)) {
			for(size_t indexRenderEntity = 0; indexRenderEntity < rEntities.size(); ++indexRenderEntity)
				rEntities[indexRenderEntity]->setPosition(pEntity->getPosition().x, pEntity->getPosition().y);
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

void EntityManager::addDino(int posX, int posY, int doorHeight) {
	std::vector<RenderEntity*> renderEntityArray;

	/*****************/
	/*    Render     */
	/*****************/
	RenderEntity* rEntity1 = new RenderEntity("../assets/surface/dino/dinoStop.png", Symp::Surface);
	float scaleFactor = (float)doorHeight / (float)rEntity1->getHeight();
	rEntity1->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntity1->setHotSpot(0.5, 0.5);
	rEntity1->setShow(false);
	renderEntityArray.push_back(rEntity1);

	RenderEntity* rEntity2 = new RenderEntity("../assets/animation/dino_animation.xml", Symp::Animation);
	rEntity2->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntity2->setHotSpot(0.5, 0.5);
	rEntity2->setShow(true);
	renderEntityArray.push_back(rEntity2);

	/*****************/
	/*   Physical    */
	/*****************/
	float width = rEntity1->getWidth();
	float height = rEntity1->getHeight();

 	PhysicalEntity* pEntity = new PhysicalEntity(
 		m_pPhysicalWorld->getWorld(), 
 		b2Vec2(posX, posY), 
 		b2Vec2(width, height), 
 		PhysicalType::Dino
 		);
	pEntity->setMass(50.f, 1.f);

	/*****************/
	/*   Add Dino    */
	/*****************/
	m_uiDinoIndex = getNbEntities();
	addEntity(renderEntityArray, 63, pEntity, NULL);
}

std::vector<RenderEntity*> EntityManager::getRenderDino() const {
	return m_renderEntityArray[m_uiDinoIndex];
}

PhysicalEntity* EntityManager::getPhysicalDino() const {
	return m_physicalEntityArray[m_uiDinoIndex];
}

bool EntityManager::isDinoReady() const {
	return (getRenderDino().size() > 0 && getPhysicalDino() != NULL) ? true : false;
}

// Initialize singleton
IND_Entity2dManager*	EntityManager::m_pEntity2dManager = NULL;
}