#include "EntityManager.h"
#include "sound/SoundManager.h"
#include "power/Fever.h"
#include "power/Sneeze.h"
#include <stdexcept>

namespace Symp {

EntityManager::EntityManager() {
	m_thermometerSupportIndex = -1;
	m_thermometerTemperatureIndex = -1;
}

EntityManager::~EntityManager(){
	m_pEntity2dManager->end();
	DISPOSE(m_pEntity2dManager);
	RenderEntity::end();
	delete m_pPhysicalWorld;
	deleteAllEntities();
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

	// Delete entities which has to be destroyed
	std::vector<std::vector<RenderEntity*>>::iterator itRender = m_renderEntityArray.begin();
	std::vector<std::vector<SoundEntity*>>::iterator itSound = m_soundEntityArray.begin();
	for(std::vector<PhysicalEntity*>::iterator itPhysical = m_physicalEntityArray.begin(); itPhysical != m_physicalEntityArray.end();){
		//if PhysicalEntityHasToBeDestroyed
		if((*itPhysical) != nullptr && (*itPhysical)->hasToBeDestroyed()){
			for(size_t i = 0; i < (*itRender).size(); ++i){
				if((*itRender)[i] != nullptr)
					m_pEntity2dManager->remove((*itRender)[i]->getIND_Entity2d());
			}
			itRender = m_renderEntityArray.erase(itRender);
			m_pPhysicalWorld->getWorld()->DestroyBody((*itPhysical)->getb2Body());
			itPhysical = m_physicalEntityArray.erase(itPhysical);
			itSound = m_soundEntityArray.erase(itSound);
		}
		else{
			++itRender;
			++itPhysical;
			++itSound;
		}
	}

	// Update Physical entities
	m_pPhysicalWorld->updatePhysics();
	//if(!getPhysicalDino()->getb2Body()->IsActive()) getPhysicalDino()->getb2Body()->SetActive(true);
	if(EntityManager::getInstance()->isPowerExisting(PowerType::SneezeType))
		PhysicalEntity::checkMovableObject(EntityManager::getInstance()->getPower(PowerType::SneezeType)->isActivated());
	
	// Update Render Entities which correspond to Physical Entities
	for(size_t i = 0; i < m_renderEntityArray.size(); i++) {
		std::vector<RenderEntity*> rEntities = m_renderEntityArray.at(i);
		PhysicalEntity* pEntity = m_physicalEntityArray.at(i);
		if((rEntities.size() > 0) && (pEntity != nullptr)) {
			for(size_t indexRenderEntity = 0; indexRenderEntity < rEntities.size(); ++indexRenderEntity){
				if(rEntities[indexRenderEntity] != nullptr)
					rEntities[indexRenderEntity]->setPosition(pEntity->getPosition().x, pEntity->getPosition().y);
			}
		}
	}

	// Update specific elements when fever
	if(isPowerExisting(PowerType::FeverType)) {
		setThermometherRender();
		setFlames();
	}
}

void EntityManager::deleteAllEntities() {
	// Delete physicalEntityArray
	// for(std::vector<PhysicalEntity*>::iterator it = m_physicalEntityArray.begin(); it != m_physicalEntityArray.end(); ++it) {
	// 	delete *it;
	// }
	m_physicalEntityArray.clear();

	// Delete renderEntityArray
	// for(std::vector<std::vector<RenderEntity*>>::iterator it = m_renderEntityArray.begin(); it != m_renderEntityArray.end(); ++it) {
	// 	for(std::vector<RenderEntity*>::iterator itt = (*it).begin(); itt != (*it).end(); ++itt) {
	// 		delete *itt;
	// 	}
	// 	(*it).clear();
	// }
	m_renderEntityArray.clear();

	// Delete soundEntityArray
	// for(std::vector<std::vector<SoundEntity*>>::iterator it = m_soundEntityArray.begin(); it != m_soundEntityArray.end(); ++it) {
	// 	for(std::vector<SoundEntity*>::iterator itt = (*it).begin(); itt != (*it).end(); ++itt) {
	// 		delete *itt;
	// 	}
	// 	(*it).clear();
	// }
	m_soundEntityArray.clear();
}

bool EntityManager::deleteEntity(size_t indexEntity) {
	size_t indexCurrent = 0;

	std::vector<std::vector<RenderEntity*>>::iterator itRender = m_renderEntityArray.begin();
	std::vector<std::vector<SoundEntity*>>::iterator itSound = m_soundEntityArray.begin();

	for(std::vector<PhysicalEntity*>::iterator itPhysical = m_physicalEntityArray.begin(); itPhysical != m_physicalEntityArray.end();){
		if(indexCurrent == indexEntity){

			for(size_t i = 0; i < (*itRender).size(); ++i){
				m_pEntity2dManager->remove((*itRender)[i]->getIND_Entity2d());
			}
			
			for(std::vector<RenderEntity*>::iterator it = (*itRender).begin(); it != (*itRender).end(); ++it) {
				delete *it;
			}
			itRender = m_renderEntityArray.erase(itRender);


			m_pPhysicalWorld->getWorld()->DestroyBody(m_physicalEntityArray.at(indexCurrent)->getb2Body());
			
			delete *itPhysical;
			itPhysical = m_physicalEntityArray.erase(itPhysical);



			for(std::vector<SoundEntity*>::iterator it = (*itSound).begin(); it != (*itSound).end(); ++it) {
				delete *it;
			}
			itSound = m_soundEntityArray.erase(itSound);

			return true;
		}

		else{
			itRender++;
			itPhysical++;
			itSound++;
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
	RenderEntity* rEntityStopShivering = new RenderEntity("../assets/animation/Shivering.xml", Symp::Animation);
	rEntityStopShivering->setScale(scaleFactor, scaleFactor);
	rEntityStopShivering->setHotSpot(0.5, 0.5);
	rEntityStopShivering->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::StopShivering, rEntityStopShivering);

	// Walk Normal
	RenderEntity* rEntity2 = new RenderEntity("../assets/animation/WalkRight.xml", Symp::Animation);
	rEntity2->setScale(scaleFactor, scaleFactor);
	rEntity2->setHotSpot(0.5, 0.5);
	rEntity2->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::WalkNormal, rEntity2);

	// Walk Fever
	RenderEntity* rEntityFever = new RenderEntity("../assets/animation/Fever.xml", Symp::Animation);
	rEntityFever->setScale(scaleFactor, scaleFactor);
	rEntityFever->setHotSpot(0.5, 0.5);
	rEntityFever->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::WalkFever, rEntityFever);

