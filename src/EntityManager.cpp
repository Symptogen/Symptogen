#include "EntityManager.h"
#include "sound/SoundManager.h"

namespace Symp {

EntityManager::EntityManager() {
	m_pEntity2dManager = new IND_Entity2dManager();
 	m_pPhysicalWorld = new PhysicalWorld();
 	m_pSneezePower = new Sneeze();
 	m_pSneezePower->setRepulsionStrength(500);
 	m_pSneezePower->setTimeToTriggerRandomSneeze(1000);
}

EntityManager::~EntityManager(){
	m_pEntity2dManager->end();
    DISPOSE(m_pEntity2dManager);
	RenderEntity::end();
	delete m_pPhysicalWorld;
	delete m_pSneezePower;
}

void EntityManager::initRender(Render* pRender) {
	m_pEntity2dManager->init(pRender->getIND_Render());
	RenderEntity::init(pRender);
}

bool EntityManager::addRenderEntity(std::vector<RenderEntity*> renderEntityArray, unsigned int layer) {
	m_renderEntityArray.push_back(renderEntityArray);
	m_physicalEntityArray.push_back(NULL);
	m_soundEntityArray.push_back(std::vector<SoundEntity*>());
	bool check = false;
	for(size_t indexRenderEntity = 0; indexRenderEntity < renderEntityArray.size(); ++indexRenderEntity)
		check = m_pEntity2dManager->add(layer, renderEntityArray[indexRenderEntity]->getIND_Entity2d());
	return check;
}

bool EntityManager::addPhysicalEntity(PhysicalEntity* pPhysicalEntity) {
	m_renderEntityArray.push_back(std::vector<RenderEntity*>());
	m_physicalEntityArray.push_back(pPhysicalEntity);
	m_soundEntityArray.push_back(std::vector<SoundEntity*>());
	return true;
}

bool EntityManager::addSoundEntity(std::vector<SoundEntity*> pSoundEntityArray) {
	m_renderEntityArray.push_back(std::vector<RenderEntity*>());
	m_physicalEntityArray.push_back(NULL);
	m_soundEntityArray.push_back(pSoundEntityArray);
	return true;
}

bool EntityManager::addEntity(std::vector<RenderEntity*> renderEntityArray, unsigned int layer, PhysicalEntity* pPhysicalEntity, std::vector<SoundEntity*> pSoundEntityArray) {
	m_renderEntityArray.push_back(renderEntityArray);
	m_physicalEntityArray.push_back(pPhysicalEntity);	
	m_soundEntityArray.push_back(pSoundEntityArray);
	bool check = false;
	for(size_t indexRenderEntity = 0; indexRenderEntity < renderEntityArray.size(); ++indexRenderEntity)
		check = m_pEntity2dManager->add(layer, renderEntityArray[indexRenderEntity]->getIND_Entity2d());
	return check;
}

bool EntityManager::addRenderEntityToExistingEntity(RenderEntity* renderEntity, size_t indexExistingEntity) {
	m_renderEntityArray[indexExistingEntity].push_back(renderEntity);
	return true;
}

bool EntityManager::addSoundEntityToExistingEntity(SoundEntity* soundEntity, size_t indexExistingEntity) {
	m_soundEntityArray[indexExistingEntity].push_back(soundEntity);
	return true;
}

void EntityManager::renderEntities() {
	for(unsigned int layer = 0; layer < 64; ++layer)
		m_pEntity2dManager->renderEntities2d(layer);
}

void EntityManager::updateEntities() {
	// Update Physical entities
	m_pPhysicalWorld->updatePhysics();

	m_pSneezePower->execute();

	if(getPhysicalDino()->getLinearVelocity().x == 0) {
		getRenderDino().at(DinoAction::Stop)->setShow(true);
		getRenderDino().at(DinoAction::WalkRight)->setShow(false);
	}
	else {
		getRenderDino().at(DinoAction::Stop)->setShow(false);
		getRenderDino().at(DinoAction::WalkRight)->setShow(true);
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
	m_soundEntityArray.clear();
}

bool EntityManager::deleteEntity(size_t index) {
	// Not implemented yet
	return false;
}

void EntityManager::addDino(int posX, int posY, int doorHeight) {
	/*****************/
	/*    Render     */
	/*****************/
	std::vector<RenderEntity*> renderEntityArray;

	RenderEntity* rEntity1 = new RenderEntity("../assets/surface/dino/dinoStop.png", Symp::Surface);
	float scaleFactor = (float)doorHeight / (float)rEntity1->getHeight();
	rEntity1->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntity1->setHotSpot(0.5, 0.5);
	rEntity1->setShow(true);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::Stop, rEntity1);

	RenderEntity* rEntity2 = new RenderEntity("../assets/animation/dino_animation.xml", Symp::Animation);
	rEntity2->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntity2->setHotSpot(0.5, 0.5);
	rEntity2->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::WalkRight, rEntity2);

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
	/*     Sound     */
	/*****************/
	std::vector<SoundEntity*> soundEntityArray;

	size_t indexSound1 = SoundManager::getInstance()->loadSound("../assets/sounds/flamewind.ogg");
	SoundEntity* sEntity1 = new SoundEntity(indexSound1);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::Stop, sEntity1);

	size_t indexSound2 = SoundManager::getInstance()->loadSound("../assets/sounds/flamewind.ogg");
	SoundEntity* sEntity2 = new SoundEntity(indexSound2);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::WalkRight, sEntity2);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::WalkLeft, sEntity2);

	size_t indexSound3 = SoundManager::getInstance()->loadSound("../assets/sounds/jump.ogg");
	SoundEntity* sEntity3 = new SoundEntity(indexSound3);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::Jump, sEntity3);

	/*****************/
	/*   Add Dino    */
	/*****************/
	m_uiDinoIndex = getNbEntities();
	addEntity(renderEntityArray, 63, pEntity, soundEntityArray);
}

std::vector<RenderEntity*> EntityManager::getRenderDino() const {
	return m_renderEntityArray[m_uiDinoIndex];
}

PhysicalEntity* EntityManager::getPhysicalDino() const {
	return m_physicalEntityArray[m_uiDinoIndex];
}

std::vector<SoundEntity*> EntityManager::getSoundDino() const {
	return m_soundEntityArray[m_uiDinoIndex];
}

bool EntityManager::isDinoReady() const {
	return (getRenderDino().size() > 0 && getPhysicalDino() != NULL) ? true : false;
}

// Initialize singleton
IND_Entity2dManager*	EntityManager::m_pEntity2dManager = NULL;
}