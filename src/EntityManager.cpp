#include "EntityManager.h"
#include "sound/SoundManager.h"
#include "power/Fever.h"
#include "power/Sneeze.h"

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
	// Delete entities which has to be destroyed
	std::vector<std::vector<RenderEntity*>>::iterator itRender = m_renderEntityArray.begin();
	std::vector<std::vector<SoundEntity*>>::iterator itSound = m_soundEntityArray.begin();
	for(std::vector<PhysicalEntity*>::iterator itPhysical = m_physicalEntityArray.begin(); itPhysical != m_physicalEntityArray.end();){
		//if PhysicalEntityHasToBeDestroyed
		if((*itPhysical) != nullptr && (*itPhysical)->hasToBeDestroyed()){
			for(size_t i = 0; i < (*itRender).size(); ++i){
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

	// Update specific elements when fever
	if(isPowerExisting(PowerType::FeverType)) {
		setThermometherRender();
		setFlames();
	}
}

void EntityManager::deleteAllEntities() {
	m_physicalEntityArray.clear();
	m_renderEntityArray.clear();
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
			itRender = m_renderEntityArray.erase(itRender);
			m_pPhysicalWorld->getWorld()->DestroyBody(m_physicalEntityArray.at(indexCurrent)->getb2Body());
			itPhysical = m_physicalEntityArray.erase(itPhysical);
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
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::StopHotFever, rEntityFeverStop);

	// Walk
	RenderEntity* rEntity2 = new RenderEntity("../assets/animation/WalkRight.xml", Symp::Animation);
	rEntity2->setScale(scaleFactor, scaleFactor);
	rEntity2->setHotSpot(0.5, 0.5);
	rEntity2->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::Walk, rEntity2);

	// Fever
	RenderEntity* rEntityFever = new RenderEntity("../assets/animation/Fever.xml", Symp::Animation);
	rEntityFever->setScale(scaleFactor, scaleFactor);
	rEntityFever->setHotSpot(0.5, 0.5);
	rEntityFever->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::WalkHotFever, rEntityFever);

	// Jump
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::Jump, NULL);

	// die
	RenderEntity* rEntity3 = new RenderEntity("../assets/animation/Die.xml", Symp::Animation);
	rEntity3->setScale(scaleFactor, scaleFactor);
	rEntity3->setHotSpot(0.5, 0.5);
	rEntity3->setShow(false);
	renderEntityArray.insert(renderEntityArray.begin() + DinoAction::Die, rEntity3);

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
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::StopHotFever, NULL);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::Walk, NULL);
	soundEntityArray.insert(soundEntityArray.begin() + DinoAction::WalkHotFever, NULL);


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
	addEntity(renderEntityArray, 63, pEntity, soundEntityArray);
}

void EntityManager::killDino() {
	// If the animation is not playing : dino is not dead
	if(!getRenderDino().at(DinoAction::Die)->isAnimationPlaying()) {
		SoundManager::getInstance()->play(getSoundDino()[DinoAction::Die]->getIndexSound());
		setDinoRender(DinoAction::Die);
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
	PhysicalEntity* pDino = getPhysicalDino();
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
	b2Vec2 pos;
	if(getPhysicalDino()->getLinearVelocity().x < 0) {
		flames1->flipHorizontaly(true);
		pos = b2Vec2(pDino->getPosition().x - 2*pDino->getWidth(), pDino->getPosition().y);
	}
	else{
		flames1->flipHorizontaly(false);
		pos = b2Vec2(pDino->getPosition().x + 2*pDino->getWidth(), pDino->getPosition().y);
	}
	PhysicalEntity* physicalFlamesEntity = new PhysicalEntity(
		m_pPhysicalWorld->getWorld(),
		pos,
		b2Vec2(flames1->getWidth(), flames1->getHeight()),
		PhysicalType::Flames
	);
	physicalFlamesEntity->setMass(0.1f, 0.f);

	if(getPhysicalDino()->getLinearVelocity().x < 0) {
		physicalFlamesEntity->getb2Body()->ApplyLinearImpulse(
			b2Vec2(-20000, 0),
			physicalFlamesEntity->getb2Body()->GetWorldCenter(), 
			physicalFlamesEntity->isAwake());
	}
	else{
		physicalFlamesEntity->getb2Body()->ApplyLinearImpulse(
			b2Vec2(20000, 0),
			physicalFlamesEntity->getb2Body()->GetWorldCenter(), 
			physicalFlamesEntity->isAwake());
	}

	/**************/
	/* Add Flames */
	/**************/
	addEntity(renderFlamesArray, 63, physicalFlamesEntity, std::vector<SoundEntity*>());
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
		m_powerArray.at(powerType);
		return true;
	}
	catch(std::out_of_range& err){
		return false;
	}
}

DinoAction EntityManager::getCurrentDinoAction() const {
	try{
		if(!getRenderDino().at(DinoAction::Die)->isAnimationPlaying()){
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
		return DinoAction::Die;
	}
	catch(std::out_of_range& err){
		std::cerr << err.what() << " : Error when access a specific RenderEntity of dino in function EntityManager::getCurrentDinoAction." << std::endl;
		std::cerr << "The program will be aborted." << std::endl;
		exit(EXIT_FAILURE);
	}
}

PowerType EntityManager::getCurrentPowerState() const{
	if(isPowerExisting(PowerType::SneezeType) 
		&& (dynamic_cast<Sneeze*>(getPower(PowerType::SneezeType))->isWarningSneeze() || dynamic_cast<Sneeze*>(getPower(PowerType::SneezeType))->isSneezing()))
		return PowerType::SneezeType;
	else if(isPowerExisting(PowerType::FeverType) && dynamic_cast<Fever*>(getPower(PowerType::FeverType))->getThermometerStep() >= 6)
		return PowerType::FeverType;
	else
		return PowerType::NormalType;
}

bool EntityManager::isDinoAllowToMove(){
	if(!isPowerExisting(PowerType::SneezeType))
		return true;
	if(!getPower(PowerType::SneezeType)->isActivated() && !getRenderDino().at(DinoAction::Die)->isAnimationPlaying())
		return true;
	else
		return false;
}

void EntityManager::setDinoRender(DinoAction dinoAction) {
	if(!getRenderDino().at(DinoAction::Die)->isAnimationPlaying() && !getRenderDino().at(DinoAction::Die)->isAnimationFinish()){
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
				if(dinoAction == DinoAction::Die){
					getRenderDino().at(DinoAction::Die)->manageAnimationTimer(AnimationLength::DieLength);
				}
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

void EntityManager::setFlames(){
	for(size_t indexEntity = 0; indexEntity < getPhysicalEntityArray().size(); ++indexEntity) {
		if(getPhysicalEntityArray().at(indexEntity) != nullptr){
			if(getPhysicalEntityArray().at(indexEntity)->getType() == PhysicalType::Flames){
				getPhysicalEntityArray().at(indexEntity)->getb2Body()->ApplyLinearImpulse(
					b2Vec2(getPhysicalEntityArray().at(indexEntity)->getLinearVelocity().x, 0),
					getPhysicalEntityArray().at(indexEntity)->getb2Body()->GetWorldCenter(), 
					getPhysicalEntityArray().at(indexEntity)->isAwake());
			}
		}
	}
}

}