	// Walk Fever
	RenderEntity* rEntityWalkHypothermia = new RenderEntity("../assets/animation/WalkRightHypothermia.xml", Symp::Animation);
	rEntityWalkHypothermia->setScale(scaleFactor, scaleFactor);
	rEntityWalkHypothermia->setHotSpot(0.5, 0.5);
	rEntityWalkHypothermia->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::WalkHypothermia, rEntityWalkHypothermia);

	// Walk Shivering
	RenderEntity* rEntityWalkShivering = new RenderEntity("../assets/animation/WalkRightShivering.xml",  Symp::Animation);
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
	RenderEntity* rEntitySneeze = new RenderEntity("../assets/animation/Sneeze.xml", Symp::Animation);
	rEntitySneeze->setScale(scaleFactor, scaleFactor);
	rEntitySneeze->setHotSpot(0.5, 0.5);
	rEntitySneeze->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::Sneezing, rEntitySneeze);


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
	 
	/*********/
	/* Sound */
	/*********/
	std::vector<SoundEntity*> soundEntityArray;

	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::StopNormal, NULL);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::StopFever, NULL);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::StopHypothermia, NULL);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::StopShivering, NULL);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::WalkNormal, NULL);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::WalkFever, NULL);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::WalkHypothermia, NULL);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::WalkShivering, NULL);

	// Jump
	size_t indexSound3 = SoundManager::getInstance()->loadSound("../assets/audio/jump.ogg");
	SoundEntity* sEntity3 = new SoundEntity(indexSound3);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::Jump, sEntity3);

	// Normal Death
	size_t indexSound5 = SoundManager::getInstance()->loadSound("../assets/audio/death.ogg");
	SoundEntity* sEntity5 = new SoundEntity(indexSound5);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::DeathNormal, sEntity5);
	// Fever Death
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::DeathFever, sEntity5);
	// Hypothermia Death
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::DeathHypothermia, sEntity5);

	// Sneeze
	size_t indexSound4 = SoundManager::getInstance()->loadSound("../assets/audio/sneeze.ogg");
	SoundEntity* sEntity4 = new SoundEntity(indexSound4);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::Sneezing, sEntity4);

	/*****************/
	/* Add Dino */
	/*****************/
	addEntity(renderEntityArray, 63, pEntity, soundEntityArray);
}

void EntityManager::killDino(DinoAction deathType) {
	// If the animation is not playing : dino is not dead
	if(!isDeathAnimationPlaying()) {
		SoundManager::getInstance()->play(getSoundDino()[deathType]->getIndexSound());
		setDinoRender(deathType);
	}
}

