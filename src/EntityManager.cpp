#include "EntityManager.h"
#include "sound/SoundManager.h"
#include "power/Fever.h"
#include "power/Sneeze.h"
#include <stdexcept>

#include <ctime>

namespace Symp {

EntityManager::EntityManager() {
	m_thermometerSupportIndex = -1;
	m_thermometerTemperatureIndex = -1;
	m_bIsDinoShivering = false;}

EntityManager::~EntityManager(){
	m_pEntity2dManager->end();
	DISPOSE(m_pEntity2dManager);
	RenderEntity::end();
	deleteAllEntities();
}

void EntityManager::initRender(Render* pRender) {
	m_pEntity2dManager = new IND_Entity2dManager();
 	m_pPhysicalWorld = new PhysicalWorld();
	m_pEntity2dManager->init(pRender->getIND_Render());
	RenderEntity::init(pRender);
}

void EntityManager::destroyRender() {
	delete m_pPhysicalWorld;
}

/************************************************************************************/
/* Manage entities */
/************************************************************************************/

bool EntityManager::addRenderEntity(std::vector<RenderEntity*> renderEntityArray, unsigned int layer) {
	m_renderEntityArray.push_back(renderEntityArray);
	m_physicalEntityArray.push_back(NULL);
	bool check = false;
	for(size_t indexRenderEntity = 0; indexRenderEntity < renderEntityArray.size(); ++indexRenderEntity)
		check = m_pEntity2dManager->add(layer, renderEntityArray[indexRenderEntity]->getIND_Entity2d());
	return check;
}

bool EntityManager::addPhysicalEntity(PhysicalEntity* pPhysicalEntity) {
	m_renderEntityArray.push_back(std::vector<RenderEntity*>());
	m_physicalEntityArray.push_back(pPhysicalEntity);
	return true;
}

bool EntityManager::addEntity(std::vector<RenderEntity*> renderEntityArray, unsigned int layer, PhysicalEntity* pPhysicalEntity) {
	m_renderEntityArray.push_back(renderEntityArray);
	m_physicalEntityArray.push_back(pPhysicalEntity);
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

void EntityManager::renderEntities() {
	for(unsigned int layer = 0; layer < 64; ++layer)
		m_pEntity2dManager->renderEntities2d(layer);
}

void EntityManager::updateEntities() {

	// Delete entities which has to be destroyed
	std::vector<std::vector<RenderEntity*>>::iterator itRender = m_renderEntityArray.begin();

	for(std::vector<PhysicalEntity*>::iterator itPhysical = m_physicalEntityArray.begin(); itPhysical != m_physicalEntityArray.end();) {

		// If PhysicalEntity hasToBeDestroyed
		if((*itPhysical) != nullptr && (*itPhysical)->hasToBeDestroyed()){
			for(size_t i = 0; i < (*itRender).size(); ++i){
				if((*itRender)[i] != nullptr)
					m_pEntity2dManager->remove((*itRender)[i]->getIND_Entity2d());
			}
			itRender = m_renderEntityArray.erase(itRender);
			m_pPhysicalWorld->destroyPhysicalBody((*itPhysical));
			itPhysical = m_physicalEntityArray.erase(itPhysical);
		}

		// If a DestructibleObject has a RenderEntity with an animation finished
		else if((*itPhysical) != nullptr && (*itPhysical)->getType() == PhysicalType::DestructibleObject && (*itRender).size() > 0){
			for(size_t i = 0; i < (*itRender).size(); ++i){
				if((*itRender)[i] != nullptr && (*itRender)[i]->isAnimationFinish()){
					(*itPhysical)->hasToBeDestroyed(true);
				}
			}
			++itRender;
			++itPhysical;
		}
		else{
			++itRender;
			++itPhysical;
		}
	}

	// Shivering
	if(isPowerExisting(PowerType::FeverType)) {
		if(getIsDinoShivering()) {
			static_cast<Fever*>(m_powerArray.at(PowerType::FeverType))->shiverBackground();
		}
	}


	// Update Physical entities
	m_pPhysicalWorld->updatePhysics();
	if(EntityManager::getInstance()->isPowerExisting(PowerType::SneezeType)) {
		PhysicalEntity::checkMovableObject(EntityManager::getInstance()->getPower(PowerType::SneezeType)->isActivated());
	 }
	

	// Update Render Entities which correspond to Physical Entities
	for(size_t i = 0; i < m_renderEntityArray.size(); i++) {
		std::vector<RenderEntity*> rEntities = m_renderEntityArray.at(i);
		PhysicalEntity* pEntity = m_physicalEntityArray.at(i);
		if((rEntities.size() > 0) && (pEntity != nullptr)) {
			for(size_t indexRenderEntity = 0; indexRenderEntity < rEntities.size(); ++indexRenderEntity){
				if(rEntities[indexRenderEntity] != nullptr){
					rEntities[indexRenderEntity]->setPosition(pEntity->getPosition().x, pEntity->getPosition().y);
				}
			}
		}
	}

	// Update specific elements when fever
	if(isPowerExisting(PowerType::FeverType)) {
		updateThermometherRender();
		updateFlames();
		updateDestructibleObjects();
	}
}

void EntityManager::deleteAllEntities() {

	RenderEntity::s_surfaceMap.clear();
	RenderEntity::s_animationMap.clear();
	PhysicalEntity::s_verticesMap.clear();

	// Delete physicalEntityArray
	for(size_t t = 0; t < m_physicalEntityArray.size(); t++) {
		if(m_physicalEntityArray.at(t) != nullptr) {
			delete m_physicalEntityArray.at(t);
		}
	}
	m_physicalEntityArray.clear();

	// Delete renderEntityArray
	for(size_t t = 0; t < m_renderEntityArray.size(); t++) {
		for(size_t tt = 0; tt < m_renderEntityArray.at(t).size(); tt++) {
			if(m_renderEntityArray.at(t).at(tt) != nullptr) {
				m_pEntity2dManager->remove(m_renderEntityArray.at(t).at(tt)->getIND_Entity2d());
				delete m_renderEntityArray.at(t).at(tt);
			}
		}
		m_renderEntityArray.at(t).clear();
	}
	m_renderEntityArray.clear();
}

bool EntityManager::deleteEntity(size_t indexEntity) {
	size_t indexCurrent = 0;

	std::vector<std::vector<RenderEntity*>>::iterator itRender = m_renderEntityArray.begin();

	for(std::vector<PhysicalEntity*>::iterator itPhysical = m_physicalEntityArray.begin(); itPhysical != m_physicalEntityArray.end();){
		if(indexCurrent == indexEntity){

			for(size_t i = 0; i < (*itRender).size(); ++i){
				m_pEntity2dManager->remove((*itRender)[i]->getIND_Entity2d());
			}
			
			for(std::vector<RenderEntity*>::iterator it = (*itRender).begin(); it != (*itRender).end(); ++it) {
				delete *it;
			}
			itRender = m_renderEntityArray.erase(itRender);


			m_pPhysicalWorld->destroyPhysicalBody(m_physicalEntityArray.at(indexCurrent));
			
			delete *itPhysical;
			itPhysical = m_physicalEntityArray.erase(itPhysical);

			return true;
		}

		else{
			itRender++;
			itPhysical++;
			indexCurrent++;
		}
	}
	return false;
}

/********************************************************************************/
/* Manage powers */
/********************************************************************************/

void EntityManager::addPower(Power* newPower, PowerType type) {
	m_powerArray.insert(m_powerArray.begin() + type, newPower);
}

void EntityManager::executePowers() {
	for(size_t i = 0; i < m_powerArray.size(); ++i){
		if(m_powerArray.at(i) != nullptr)
			m_powerArray.at(i)->execute();
	}
}

void EntityManager::deleteAllPowers() {
	m_powerArray.clear();

	// Initialize Power array
	m_powerArray.push_back(nullptr);
	m_powerArray.push_back(nullptr);
	m_powerArray.push_back(nullptr);
}

/************************************************************************************/
/* Manage specific entities */
/************************************************************************************/

void EntityManager::addDino(int posX, int posY, int dinoWidth) {
	/**********/
	/* Render */
	/**********/
	std::vector<RenderEntity*> renderEntityArray;

	// Normal Stop
	RenderEntity* rEntityNormalStop = new RenderEntity("../assets/surface/dino/dinoStop.png", Symp::Surface);
	float scaleFactor = (float)dinoWidth / (float)rEntityNormalStop->getWidth();

	rEntityNormalStop->setScale(scaleFactor, scaleFactor);
	rEntityNormalStop->setHotSpot(0.5, 0.5);
	rEntityNormalStop->setShow(true);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::StopNormal, rEntityNormalStop);

	// Stop Fever
	RenderEntity* rEntityFeverStop = new RenderEntity("../assets/surface/dino/dinoFeverStop.png", Symp::Surface);
	rEntityFeverStop->setScale(scaleFactor, scaleFactor);
	rEntityFeverStop->setHotSpot(0.5, 0.5);
	rEntityFeverStop->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::StopFever, rEntityFeverStop);

