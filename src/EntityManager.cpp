#include "EntityManager.h"
#include "sound/SoundManager.h"
#include "power/Fever.h"

namespace Symp {

EntityManager::EntityManager() {
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
	m_pEntity2dManager = new IND_Entity2dManager();
 	m_pPhysicalWorld = new PhysicalWorld();
 	
	m_pEntity2dManager->init(pRender->getIND_Render());
	RenderEntity::init(pRender);
}

/************************************************************************************/
/*							Manage entities 										*/
/************************************************************************************/

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
	
	// Update Render Entities which correspond to Physical Entities
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

	// Update Thermometer
	if(isPowerExisting(PowerType::FeverType)){
		std::vector<RenderEntity*> renderEntities = getRenderEntity(m_thermometerIndex);
		float posX = getRenderDino()[0]->getPosX() - 200;
		float posY = getRenderDino()[0]->getPosY() - 200;
		for(size_t indexRenderEntity = 0; indexRenderEntity < renderEntities.size(); ++indexRenderEntity){
			if(renderEntities[indexRenderEntity] != NULL)
				renderEntities[indexRenderEntity]->setPosition(posX, posY);
		}
		setThermometerRender();
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

/********************************************************************************/
/*							Manage powers 										*/
/********************************************************************************/

void EntityManager::addPower(Power* newPower) {
	m_powerArray.push_back(newPower);
}

void EntityManager::executePowers() {
	for(size_t i = 0; i < m_powerArray.size(); ++i){
		m_powerArray[i]->execute();
	}
}

void EntityManager::deleteAllPowers() {
	m_powerArray.clear();
}

/************************************************************************************/
/*							Manage specific entities 								*/
/************************************************************************************/

void EntityManager::addDino(int posX, int posY, int dinoWidth) {
	/*****************/
	/*    Render     */
	/*****************/
	std::vector<RenderEntity*> renderEntityArray;

	// Stop
	RenderEntity* rEntity1 = new RenderEntity("../assets/surface/dino/dinoStop.png", Symp::Surface);
	float scaleFactor = (float)dinoWidth / (float)rEntity1->getWidth();

	rEntity1->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntity1->setHotSpot(0.5, 0.5);
	rEntity1->setShow(true);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::Stop, rEntity1);

	// Walk
	RenderEntity* rEntity2 = new RenderEntity("../assets/animation/WalkRight.xml", Symp::Animation);
	rEntity2->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntity2->setHotSpot(0.5, 0.5);
	rEntity2->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::Walk, rEntity2);

	// Jump
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::Jump, NULL);

	// die
	RenderEntity* rEntity3 = new RenderEntity("../assets/animation/Die.xml", Symp::Animation);
	rEntity3->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntity3->setHotSpot(0.5, 0.5);
	rEntity3->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::Die, rEntity3);
	
	// Sneeze
	RenderEntity* rEntitySneeze = new RenderEntity("../assets/animation/Sneeze.xml", Symp::Animation);
	rEntitySneeze->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntitySneeze->setHotSpot(0.5, 0.5);
	rEntitySneeze->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::Sneezing, rEntitySneeze);
	
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
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::Walk, NULL);

	// Jump
	size_t indexSound3 = SoundManager::getInstance()->loadSound("../assets/audio/jump.ogg");
	SoundEntity* sEntity3 = new SoundEntity(indexSound3);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::Jump, sEntity3);

	// Death
	size_t indexSound5 = SoundManager::getInstance()->loadSound("../assets/audio/death.ogg");
	SoundEntity* sEntity5 = new SoundEntity(indexSound5);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::Die, sEntity5);

	// Sneeze
	size_t indexSound4 = SoundManager::getInstance()->loadSound("../assets/audio/sneeze.ogg");
	SoundEntity* sEntity4 = new SoundEntity(indexSound4);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::Sneezing, sEntity4);

	/*****************/
	/*   Add Dino    */
	/*****************/
	m_dinoIndex = getNbEntities();
	addEntity(renderEntityArray, 63, pEntity, soundEntityArray);
}

void EntityManager::killDino(DinoAction action) {
	// Animation
	setDinoRender(action);
	
	// Play sound
	SoundManager::getInstance()->play(getSoundDino()[action]->getIndexSound());

}

