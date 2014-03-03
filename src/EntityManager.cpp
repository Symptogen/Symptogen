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
/* Manage entities */
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
	if(isPowerExisting(PowerType::FeverType)) {
		updateThermomether();
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
/* Manage powers */
/********************************************************************************/

void EntityManager::addPower(Power* newPower) {
m_powerArray.push_back(newPower);
}

void EntityManager::executePowers() {
	for(size_t i = 0; i < m_powerArray.size(); ++i){
		m_powerArray[i]->execute();
		m_powerArray[i]->updatePowerTimer();
	}
}

void EntityManager::deleteAllPowers() {
	m_powerArray.clear();
}

/************************************************************************************/
/* Manage specific entities */
/************************************************************************************/

void EntityManager::addDino(int posX, int posY, int dinoWidth) {
	/*****************/
	/* Render */
	/*****************/
	std::vector<RenderEntity*> renderEntityArray;

	// Normal Stop
	RenderEntity* rEntityNormalStop = new RenderEntity("../assets/surface/dino/dinoStop.png", Symp::Surface);
	float scaleFactor = (float)dinoWidth / (float)rEntityNormalStop->getWidth();

	rEntityNormalStop->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntityNormalStop->setHotSpot(0.5, 0.5);
	rEntityNormalStop->setShow(true);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::NormalStop, rEntityNormalStop);

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

	// Fever
	RenderEntity* rEntityFever = new RenderEntity("../assets/animation/Fever.xml", Symp::Animation);
	rEntityFever->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntityFever->setHotSpot(0.5, 0.5);
	rEntityFever->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::HotFever, rEntityFever);

	// Stop Fever
	RenderEntity* rEntityFeverStop = new RenderEntity("../assets/surface/dino/dinoFeverStop.png", Symp::Surface);
	rEntityFeverStop->setScale(scaleFactor, scaleFactor);
	// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
	rEntityFeverStop->setHotSpot(0.5, 0.5);
	rEntityFeverStop->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::FeverStop, rEntityFeverStop);

	/*****************/
	/* Physical */
	/*****************/
	  float width = rEntityNormalStop->getWidth();
	  float height = rEntityNormalStop->getHeight();

	  PhysicalEntity* pEntity = new PhysicalEntity(
	  m_pPhysicalWorld->getWorld(),
	  b2Vec2(posX, posY),
	  b2Vec2(width, height),
	  PhysicalType::Dino
	  );
	  pEntity->setMass(50.f, 0.f);
	 
	/*****************/
	/* Sound */
	/*****************/
	std::vector<SoundEntity*> soundEntityArray;

	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::NormalStop, NULL);
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
	/* Add Dino */
	/*****************/
	m_dinoIndex = getNbEntities();
	addEntity(renderEntityArray, 63, pEntity, soundEntityArray);
}

void EntityManager::killDino(DinoAction action) {
	if(!getRenderDino().at(action)->isAnimationPlaying()){
		SoundManager::getInstance()->play(getSoundDino()[action]->getIndexSound());
	}
	setDinoRender(action);
}

void EntityManager::addThermometer() {
	/*****************/
	/* Render */
	/*****************/

	std::vector<RenderEntity*> supportArray;
	RenderEntity* support = new RenderEntity("../assets/surface/thermometer/thermometer.png", Symp::Surface);
	support->setScale(0.5, 0.5);
	support->setShow(true);
	supportArray.push_back(support);

	std::vector<RenderEntity*> tempArray;
	RenderEntity* temperature = new RenderEntity("../assets/surface/thermometer/temperature.png", Symp::Surface);
	temperature->setHotSpot(0.5, 0.5);
	temperature->setScale(support->getWidth(), 1);
	temperature->setShow(true);
	tempArray.push_back(temperature);

	/************************/
	/* Add Thermometer */
	/************************/

	m_thermometerTemperatureIndex = getNbEntities();
	addRenderEntity(tempArray, 63);

	m_thermometerSupportIndex = getNbEntities();
	addRenderEntity(supportArray, 63);

	
}

/************************************************************************************/
/* Getters & Setters */
/************************************************************************************/

bool EntityManager::isPowerExisting(PowerType powerType) const{
	if(powerType == PowerType::SneezeType)
		return (m_powerArray.size() >= 1) ? true : false;
	else if(powerType == PowerType::FeverType)
		return (m_powerArray.size() >= 2) ? true : false;
	else if(powerType == PowerType::HeadacheType)
		return (m_powerArray.size() >= 3) ? true : false;
	else
		return false;
}

