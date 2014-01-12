#include "EntityManager.h"

EntityManager::EntityManager(Render* pRender){
	m_pEntity2dManager = new IND_Entity2dManager();
	m_pEntity2dManager->init(pRender->getIND_Render());
	RenderEntity::init(pRender);
}

EntityManager::~EntityManager(){
	m_pEntity2dManager->end();
    DISPOSE(m_pEntity2dManager);
	RenderEntity::end();
}

bool EntityManager::addRenderEntity(RenderEntity* pRenderEntity, int layer){
	m_EntityArray.push_back(make_pair<RenderEntity*, PhysicalEntity*>(pRenderEntity, NULL));
	return m_pEntity2dManager->add(layer, pRenderEntity->getIND_Entity2d());
}

void EntityManager::addPhysicalEntity(PhysicalEntity* pPhysicalEntity){
	m_EntityArray.push_back(make_pair<RenderEntity*, PhysicalEntity*>(NULL, pPhysicalEntity));
}

bool EntityManager::addEntity(RenderEntity* pRenderEntity, int layer, PhysicalEntity* pPhysicalEntity){
	m_EntityArray.push_back(make_pair<RenderEntity*, PhysicalEntity*>(pRenderEntity, pPhysicalEntity));
	return m_pEntity2dManager->add(layer, pRenderEntity->getIND_Entity2d());
}

void EntityManager::renderEntities(){
	m_pEntity2dManager->renderEntities2d();
}

void EntityManager::updateEntities(){
	//update RenderEntities
	//a renderManager is missing ?
}