#ifndef _H_SYMPTOGEN_RENDER_LINK_PHYSICAL_ENTITY_MANAGER_H_
#define _H_SYMPTOGEN_RENDER_LINK_PHYSICAL_ENTITY_MANAGER_H_

#include <vector>
#include <utility> //pair

#include <Indie.h>
#include <IND_Entity2d.h>
#include <IND_Entity2dManager.h>

#include "Render.h"
#include "RenderEntity.h"
#include "PhysicalEntity.h"

/**
	Manager of RenderEntities and PhysicalEntities.
*/
typedef std::pair<RenderEntity*, PhysicalEntity*> Entity;

class EntityManager {
public:
	EntityManager(Render* pRender);
	~EntityManager();

	//add entities
	bool addRenderEntity(RenderEntity* pRenderEntity, int layer);
	void addPhysicalEntity(PhysicalEntity* pPhysicalEntity);
	bool addEntity(RenderEntity* pRenderEntity, int layer, PhysicalEntity* pPhysicalEntity);
	//display entities
	void renderEntities();
	//update entities
	void updateEntities();

	//getters
	std::vector<Entity> 	getEntityArray(){return m_EntityArray;}
	IND_Entity2dManager* 	getIND_Entity2dManager(){return m_pEntity2dManager;}
	int 					getNbEntities(){return m_EntityArray.size();}

private:
	std::vector<Entity>		m_EntityArray;
	IND_Entity2dManager*	m_pEntity2dManager;
};

#endif //_H_SYMPTOGEN_RENDER_LINK_PHYSICAL_ENTITY_MANAGER_H_