	// Stop Hypothermia
	RenderEntity* rEntityStopHypothermia = new RenderEntity("../assets/surface/dino/dinoStopHypothermia.png", Symp::Surface);
	rEntityStopHypothermia->setScale(scaleFactor, scaleFactor);
	rEntityStopHypothermia->setHotSpot(0.5, 0.5);
	rEntityStopHypothermia->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::StopHypothermia, rEntityStopHypothermia);

	// Stop Shivering
	RenderEntity* rEntityStopShivering = new RenderEntity("../assets/animation/Shivering/Shivering.xml", Symp::Animation);
	rEntityStopShivering->setScale(scaleFactor, scaleFactor);
	rEntityStopShivering->setHotSpot(0.5, 0.5);
	rEntityStopShivering->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::StopShivering, rEntityStopShivering);

	// Walk Normal
	RenderEntity* rEntity2 = new RenderEntity("../assets/animation/WalkRight/WalkRightNormal/WalkRightNormal.xml", Symp::Animation);
	rEntity2->setScale(scaleFactor, scaleFactor);
	rEntity2->setHotSpot(0.5, 0.5);
	rEntity2->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::WalkNormal, rEntity2);

	// Walk Fever
	RenderEntity* rEntityFever = new RenderEntity("../assets/animation/WalkRight/WalkRightFever/WalkRightFever.xml", Symp::Animation);
	rEntityFever->setScale(scaleFactor, scaleFactor);
	rEntityFever->setHotSpot(0.5, 0.5);
	rEntityFever->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::WalkFever, rEntityFever);

	// Walk Hypothermia
	RenderEntity* rEntityWalkHypothermia = new RenderEntity("../assets/animation/WalkRight/WalkRightHypothermia/WalkRightHypothermia.xml", Symp::Animation);
	rEntityWalkHypothermia->setScale(scaleFactor, scaleFactor);
	rEntityWalkHypothermia->setHotSpot(0.5, 0.5);
	rEntityWalkHypothermia->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::WalkHypothermia, rEntityWalkHypothermia);

	// Walk Shivering
	RenderEntity* rEntityWalkShivering = new RenderEntity("../assets/animation/WalkRight/WalkRightShivering/WalkRightShivering.xml",  Symp::Animation);
	rEntityWalkShivering->setScale(scaleFactor, scaleFactor);
	rEntityWalkShivering->setHotSpot(0.5, 0.5);
	rEntityWalkShivering->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::WalkShivering, rEntityWalkShivering);


	// Jump
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::Jump, NULL);

	// Normal Death
	RenderEntity* rEntity3 = new RenderEntity("../assets/animation/Death/NormalDeath/NormalDeath.xml", Symp::Animation);
	rEntity3->setScale(scaleFactor, scaleFactor);
	rEntity3->setHotSpot(0.5, 0.5);
	rEntity3->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::DeathNormal, rEntity3);

	// Fever Death
	RenderEntity* rEntityFeverDeath = new RenderEntity("../assets/animation/Death/FeverDeath/FeverDeath.xml", Symp::Animation);
	rEntityFeverDeath->setScale(scaleFactor, scaleFactor);
	rEntityFeverDeath->setHotSpot(0.5, 0.5);
	rEntityFeverDeath->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::DeathFever, rEntityFeverDeath);

	// Hypothermia Death
	RenderEntity* rEntityHypothermiaDeath = new RenderEntity("../assets/animation/Death/HypothermiaDeath/HypothermiaDeath.xml", Symp::Animation);
	rEntityHypothermiaDeath->setScale(scaleFactor, scaleFactor);
	rEntityHypothermiaDeath->setHotSpot(0.5, 0.5);
	rEntityHypothermiaDeath->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::DeathHypothermia, rEntityHypothermiaDeath);

	// Shivering death

	// Sneeze
	RenderEntity* rEntitySneeze = new RenderEntity("../assets/animation/Sneeze/NormalSneeze/NormalSneeze.xml", Symp::Animation);
	rEntitySneeze->setScale(scaleFactor, scaleFactor);
	rEntitySneeze->setHotSpot(0.5, 0.5);
	rEntitySneeze->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::Sneezing, rEntitySneeze);

	// Fever  Sneeze
	RenderEntity* rEntityFeverSneeze = new RenderEntity("../assets/animation/Sneeze/FeverSneeze/FeverSneeze.xml", Symp::Animation);
	rEntityFeverSneeze->setScale(scaleFactor, scaleFactor);
	rEntityFeverSneeze->setHotSpot(0.5, 0.5);
	rEntityFeverSneeze->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::FeverSneezing, rEntityFeverSneeze);

	// Cold Sneeze
	RenderEntity* rEntityColdSneeze = new RenderEntity("../assets/animation/Sneeze/HypothermiaSneeze/HypothermiaSneeze.xml", Symp::Animation);
	rEntityColdSneeze->setScale(scaleFactor, scaleFactor);
	rEntityColdSneeze->setHotSpot(0.5, 0.5);
	rEntityColdSneeze->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::ColdSneezing, rEntityColdSneeze);

	// HeadacheAction
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::HeadacheAction, NULL);


	/************/
	/* Physical */
	/************/
	float width = rEntityNormalStop->getWidth();
	float height = rEntityNormalStop->getHeight();

	PhysicalEntity* pEntity = new PhysicalEntity(
		m_pPhysicalWorld->getWorld(),
		b2Vec2(posX, posY),
		b2Vec2(width, height),
		PhysicalType::Dino
	);

	pEntity->setCustomChainHitbox("../assets/collision/dinoCollision.xml");

	/* Linear Damping       Max Speed
	0f                   120
	10f                  120
	50f                  120
	55f                  90
	60f                  0
	70f                  0
	100f                 0
	100000f              0 */
	pEntity->setLinearDamping(1.f);
	pEntity->setMass(50.f, 0.f);

	/*****************/
	/* Add Dino */
	/*****************/
	addEntity(renderEntityArray, 63, pEntity);
}

