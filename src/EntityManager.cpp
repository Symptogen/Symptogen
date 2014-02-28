#include "EntityManager.h"
#include "sound/SoundManager.h"
#include "power/Sneeze.h"
#include "power/Fever.h"

namespace Symp {

EntityManager::EntityManager() {
	m_pEntity2dManager = new IND_Entity2dManager();
 	m_pPhysicalWorld = new PhysicalWorld();

 	//Init powers
 	Sneeze* pSneeze = new Sneeze();
 	pSneeze->setRepulsionStrength(500);
 	pSneeze->setTimeToTriggerRandomSneeze(5);
 	m_powerArray.push_back(pSneeze);

 	Fever* pFever = new Fever();
 	m_powerArray.push_back(pFever);
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
		if(renderEntityArray[indexRenderEntity] != NULL)
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
	
	// Update Render Entities
	for(size_t i = 0; i < m_renderEntityArray.size(); i++) {
		std::vector<RenderEntity*> rEntities = m_renderEntityArray.at(i);
		PhysicalEntity* pEntity = m_physicalEntityArray.at(i);
		if((rEntities.size() > 0) && (pEntity != nullptr)) {
			for(size_t indexRenderEntity = 0; indexRenderEntity < rEntities.size(); ++indexRenderEntity){
				if(rEntities[indexRenderEntity] != NULL)
					rEntities[indexRenderEntity]->setPosition(pEntity->getPosition().x, pEntity->getPosition().y);
			}
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

	// Stop
	RenderEntity* rEntity1 = new RenderEntity("../assets/surface/dino/dinoStop.png", Symp::Surface);
	float scaleFactor = (float)doorHeight / (float)rEntity1->getHeight();
	rEntity1->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntity1->setHotSpot(0.5, 0.5);
	rEntity1->setShow(true);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::Stop, rEntity1);

	// WalkRight
	RenderEntity* rEntity2 = new RenderEntity("../assets/animation/WalkRight.xml", Symp::Animation);
	rEntity2->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntity2->setHotSpot(0.5, 0.5);
	rEntity2->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::WalkRight, rEntity2);

	// WalkLeft
	RenderEntity* rEntityLeft = new RenderEntity("../assets/animation/WalkRight.xml", Symp::Animation);
	rEntityLeft->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntityLeft->setHotSpot(0.5, 0.5);
	rEntityLeft->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::WalkLeft, rEntityLeft);


	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::Jump, NULL);

	// Sneeze
	RenderEntity* rEntitySneeze = new RenderEntity("../assets/animation/Sneeze.xml", Symp::Animation);
	rEntitySneeze->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntitySneeze->setHotSpot(0.5, 0.5);
	rEntitySneeze->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::Sneezing, rEntitySneeze);

	// die by fall
	RenderEntity* rEntity3 = new RenderEntity("../assets/animation/Die.xml", Symp::Animation);
	rEntity3->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntity3->setHotSpot(0.5, 0.5);
	rEntity3->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::DieByFall, rEntity3);

	// Death by spikes
	RenderEntity* rEntity4 = new RenderEntity("../assets/animation/Die.xml", Symp::Animation);
	rEntity4->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntity4->setHotSpot(0.5, 0.5);
	rEntity4->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::DieBySpikes, rEntity4);

	// Death by freeze
	RenderEntity* rEntity5 = new RenderEntity("../assets/animation/Die.xml", Symp::Animation);
	rEntity5->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntity5->setHotSpot(0.5, 0.5);
	rEntity5->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::DieByFreeze, rEntity5);

	// Death by hot
	RenderEntity* rEntity6 = new RenderEntity("../assets/animation/Die.xml", Symp::Animation);
	rEntity6->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntity6->setHotSpot(0.5, 0.5);
	rEntity6->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::DieByHot, rEntity6);
	
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
	
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::Stop, NULL);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::WalkRight, NULL);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::WalkLeft, NULL);

	// Jump
	size_t indexSound3 = SoundManager::getInstance()->loadSound("../assets/audio/jump.ogg");
	SoundEntity* sEntity3 = new SoundEntity(indexSound3);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::Jump, sEntity3);

	// Sneeze
	size_t indexSound4 = SoundManager::getInstance()->loadSound("../assets/audio/sneeze.ogg");
	SoundEntity* sEntity4 = new SoundEntity(indexSound4);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::Sneezing, sEntity4);

	// Death by fall
	size_t indexSound5 = SoundManager::getInstance()->loadSound("../assets/audio/death-fall.ogg");
	SoundEntity* sEntity5 = new SoundEntity(indexSound5);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::DieByFall, sEntity5);

	// Death by spikes
	size_t indexSound6 = SoundManager::getInstance()->loadSound("../assets/audio/death-spikes.ogg");
	SoundEntity* sEntity6 = new SoundEntity(indexSound6);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::DieBySpikes, sEntity6);

	// Death by freeze
	size_t indexSound7 = SoundManager::getInstance()->loadSound("../assets/audio/death-freeze.ogg");
	SoundEntity* sEntity7 = new SoundEntity(indexSound7);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::DieByFreeze, sEntity7);

	// Death by Hot
	size_t indexSound8 = SoundManager::getInstance()->loadSound("../assets/audio/death-freeze.ogg");
	SoundEntity* sEntity8 = new SoundEntity(indexSound8);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::DieByHot, sEntity8);

	/*****************/
	/*   Add Dino    */
	/*****************/
	m_uiDinoIndex = getNbEntities();
	addEntity(renderEntityArray, 63, pEntity, soundEntityArray);

}

void EntityManager::updateDinoRender(DinoAction dinoAction) const {
	for(size_t indexRenderDino = 0; indexRenderDino < getRenderDino().size(); ++indexRenderDino){
		if(getRenderDino()[indexRenderDino] != NULL)
			getRenderDino()[indexRenderDino]->setShow(false);
		if(getRenderDino()[indexRenderDino] != NULL && indexRenderDino == static_cast<size_t>(dinoAction))
			getRenderDino().at(dinoAction)->setShow(true);
	}
}

void EntityManager::killDino(DinoAction action) {
	// Animation
	updateDinoRender(action);
	
	// Play sound
	SoundManager::getInstance()->play(getSoundDino()[action]->getIndexSound());

	// Launch level
}

void EntityManager::executePowers() {
	for(size_t i = 0; i < m_powerArray.size(); ++i){
		m_powerArray[i]->execute();
	}
}

DinoAction EntityManager::getCurrentDinoAction() const {
	size_t indexCurrentDino = 0;
	for(size_t indexRenderDino = 0; indexRenderDino < getRenderDino().size(); ++indexRenderDino){
		if(getRenderDino()[indexRenderDino] != NULL) {
			if(getRenderDino()[indexRenderDino]->isShow()){
				indexCurrentDino = indexRenderDino;
				break;
			}
		}
	}
	return static_cast<DinoAction>(indexCurrentDino);
}

// Initialize singleton
IND_Entity2dManager*	EntityManager::m_pEntity2dManager = NULL;
}