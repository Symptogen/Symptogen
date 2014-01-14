#include "EntityManager.h"

namespace Symp {

EntityManager::EntityManager(Render* pRender){
	m_pEntity2dManager = new IND_Entity2dManager();
	m_pEntity2dManager->init(pRender->getIND_Render());
	RenderEntity::init(pRender);
 	m_pPhysicalManager = new PhysicalManager(0.f, 0.f);
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
	std::cerr << "update entities" << std::endl;
	//just update RenderEntities
	int32 numEntity = 0;
	std::vector<RenderEntity*>::iterator it;
	for(it = m_renderEntityArray.begin(); it != m_renderEntityArray.end(); ++it) {
		PhysicalEntity* tmpPhysicalEntity;
		try {
			std::cerr << "try to get entity " << numEntity << std::endl;
			PhysicalEntity* tmpPhysicalEntity = m_physicalEntityArray.at(numEntity);
			if(tmpPhysicalEntity == nullptr) {
				std::cerr << "EntityManager::updateEntities function : Physical Entity is null. it will not be update" << std::endl;
			}
			else {
				(*it)->setPosition(tmpPhysicalEntity->getPosition().x, tmpPhysicalEntity->getPosition().y, 0.f);
			}
		}
		catch(const std::out_of_range& oor) {
			std::cerr << "EntityManager::updateEntities function : out_of_range exception. The physical entity will not be update" << std::endl;
		}
		numEntity++;
	}
}

}