void EntityManager::killDino() {
	DinoAction deathType = getRightDeath();

	// If the animation is not playing : dino is not dead
	if(!isDeathAnimationPlaying()) {
		switch(deathType) {
			case DeathFever:
				SoundManager::getInstance()->playSound(SoundType::HOTFEVER_DEATH);
				break;
			case DeathHypothermia:
				SoundManager::getInstance()->playSound(SoundType::COLDFEVER_DEATH);
				break;
			case DeathNormal:
				SoundManager::getInstance()->playSound(SoundType::DEATH);
				break;
			default:
				break;
		}
		setDinoRender(deathType);
	}
}

void EntityManager::addThermometer() {
	std::vector<RenderEntity*> supportArray;
	RenderEntity* support = new RenderEntity("../assets/surface/thermometer/thermometer.png", Symp::Surface);
	support->setScale(0.1, 0.1);
	support->setShow(true);
	supportArray.push_back(support);

	std::vector<RenderEntity*> tempArray;
	RenderEntity* temperature = new RenderEntity("../assets/surface/thermometer/temperature.png", Symp::Surface);
	temperature->setHotSpot(0.5, 0.5);
	temperature->setScale(support->getWidth(), 1);
	temperature->setShow(true);
	tempArray.push_back(temperature);

	/*******************/
	/* Add Thermometer */
	/*******************/

	m_thermometerTemperatureIndex = getNbEntities();
	addRenderEntity(tempArray, 63);

	m_thermometerSupportIndex = getNbEntities();
	addRenderEntity(supportArray, 63);
}

