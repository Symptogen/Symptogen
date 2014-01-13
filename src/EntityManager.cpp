#include "EntityManager.h"

namespace Symp{

EntityManager::EntityManager(Render* pRender){
	m_pEntity2dManager = new IND_Entity2dManager();
	m_pEntity2dManager->init(pRender->getIND_Render());
	RenderEntity::init(pRender);
 	m_pPhysicalManager = new PhysicalManager(0.f, -10.f);
}

EntityManager::~EntityManager(){
	m_pEntity2dManager->end();
    DISPOSE(m_pEntity2dManager);
	RenderEntity::end();
}

bool EntityManager::addRenderEntity(RenderEntity* pRenderEntity, int layer){
	m_renderEntityArray.push_back(pRenderEntity);
	m_physicalEntityArray.push_back(NULL);
	return m_pEntity2dManager->add(layer, pRenderEntity->getIND_Entity2d());
}

void EntityManager::addPhysicalEntity(PhysicalEntity* pPhysicalEntity){
	m_renderEntityArray.push_back(NULL);
	m_physicalEntityArray.push_back(pPhysicalEntity);
}

bool EntityManager::addEntity(RenderEntity* pRenderEntity, int layer, PhysicalEntity* pPhysicalEntity){
	m_renderEntityArray.push_back(pRenderEntity);
	m_physicalEntityArray.push_back(pPhysicalEntity);	
	return m_pEntity2dManager->add(layer, pRenderEntity->getIND_Entity2d());
}

void EntityManager::renderEntities(){
	m_pEntity2dManager->renderEntities2d();
}

void EntityManager::updateEntities(){
	//update physical entities
	m_pPhysicalManager->updatePhysics();
	//update render entities
	int32 numEntity = 0;
	for(std::vector<RenderEntity*>::iterator renderEntity = m_renderEntityArray.begin();
		renderEntity != m_renderEntityArray.end();
		++renderEntity){
		PhysicalEntity* tmpPhysicalEntity = m_physicalEntityArray[numEntity];
		(*renderEntity)->setPosition(tmpPhysicalEntity->getPosX(), tmpPhysicalEntity->getPosY(), 0.f);
		numEntity++;
	}
}

}