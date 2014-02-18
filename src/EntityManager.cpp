#include "EntityManager.h"

namespace Symp {

EntityManager::EntityManager() {
	EntityManager::m_pEntity2dManager = new IND_Entity2dManager();
 	m_pPhysicalManager = new PhysicalManager(0.f, 0.f);
}

EntityManager::~EntityManager(){
	m_pEntity2dManager->end();
    DISPOSE(m_pEntity2dManager);
	RenderEntity::end();
	delete m_pPhysicalManager;
}

EntityManager* EntityManager::getEntityManagerInstance() {
	if(m_instance == NULL) {
		m_instance = new EntityManager();
	}

	return m_instance;
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

void EntityManager::renderEntities(){
	m_pEntity2dManager->renderEntities2d();
}

void EntityManager::updateEntities() {
	// Update Physical entities
	m_pPhysicalManager->updatePhysics();
	// Update Render Entities
	int32 numEntity = 0;
	std::vector<RenderEntity*>::iterator it;
	for(it = m_renderEntityArray.begin(); it != m_renderEntityArray.end(); ++it) {
		PhysicalEntity* tmpPhysicalEntity;
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

void EntityManager::addDino(){
 	//PhysicalEntity* pPhysicalDino = new PhysicalEntity(m_pPhysicalManager->getWorld(), b2Vec2(100.f, 100.f));
	//pPhysicalDino->setMass(100.f, 100.f);
	//pPhysicalDino->setPosition(0.f, 0.f);
	RenderEntity *pRenderDino = new RenderEntity("../assets/animation/rabbit_animation.xml", Symp::Animation);
	pRenderDino->setHotSpot(0.5f, 0.5f);
	pRenderDino->setSequence(0); //sequence "rabbit_flash_normal" in rabbit_anmaition.xml
	pRenderDino->setPosition(400.f, 400.f);
	addEntity(pRenderDino, 0, nullptr, nullptr);
}

RenderEntity* EntityManager::getRenderDino() const {
	return m_renderEntityArray[0];
}

PhysicalEntity* EntityManager::getPhysicalDino() const {
	return m_physicalEntityArray[0];

}

void EntityManager::loadTestWorld(){
 	//Temporary !
 	// ----- PHYSIC DATA -----
	b2World* world = getPhysicalManager()->getWorld();
	//static body => ground
	PhysicalEntity* pGround = new PhysicalEntity(world, b2Vec2(100.0f, 1.0f));
	pGround->setMass(0.f, 0.f);
	pGround->setPosition(0.f, -50.f);
	//dynamic body => rabbit1
	PhysicalEntity* pRabbit1 = new PhysicalEntity(world, b2Vec2(100.f, 100.f));
	pRabbit1->setMass(100.f, 100.f);
	pRabbit1->setPosition(400.f, 200.f);
	//dynamic body => rabbit2
	PhysicalEntity* pRabbit2 = new PhysicalEntity(world, b2Vec2(10.f, 10.f));
	pRabbit2->setMass(10.f, 10.f);
	pRabbit2->setPosition(400.f, 300.f);

	// ----- RENDER DATA -----
	// background
	RenderEntity* rBack = new RenderEntity("../assets/cave.png", Symp::Surface);
	rBack->setHotSpot(0.5f, 0.5f);
	rBack->setPosition(0.f, 300.f);
	// Creating 2d entity for the Rabbit1
	RenderEntity *rRabbit1 = new RenderEntity("../assets/animation/rabbit_animation.xml", Symp::Animation);
	rRabbit1->setHotSpot(0.5f, 0.5f);
	rRabbit1->setSequence(0); //sequence "rabbit_flash_normal" in rabbit_anmaition.xml
	// Creating 2d entity for the Rabbit2
	RenderEntity *rRabbit2 = new RenderEntity("../assets/animation/rabbit_animation.xml", Symp::Animation);
	rRabbit2->setHotSpot(0.5f, 0.5f);
	rRabbit2->setSequence(1); //sequence "rabbit_flash_fast" in rabbit_anmaition.xml

	// ----- ADD ENTITIES TO THE MANAGER -----
	addRenderEntity(rBack, 0);
	addPhysicalEntity(pGround);
	addEntity(rRabbit1, 0, pRabbit1, NULL);
	addEntity(rRabbit2, 0, pRabbit2, NULL);
}

// Initialize singleton
EntityManager*			EntityManager::m_instance = NULL;
IND_Entity2dManager*	EntityManager::m_pEntity2dManager = NULL;
}