void EntityManager::addFlames() {
	for(size_t indexEntity = 0; indexEntity < getPhysicalEntityArray().size(); ++indexEntity) {
		if(getPhysicalEntityArray().at(indexEntity) != nullptr){
			if(getPhysicalEntityArray().at(indexEntity)->getType() == PhysicalType::Flames){
				return;
			}
		}
	}

	/************/
	/*  Render  */
	/************/

	std::vector<RenderEntity*> renderFlamesArray;
	RenderEntity* flames1 = new RenderEntity("../assets/animation/Flames.xml", Symp::Animation);
	flames1->setHotSpot(0.5, 0.5);
	flames1->setScale(0.1, 0.1);
	flames1->setShow(true);
	renderFlamesArray.push_back(flames1);

	/************/
	/* Physical */
	/************/

	PhysicalEntity* pDino = getPhysicalDino();
	b2Vec2 pos;
	if(getRenderDino().at(DinoAction::StopNormal)->isFlippedHorizontaly()) {
		flames1->flipHorizontaly(true);
		pos = b2Vec2(pDino->getPosition().x - pDino->getWidth(), pDino->getPosition().y);
	}
	else{
		flames1->flipHorizontaly(false);
		pos = b2Vec2(pDino->getPosition().x + pDino->getWidth(), pDino->getPosition().y);
	}
	PhysicalEntity* physicalFlamesEntity = new PhysicalEntity(
		m_pPhysicalWorld->getWorld(),
		pos,
		b2Vec2(flames1->getWidth(), flames1->getHeight()),
		PhysicalType::Flames
	);
	physicalFlamesEntity->setMass(1.f, 0.f);

	/**************/
	/* Add Flames */
	/**************/

	addEntity(renderFlamesArray, 62, physicalFlamesEntity);
}