void EntityManager::addThermometer() {
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
	flames1->setScale(0.2, 0.2);
	flames1->setShow(true);
	renderFlamesArray.push_back(flames1);

	/************/
	/* Physical */
	/************/
	PhysicalEntity* pDino = getPhysicalDino();
	b2Vec2 pos;
	if(getRenderDino().at(DinoAction::StopNormal)->isFlippedHorizontaly()) {
		flames1->flipHorizontaly(true);
		pos = b2Vec2(pDino->getPosition().x - 1.5*pDino->getWidth(), pDino->getPosition().y);
	}
	else{
		flames1->flipHorizontaly(false);
		pos = b2Vec2(pDino->getPosition().x + 1.5*pDino->getWidth(), pDino->getPosition().y);
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
	addEntity(renderFlamesArray, 63, physicalFlamesEntity, std::vector<SoundEntity*>());
}

void EntityManager::shiverBackground() {
	int i = 0;
	// Get all physical entities near from Dino
	for(std::vector<PhysicalEntity*>::iterator it = getPhysicalEntityArray().begin(); it != getPhysicalEntityArray().end(); ++it) {
		if((*it) != nullptr) {

			b2Vec2 dinoPosition = getPhysicalDino()->getPosition();	 	
		 	b2Vec2 position = (*it)->getPosition();
		
		 	b2Vec2 distance = position - dinoPosition;
			if(sqrt(pow(distance.x, 2) + pow(distance.y, 2)) < 100) {

				// Animate blocs
				

				// std::vector<RenderEntity*> renderEntity = getRenderEntity(getIndexEntity(*it));

				// if( !renderEntity.empty()) {
				// 	for(std::vector<RenderEntity*>::iterator render_it = renderEntity.begin(); render_it != renderEntity.end(); ++render_it) {
				// 		if(*render_it != nullptr) {
				// 			//(*render_it)->setShow(false);
				// 		}
						
				// 	}
				// }
				

			}
		}	
		i++;	
	}
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
std::vector<SoundEntity*> EntityManager::getSoundEntity(size_t index) const {
	try{
		return m_soundEntityArray.at(index);
	}
	catch(std::out_of_range& err){
		std::cerr << err.what() << " : Error when access vector<SoundEntity*> at index " << index << " in function EntityManager::getSoundEntity." << std::endl;
		return std::vector<SoundEntity*>();
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

std::vector<SoundEntity*> EntityManager::getSoundDino() const {
	try{
		for(size_t indexEntity = 0; indexEntity < getPhysicalEntityArray().size(); ++indexEntity) {
			if(getPhysicalEntityArray().at(indexEntity) != nullptr){
				if(getPhysicalEntityArray().at(indexEntity)->getType() == PhysicalType::Dino){
					return m_soundEntityArray.at(indexEntity);
				}
			}
		}
		return std::vector<SoundEntity*>();
	}
	catch(std::out_of_range& err){
		std::cerr << err.what() << " : Error when access vector<SoundEntity*> of dino at index in function EntityManager::getSoundDino." << std::endl;
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

PowerType EntityManager::getCurrentPowerState() const{
	try{
		if(isPowerExisting(PowerType::SneezeType) 
			&& (dynamic_cast<Sneeze*>(getPower(PowerType::SneezeType))->isWarningSneeze() || dynamic_cast<Sneeze*>(getPower(PowerType::SneezeType))->isSneezing()))
			return PowerType::SneezeType;
		else if(isPowerExisting(PowerType::FeverType) && getPower(PowerType::FeverType)->isActivated()) {
			if(dynamic_cast<Fever*>(getPower(PowerType::FeverType))->isInHotRange())
				return PowerType::FeverType;
			else if(dynamic_cast<Fever*>(getPower(PowerType::FeverType))->isInColdRange())
				return PowerType::ShiveringType;
			else
				return PowerType::NormalType;
		}
		else
			return PowerType::NormalType;
	}
	catch(std::bad_cast& err){
		std::cerr << err.what() << " : Cast error in function getCurrentPowerState()" << std::endl;
		return PowerType::NormalType;
	}
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
	// Flip to the left all render entities
	for(size_t i = 0; i < getRenderDino().size(); ++i) {
		if(getRenderDino().at(i) != nullptr){
			if(getPhysicalDino()->getLinearVelocity().x < 0) {
				getRenderDino().at(i)->flipHorizontaly(true);
			}
			else if(getPhysicalDino()->getLinearVelocity().x > 0) {
				getRenderDino().at(i)->flipHorizontaly(false);
			}
		}
	}
	// Set visible the correct render entity
	for(size_t indexRenderDino = 0; indexRenderDino < getRenderDino().size(); ++indexRenderDino){
		if(getRenderDino()[indexRenderDino] != nullptr) {
			getRenderDino()[indexRenderDino]->setShow(false);
		}

		if(getRenderDino()[indexRenderDino] != nullptr && indexRenderDino == static_cast<size_t>(dinoAction)){
			getRenderDino().at(dinoAction)->setShow(true);
			if(dinoAction == getRightDeath()){
				getRenderDino().at(getRightDeath())->manageAnimationTimer(AnimationLength::DieLength);
			}
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

void EntityManager::setThermometherRender() {
	std::vector<RenderEntity*> tempRenderEntities = getRenderEntity(m_thermometerTemperatureIndex);
	std::vector<RenderEntity*> supportRenderEntities = getRenderEntity(m_thermometerSupportIndex);

	// Get data
	try{
		float currentTemp = dynamic_cast<Fever*>(getPower(PowerType::FeverType))->getCurrentTemperature();
		int maxTemp = dynamic_cast<Fever*>(getPower(PowerType::FeverType))->getMaxTemperature();
		int minTemp = dynamic_cast<Fever*>(getPower(PowerType::FeverType))->getMinTemperature();

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
		float leftMargin = 10;
		float posX = getRenderDino()[0]->getPosX() - 200 + leftMargin;
		float posY = getRenderDino()[0]->getPosY() - 140 ;

		tempRenderEntities.at(0)->setAngleXYZ(0, 0, 180);
		tempRenderEntities.at(0)->setPosition(posX + supportRenderEntities.at(0)->getWidth(), posY + supportRenderEntities.at(0)->getHeight());
		supportRenderEntities.at(0)->setPosition(posX, posY);
	}
	catch(std::bad_cast& err){
		std::cerr << err.what() << " : Cast error in function setThermometherRender(). The render thermometer can't be updated." << std::endl;
		return;
	}
}

void EntityManager::setFlames(){
	for(size_t indexEntity = 0; indexEntity < getPhysicalEntityArray().size(); ++indexEntity) {
		if(getPhysicalEntityArray().at(indexEntity) != nullptr){
			if(getPhysicalEntityArray().at(indexEntity)->getType() == PhysicalType::Flames){
				b2Vec2 pos;
				// Update Render
				for(size_t indexRenderFlames = 0; indexRenderFlames < getRenderEntityArray().at(indexEntity).size(); indexRenderFlames++){
					if(getRenderDino().at(DinoAction::StopNormal)->isFlippedHorizontaly()) {
						pos = b2Vec2(getRenderDino().at(DinoAction::StopNormal)->getPosX() - 1.5*getRenderDino().at(DinoAction::StopNormal)->getWidth(), getRenderDino().at(DinoAction::StopNormal)->getPosY());
						getRenderEntityArray().at(indexEntity).at(indexRenderFlames)->flipHorizontaly(true);
					}
					else{
						pos = b2Vec2(getRenderDino().at(DinoAction::StopNormal)->getPosX() + 1.5*getRenderDino().at(DinoAction::StopNormal)->getWidth(), getRenderDino().at(DinoAction::StopNormal)->getPosY());
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

bool EntityManager::isDeathAnimationPlaying(){
	if(getRenderDino().at(DinoAction::DeathNormal)->isAnimationPlaying()
		||getRenderDino().at(DinoAction::DeathFever)->isAnimationPlaying()
		||getRenderDino().at(DinoAction::DeathHypothermia)->isAnimationPlaying())
		return true;
	else
		return false;
}

DinoAction EntityManager::getRightDeath(){
	if(getCurrentPowerState() == PowerType::FeverType)
		return DinoAction::DeathFever;
	else if(getCurrentPowerState() == PowerType::ShiveringType)
		return DinoAction::DeathHypothermia;
	else 
		return DinoAction::DeathNormal;
}

DinoAction 	EntityManager::getRightWalk(){
	if(getCurrentPowerState() == PowerType::SneezeType)
		return DinoAction::Sneezing;
	else if(getCurrentPowerState() == PowerType::FeverType)
		return DinoAction::WalkFever;
	else if(getCurrentPowerState() == PowerType::HypothermiaType)
		return DinoAction::WalkHypothermia;
	else if(getCurrentPowerState() == PowerType::ShiveringType)
		return DinoAction::WalkShivering;
	else 
		return DinoAction::WalkNormal;
}

DinoAction 	EntityManager::getRightStop(){
	if(getCurrentPowerState() == PowerType::SneezeType)
		return DinoAction::Sneezing;
	else if(getCurrentPowerState() == PowerType::FeverType)
		return DinoAction::StopFever;
	else if(getCurrentPowerState() == PowerType::HypothermiaType)
		return DinoAction::StopHypothermia;
	else if(getCurrentPowerState() == PowerType::ShiveringType)
		return DinoAction::StopShivering;
	else 
		return DinoAction::StopNormal;
}

}