void EntityManager::addThermometer() {
	/*****************/
	/*    Render     */
	/*****************/
	std::vector<RenderEntity*> renderEntityArray;

	// 0
	RenderEntity* rEntity0 = new RenderEntity("../assets/surface/thermometer/thermometer_0.png", Symp::Surface);
	rEntity0->setScale(0.2, 0.2);
	rEntity0->setShow(true);
	renderEntityArray.push_back(rEntity0);

	// 1
	RenderEntity* rEntity1 = new RenderEntity("../assets/surface/thermometer/thermometer_1.png", Symp::Surface);
	rEntity1->setScale(0.2, 0.2);
	rEntity1->setShow(false);
	renderEntityArray.push_back(rEntity1);

	// 2
	RenderEntity* rEntity2 = new RenderEntity("../assets/surface/thermometer/thermometer_2.png", Symp::Surface);
	rEntity2->setScale(0.2, 0.2);
	rEntity2->setShow(false);
	renderEntityArray.push_back(rEntity2);

	// 3
	RenderEntity* rEntity3 = new RenderEntity("../assets/surface/thermometer/thermometer_3.png", Symp::Surface);
	rEntity3->setScale(0.2, 0.2);
	rEntity3->setShow(false);
	renderEntityArray.push_back(rEntity3);

	// 4
	RenderEntity* rEntity4 = new RenderEntity("../assets/surface/thermometer/thermometer_4.png", Symp::Surface);
	rEntity4->setScale(0.2, 0.2);
	rEntity4->setShow(false);
	renderEntityArray.push_back(rEntity4);

	// 5
	RenderEntity* rEntity5 = new RenderEntity("../assets/surface/thermometer/thermometer_5.png", Symp::Surface);
	rEntity5->setScale(0.2, 0.2);
	rEntity5->setShow(false);
	renderEntityArray.push_back(rEntity5);

	// 6
	RenderEntity* rEntity6 = new RenderEntity("../assets/surface/thermometer/thermometer_6.png", Symp::Surface);
	rEntity6->setScale(0.2, 0.2);
	rEntity6->setShow(false);
	renderEntityArray.push_back(rEntity6);

	// 7
	RenderEntity* rEntity7 = new RenderEntity("../assets/surface/thermometer/thermometer_7.png", Symp::Surface);
	rEntity7->setScale(0.2, 0.2);
	rEntity7->setShow(false);
	renderEntityArray.push_back(rEntity7);
	
	/************************/
	/*   Add Thermometer    */
	/************************/
	m_thermometerIndex = getNbEntities();
	addRenderEntity(renderEntityArray, 63);
}

/************************************************************************************/
/*							Getters & Setters 										*/
/************************************************************************************/

bool EntityManager::isPowerExisting(PowerType powerType) const{
	if(powerType == PowerType::SneezeType)
		return (m_powerArray.size() == 1) ? true : false;
	else if(powerType == PowerType::FeverType)
		return (m_powerArray.size() == 2) ? true : false;
	else if(powerType == PowerType::HeadacheType)
		return (m_powerArray.size() == 3) ? true : false;
	else
		return true;
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

void EntityManager::setDinoRender(DinoAction dinoAction) {
	for(size_t indexRenderDino = 0; indexRenderDino < getRenderDino().size(); ++indexRenderDino){
		if(getRenderDino()[indexRenderDino] != NULL)
			getRenderDino()[indexRenderDino]->setShow(false);
		if(getRenderDino()[indexRenderDino] != NULL && indexRenderDino == static_cast<size_t>(dinoAction))
			getRenderDino().at(dinoAction)->setShow(true);
	}
}

void EntityManager::setThermometerRender() {
	for(size_t indexRenderTher = 0; indexRenderTher < getRenderEntity(m_thermometerIndex).size(); ++indexRenderTher){
		if(getRenderEntity(m_thermometerIndex)[indexRenderTher] != NULL)
			getRenderEntity(m_thermometerIndex)[indexRenderTher]->setShow(false);
	}
	size_t indexThermometer = dynamic_cast<Fever*>(getPower(PowerType::FeverType))->getThermometerStep();
	if(getRenderEntity(m_thermometerIndex)[indexThermometer] != NULL)
		getRenderEntity(m_thermometerIndex).at(indexThermometer)->setShow(true);
}

}