/************************************************************************************/
/* Getters & Setters */
/************************************************************************************/


std::vector<RenderEntity*> EntityManager::getRenderEntity(size_t index) const{
	try{
		return m_renderEntityArray.at(index);
	}
	catch(std::out_of_range& err){
		std::cerr << err.what() << " : Error when access std::vector<RenderEntity*> at index " << index << " in function EntityManager::getRenderEntity." << std::endl;
		return std::vector<RenderEntity*>();
	}
}

PhysicalEntity* EntityManager::getPhysicalEntity(size_t index) const {
	try{
		return m_physicalEntityArray.at(index);
	}
	catch(std::out_of_range& err){
		std::cerr << err.what() << " : Error when access PhysicalEntity* at index " << index << " in function EntityManager::getPhysicalEntity." << std::endl;
		return nullptr;
	}
}

std::vector<RenderEntity*> EntityManager::getRenderDino() const {
	try{
		for(size_t indexEntity = 0; indexEntity < getPhysicalEntityArray().size(); ++indexEntity) {
			if(getPhysicalEntityArray().at(indexEntity) != nullptr){
				if(getPhysicalEntityArray().at(indexEntity)->getType() == PhysicalType::Dino){
					return m_renderEntityArray.at(indexEntity);
				}
			}
		}
		return std::vector<RenderEntity*>();
	}
	catch(std::out_of_range& err){
		std::cerr << err.what() << " : Error when access vector<RenderEntity*> of dino in function EntityManager::getRenderDino." << std::endl;
		std::cerr << "The program will be aborted." << std::endl;
		exit(EXIT_FAILURE);
	}
}

PhysicalEntity* EntityManager::getPhysicalDino() const {
	try{
		for(size_t indexEntity = 0; indexEntity < getPhysicalEntityArray().size(); ++indexEntity) {
			if(getPhysicalEntityArray().at(indexEntity) != nullptr){
				if(getPhysicalEntityArray().at(indexEntity)->getType() == PhysicalType::Dino){
					return m_physicalEntityArray.at(indexEntity);
				}
			}
		}
		return nullptr;
	}
	catch(std::out_of_range& err){
		std::cerr << err.what() << " : Error when access PhysicalEntity* of dino at index in function EntityManager::getPhysicalDino." << std::endl;
		std::cerr << "The program will be aborted." << std::endl;
		exit(EXIT_FAILURE);
	}
}

Power* EntityManager::getPower(PowerType powerType) const {
	try{
		return m_powerArray.at(powerType);
	}
	catch(std::out_of_range& err){
		std::cerr << err.what() << " : Error when access the power with the PowerType " << powerType << " in function EntityManager::getPower." << std::endl;
		return nullptr;
	}
}

bool EntityManager::isPowerExisting(PowerType powerType) const{
	try{
		Power* pPower = m_powerArray.at(powerType);
		return (pPower != nullptr) ? true : false;
	}
	catch(std::out_of_range& err){
		return false;
	}
}


size_t EntityManager::getIndexEntity(PhysicalEntity* pPhysicalEntity) const {
	int count = 0;
	for(std::vector<PhysicalEntity*>::iterator it = getPhysicalEntityArray().begin(); it != EntityManager::getInstance()->getPhysicalEntityArray().end(); ++it) {
		if(*it == pPhysicalEntity) {
			return count;
		}
		count++;
	}
	return 0;
}

size_t EntityManager::getIndexEntityFromRenderEntity(std::vector<RenderEntity*> pRenderEntityArray) const {
	int count = 0;
	for(std::vector<std::vector<RenderEntity*>>::iterator it = getRenderEntityArray().begin(); it != EntityManager::getInstance()->getRenderEntityArray().end(); ++it) {
		if(*it == pRenderEntityArray) {
			return count;
		}
		count++;
	}
	return 0;
}