DinoAction EntityManager::getCurrentDinoAction() const {
	if(!getRenderDino().at(DinoAction::Die)->isAnimationPlaying()){
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
	else return DinoAction::Die;
}

void EntityManager::setDinoRender(DinoAction dinoAction) {
	if(!getRenderDino().at(DinoAction::Die)->isAnimationPlaying()){
		// Flip to the left all render entities
		for(size_t i = 0; i < EntityManager::getInstance()->getRenderDino().size(); ++i) {
			if(EntityManager::getInstance()->getRenderDino().at(i) != NULL){
				if(getPhysicalDino()->getLinearVelocity().x < 0)
					EntityManager::getInstance()->getRenderDino().at(i)->flipHorizontaly(true);
				else if(getPhysicalDino()->getLinearVelocity().x > 0)
					EntityManager::getInstance()->getRenderDino().at(i)->flipHorizontaly(false);
			}
		}
		// Set visible the correct render entity
		for(size_t indexRenderDino = 0; indexRenderDino < getRenderDino().size(); ++indexRenderDino){
			if(getRenderDino()[indexRenderDino] != NULL)
				getRenderDino()[indexRenderDino]->setShow(false);
			if(getRenderDino()[indexRenderDino] != NULL && indexRenderDino == static_cast<size_t>(dinoAction)){
				getRenderDino().at(dinoAction)->setShow(true);
				if(dinoAction == DinoAction::Die) getRenderDino().at(dinoAction)->playDeathAnimation();
			}
		}
	}
	else if(!getRenderDino().at(DinoAction::Die)->isAnimationFinish())
			getRenderDino().at(DinoAction::Die)->playDeathAnimation();
			
}

void EntityManager::updateThermomether() {

	// Get data
	int currentTemp = static_cast<Fever*>(m_powerArray[1])->getCurrentTemperature();
	int maxTemp = static_cast<Fever*>(m_powerArray[1])->getMaxTemperature();
	int minTemp = static_cast<Fever*>(m_powerArray[1])->getMinTemperature();
	std::vector<RenderEntity*> tempRenderEntities = getRenderEntity(m_thermometerTemperatureIndex);
	std::vector<RenderEntity*> supportRenderEntities = getRenderEntity(m_thermometerSupportIndex);
	
	// Set the temperature entity height
	float totalHeight = maxTemp - minTemp;
	float ratio = (currentTemp + maxTemp) / totalHeight;
	float maxScale = supportRenderEntities.at(0)->getHeight();
	float currentScaleFactor = ratio * maxScale;
	tempRenderEntities.at(0)->setScale(supportRenderEntities.at(0)->getWidth(), currentScaleFactor);

	// Set the thermometer color
	float colorRed = currentTemp > 0 ? (float)currentTemp/(float)maxTemp : 0;
	float colorGreen = currentTemp > 0 ? 1 - (float)currentTemp/(float)maxTemp : 1 - (float)currentTemp/(float)minTemp;
	float colorBlue = currentTemp < 0 ? (float)currentTemp/(float)minTemp : 0;
	tempRenderEntities.at(0)->setTint(255*colorRed, 255*colorGreen, 255*colorBlue);

	// Set the position of the thermomether to follow the Dino on the screen
	float leftMargin = 10;
	float posX = getRenderDino()[0]->getPosX() - 200 + leftMargin;
	float posY = getRenderDino()[0]->getPosY() - 140 ;

	tempRenderEntities.at(0)->setAngleXYZ(0, 0, 180);
	tempRenderEntities.at(0)->setPosition(posX + supportRenderEntities.at(0)->getWidth(), posY + supportRenderEntities.at(0)->getHeight());
	supportRenderEntities.at(0)->setPosition(posX, posY);

}

PowerType EntityManager::getCurrentPowerState() const{
	if(isPowerExisting(PowerType::FeverType) && static_cast<Fever*>(m_powerArray[1])->getThermometerStep() >= 6)
		return PowerType::FeverType;
	else
		return PowerType::NormalType;
}

bool EntityManager::isDinoAllowToMove(){
	if(!getPower(PowerType::SneezeType)->isActivated() && !getRenderDino().at(DinoAction::Die)->isAnimationPlaying())
		return true;
	else return false;
}

}