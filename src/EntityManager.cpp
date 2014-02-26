#include "EntityManager.h"
#include "sound/SoundManager.h"
#include "power/Sneeze.h"

namespace Symp {

EntityManager::EntityManager() {
	m_pEntity2dManager = new IND_Entity2dManager();
 	m_pPhysicalWorld = new PhysicalWorld();

 	//Init powers
 	Sneeze* pSneeze = new Sneeze();
 	pSneeze->setRepulsionStrength(500);
 	pSneeze->setTimeToTriggerRandomSneeze(5);
 	m_powerArray.push_back(pSneeze);
}

EntityManager::~EntityManager(){
	m_pEntity2dManager->end();
    DISPOSE(m_pEntity2dManager);
	RenderEntity::end();
	delete m_pPhysicalWorld;
	for(std::vector<std::vector<RenderEntity*>>::iterator it = m_renderEntityArray.begin(); it != m_renderEntityArray.end();){
		it = m_renderEntityArray.erase(it);
	}
	for(std::vector<PhysicalEntity*>::iterator it = m_physicalEntityArray.begin(); it != m_physicalEntityArray.end();){
		it = m_physicalEntityArray.erase(it);
	}
	for(std::vector<std::vector<SoundEntity*>>::iterator it = m_soundEntityArray.begin(); it != m_soundEntityArray.end();){
		it = m_soundEntityArray.erase(it);
	}
	for(std::vector<Power*>::iterator it = m_powerArray.begin(); it != m_powerArray.end();){
		it = m_powerArray.erase(it);
	}
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

	float epsilon = 0.001f;
	if(getPhysicalDino()->getLinearVelocity().x < epsilon && getPhysicalDino()->getLinearVelocity().x > -epsilon) {
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
 	pEntity->setMass(50.f, 0.f);

	/*****************/
	/*     Sound     */
	/*****************/
	std::vector<SoundEntity*> soundEntityArray;

	// Flamewind
	size_t indexSound1 = SoundManager::getInstance()->loadSound("../assets/sounds/flamewind.ogg");
	SoundEntity* sEntity1 = new SoundEntity(indexSound1);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::Stop, sEntity1);

	size_t indexSound2 = SoundManager::getInstance()->loadSound("../assets/sounds/flamewind.ogg");
	SoundEntity* sEntity2 = new SoundEntity(indexSound2);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::WalkRight, sEntity2);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::WalkLeft, sEntity2);

	// Jump
	size_t indexSound3 = SoundManager::getInstance()->loadSound("../assets/sounds/jump.ogg");
	SoundEntity* sEntity3 = new SoundEntity(indexSound3);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::Jump, sEntity3);

	// Sneeze
	size_t indexSound4 = SoundManager::getInstance()->loadSound("../assets/sounds/sneeze.ogg");
	SoundEntity* sEntity4 = new SoundEntity(indexSound4);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::Sneezing, sEntity4);

	// Death by fall
	size_t indexSound5 = SoundManager::getInstance()->loadSound("../assets/sounds/death-fall.ogg");
	SoundEntity* sEntity5 = new SoundEntity(indexSound5);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::DieByFall, sEntity5);

	// Death by spikes
	size_t indexSound6 = SoundManager::getInstance()->loadSound("../assets/sounds/death-pikes.ogg");
	SoundEntity* sEntity6 = new SoundEntity(indexSound6);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::DieBySpikes, sEntity6);

	// Death by freeze
	size_t indexSound7 = SoundManager::getInstance()->loadSound("../assets/sounds/death-freeze.ogg");
	SoundEntity* sEntity7 = new SoundEntity(indexSound7);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::DieByFreeze, sEntity7);

	/*****************/
	/*   Add Dino    */
	/*****************/
	m_uiDinoIndex = getNbEntities();
	addEntity(renderEntityArray, 63, pEntity, soundEntityArray);
}

void EntityManager::killDino(DinoAction action) {
	// Animation

	// Play sound
	SoundManager::getInstance()->play(action);

}

void EntityManager::executePowers() {
	for(size_t i = 0; i < m_powerArray.size(); ++i){
		m_powerArray[i]->execute();
	}
}

DinoAction EntityManager::getCurrentDinoAction() const {
	size_t indexCurrentDino = 0;
	for(size_t indexRenderDino = 0; indexRenderDino < getRenderDino().size(); ++indexRenderDino){
		if(getRenderDino()[indexRenderDino]->isShow()){
			indexCurrentDino = indexRenderDino;
			break;
		}
	}
	return static_cast<DinoAction>(indexCurrentDino);
}

// Initialize singleton
IND_Entity2dManager*	EntityManager::m_pEntity2dManager = NULL;
}