DinoAction EntityManager::getCurrentDinoAction() const {
	try{
		size_t indexCurrentDino = 0;
		for(size_t indexRenderDino = 0; indexRenderDino < getRenderDino().size(); ++indexRenderDino){
			if(getRenderDino().at(indexRenderDino) != nullptr) {
				if(getRenderDino().at(indexRenderDino)->isShow()){
					indexCurrentDino = indexRenderDino;
					break;
				}
			}
		}
		return static_cast<DinoAction>(indexCurrentDino);
	}
	catch(std::out_of_range& err){
		std::cerr << err.what() << " : Error when access a specific RenderEntity of dino in function EntityManager::getCurrentDinoAction." << std::endl;
		std::cerr << "The program will be aborted." << std::endl;
		exit(EXIT_FAILURE);
	}
}

PowerType EntityManager::getCurrentPowerType() const{
	try{
		if(isPowerExisting(PowerType::SneezeType) 
			&& (dynamic_cast<Sneeze*>(getPower(PowerType::SneezeType))->isWarningSneeze() || dynamic_cast<Sneeze*>(getPower(PowerType::SneezeType))->isSneezing()))
			return PowerType::SneezeType;
		else if(isPowerExisting(PowerType::FeverType))
			return PowerType::FeverType;
		else
			return PowerType::NormalType;
	}
	catch(std::bad_cast& err){
		std::cerr << err.what() << " : Cast error in function getCurrentPowerType()" << std::endl;
		return PowerType::NormalType;
	}
}

PowerState EntityManager::getCurrentPowerState() const{
	if(isPowerExisting(PowerType::FeverType)){
		Fever* feverPower = dynamic_cast<Fever*>(getPower(PowerType::FeverType));
		
		if(feverPower->isInSpitFireRange()) {
			return PowerState::SpitFireState;		
		}
		
		else if(feverPower->isInHotRange()) {
			return PowerState::HotFeverState;
		}
		
		else if(feverPower->isInShiveringRange()) {
			return PowerState::ShiveringState;
		}
		
		else if(feverPower->isInColdRange()) {
			return PowerState::HypothermiaState;
		}
	}
	return PowerState::None;
}


bool EntityManager::isDinoAllowToMove(){
	if(isPowerExisting(PowerType::SneezeType) && getPower(PowerType::SneezeType)->isActivated())
		return false;
	if(isDeathAnimationPlaying())
		return false;
	return true;
}

bool EntityManager::isDinoAllowToJump(){
	return (getPhysicalDino()->getLinearVelocity().y > -1 && getPhysicalDino()->getLinearVelocity().y < 1) ? true : false;
}

void EntityManager::setDinoRender(DinoAction dinoAction) {
	
	// Set visible the correct render entity
	for(size_t indexRenderDino = 0; indexRenderDino < getRenderDino().size(); ++indexRenderDino){
		if(getRenderDino()[indexRenderDino] != nullptr) {
			getRenderDino()[indexRenderDino]->setShow(false);
		}

		if(getRenderDino()[indexRenderDino] != nullptr && indexRenderDino == static_cast<size_t>(dinoAction)) {
			getRenderDino().at(dinoAction)->setShow(true);
			if(dinoAction == getRightDeath()) {
				getRenderDino().at(getRightDeath())->manageAnimationTimer(AnimationLength::DieLength);
			}
		}
	}
}

void EntityManager::flipDinoRenderEntities(bool flip) {

	for(size_t i = 0; i < getRenderDino().size(); ++i) {
		if(getRenderDino().at(i) != nullptr) {
			getRenderDino().at(i)->flipHorizontaly(flip);
		}
	}

}

void EntityManager::setFlowerRender(size_t index, FlowerAction action) {
	std::vector<RenderEntity*> renderFlowerArray = getRenderEntity(index);
	PhysicalEntity* physicalFlower = getPhysicalEntity(index);

	// Check that it is a flower
	if(physicalFlower->getType() == PhysicalType::Flower) {
		// Set all the animation to false
		for(size_t i = 0; i < renderFlowerArray.size(); ++i) {
			renderFlowerArray[i]->setShow(false);
		}
		// Set the right animation to true
		renderFlowerArray[action]->setShow(true);
	}
}

void EntityManager::setDestructibleObjectRender(size_t index, DestructibleObjectAction action) {
	std::vector<RenderEntity*> renderDestructibleObjectArray = getRenderEntity(index);
	// Set all the animation to false
	for(size_t i = 0; i < renderDestructibleObjectArray.size(); ++i) {
		renderDestructibleObjectArray[i]->setShow(false);
	}
	// Set the right animation to true
	renderDestructibleObjectArray[action]->setShow(true);
	// Launch timer
	renderDestructibleObjectArray[action]->manageAnimationTimer(AnimationLength::DestructibleObjectLength);
	
	// Launch sound
	switch(action) {
		case NormalBox:
			SoundManager::getInstance()->playSound(SoundType::DESTRUCTIBLE_OBJECT);
			break;
		case ByFlames:
			SoundManager::getInstance()->playSound(SoundType::DESTRUCTIBLE_OBJECT);
			break;
		case ByShivering:
			SoundManager::getInstance()->playSound(SoundType::DESTRUCTIBLE_OBJECT);
			break;
	}
}

void EntityManager::updateThermometherRender() {
	std::vector<RenderEntity*> tempRenderEntities = getRenderEntity(m_thermometerTemperatureIndex);
	std::vector<RenderEntity*> supportRenderEntities = getRenderEntity(m_thermometerSupportIndex);

	// Get data
	try{
		Fever* feverPower = dynamic_cast<Fever*>(getPower(PowerType::FeverType));
		
		float currentTemp = feverPower->getCurrentTemperature();
		int maxTemp = feverPower->getMaxTemperature();
		int minTemp = feverPower->getMinTemperature();

		// Set the temperature entity height
		float totalHeight = maxTemp - minTemp;
		float ratio = (currentTemp + maxTemp) / totalHeight;
		float maxScale = supportRenderEntities.at(0)->getHeight();
		float currentScaleFactor = ratio * maxScale;
		tempRenderEntities.at(0)->setScale(supportRenderEntities.at(0)->getWidth(), currentScaleFactor);

		// Set the thermometer color
		float colorRed = currentTemp > 0 ? currentTemp/(float)maxTemp : 0;
		float colorGreen = currentTemp > 0 ? 1 - currentTemp/(float)maxTemp : 1 - currentTemp/(float)minTemp;
		float colorBlue = currentTemp < 0 ? currentTemp/(float)minTemp : 0;
		tempRenderEntities.at(0)->setTint(255*colorRed, 255*colorGreen, 255*colorBlue);

		// Set the position of the thermomether to follow the Dino on the screen
		float posX = getRenderDino()[0]->getPosX() - getRenderDino()[0]->getWidth();
		float posY = getRenderDino()[0]->getPosY() - getRenderDino()[0]->getHeight() ;

		tempRenderEntities.at(0)->setAngleXYZ(0, 0, 180);
		tempRenderEntities.at(0)->setPosition(posX + supportRenderEntities.at(0)->getWidth(), posY + supportRenderEntities.at(0)->getHeight());
		supportRenderEntities.at(0)->setPosition(posX, posY);
	}
	catch(std::bad_cast& err){
		std::cerr << err.what() << " : Cast error in function setThermometherRender(). The render thermometer can't be updated." << std::endl;
		return;
	}
}

void EntityManager::updateFlames(){
	for(size_t indexEntity = 0; indexEntity < getPhysicalEntityArray().size(); ++indexEntity) {
		if(getPhysicalEntityArray().at(indexEntity) != nullptr){
			if(getPhysicalEntityArray().at(indexEntity)->getType() == PhysicalType::Flames){
				b2Vec2 pos;
				// Update Render
				for(size_t indexRenderFlames = 0; indexRenderFlames < getRenderEntityArray().at(indexEntity).size(); indexRenderFlames++){
					if(getRenderDino().at(DinoAction::StopNormal)->isFlippedHorizontaly()) {
						pos = b2Vec2(getRenderDino().at(DinoAction::StopNormal)->getPosX() - getRenderDino().at(DinoAction::StopNormal)->getWidth(), getRenderDino().at(DinoAction::StopNormal)->getPosY());
						getRenderEntityArray().at(indexEntity).at(indexRenderFlames)->flipHorizontaly(true);
					}
					else{
						pos = b2Vec2(getRenderDino().at(DinoAction::StopNormal)->getPosX() + getRenderDino().at(DinoAction::StopNormal)->getWidth(), getRenderDino().at(DinoAction::StopNormal)->getPosY());
						getRenderEntityArray().at(indexEntity).at(indexRenderFlames)->flipHorizontaly(false);
					}
				}
				// Update Physics
				getPhysicalEntityArray().at(indexEntity)->setPosition(pos.x, pos.y);
				getPhysicalEntityArray().at(indexEntity)->setLinearVelocity(getPhysicalDino()->getLinearVelocity());
			}
		}
	}
}

void EntityManager::updateDestructibleObjects(){
	for(size_t indexEntity = 0; indexEntity < getPhysicalEntityArray().size(); ++indexEntity) {
		if(getPhysicalEntityArray().at(indexEntity) != nullptr){
			if(getPhysicalEntityArray().at(indexEntity)->getType() == PhysicalType::DestructibleObject){
				std::vector<RenderEntity*> renderEntities = getRenderEntityArray().at(indexEntity);
				if(renderEntities.at(DestructibleObjectAction::ByFlames)->isShow()){
					PhysicalEntity::setMovableObjectDynamic();
					renderEntities.at(DestructibleObjectAction::ByFlames)->manageAnimationTimer(AnimationLength::DestructibleObjectLength);
				}
				if(renderEntities.at(DestructibleObjectAction::ByShivering)->isShow()){
					PhysicalEntity::setMovableObjectDynamic();
					renderEntities.at(DestructibleObjectAction::ByShivering)->manageAnimationTimer(AnimationLength::DestructibleObjectLength);
				}
			}
		}
	}
}

bool EntityManager::isDeathAnimationPlaying(){
	if(getRenderDino().at(DinoAction::DeathNormal)->isAnimationPlaying()
		||getRenderDino().at(DinoAction::DeathFever)->isAnimationPlaying()
		||getRenderDino().at(DinoAction::DeathHypothermia)->isAnimationPlaying())
		return true;
	else
		return false;
}

DinoAction EntityManager::getRightDeath(){

	// Hot fever or Spit fire
	if(getCurrentPowerState() == PowerState::HotFeverState || getCurrentPowerState() == PowerState::SpitFireState) {
		return DinoAction::DeathFever;
	}
	
	// Hypothermia or Shivring
	else if(getCurrentPowerState() == PowerState::HypothermiaState || getCurrentPowerState() == PowerState::ShiveringState) {
		return DinoAction::DeathHypothermia;
	}

	// Normal death
	return DinoAction::DeathNormal;
}

DinoAction 	EntityManager::getRightWalk(){

	// Sneeze

	//Only stop sprite is render when dino is in the air
	if(getPhysicalDino()->getLinearVelocity().y < 1.f && getPhysicalDino()->getLinearVelocity().y > -1.f){
		if(getCurrentPowerType() == PowerType::SneezeType) {

			if(getCurrentPowerState() == PowerState::HypothermiaState) {
				return DinoAction::ColdSneezing;
			}

			else if(getCurrentPowerState() == PowerState::HotFeverState) {
				return DinoAction::FeverSneezing;
			}

			return DinoAction::Sneezing;	
		}

		// Fever
		else if(getCurrentPowerType() == PowerType::FeverType){
			
			if(getCurrentPowerState() == PowerState::HotFeverState || getCurrentPowerState() == PowerState::SpitFireState) {
				return DinoAction::WalkFever;
			}

			else if(getCurrentPowerState() == PowerState::HypothermiaState) {
				return DinoAction::WalkHypothermia;
			}

			else if(getCurrentPowerState() == PowerState::ShiveringState) {
				return DinoAction::WalkShivering;
			}
		}

		// Normal
		return DinoAction::WalkNormal;
	}
	else return getRightStop();
}

DinoAction 	EntityManager::getRightStop() {

	// Sneeze
	if(getCurrentPowerType() == PowerType::SneezeType) {

		if(getCurrentPowerState() == PowerState::HypothermiaState) {
			return DinoAction::ColdSneezing;
		}

		else if(getCurrentPowerState() == PowerState::HotFeverState) {
			return DinoAction::FeverSneezing;
		}

		return DinoAction::Sneezing;
	}

	// Fever
	else if(getCurrentPowerType() == PowerType::FeverType){
		
		if(getCurrentPowerState() == PowerState::HotFeverState || getCurrentPowerState() == PowerState::SpitFireState) {
			return DinoAction::StopFever;
		}

		else if(getCurrentPowerState() == PowerState::HypothermiaState) {
			return DinoAction::StopHypothermia;
		}

		else if(getCurrentPowerState() == PowerState::ShiveringState) {
			return DinoAction::StopShivering;
		}
	}

	// Normal
	return DinoAction::StopNormal;
}

}