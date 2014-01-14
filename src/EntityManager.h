#ifndef _H_SYMPTOGEN_RENDER_LINK_PHYSICAL_ENTITY_MANAGER_H_
#define _H_SYMPTOGEN_RENDER_LINK_PHYSICAL_ENTITY_MANAGER_H_

#include <vector>

#include <Indie.h>
#include <IND_Entity2d.h>
#include <IND_Entity2dManager.h>

#include "Render.h"
#include "RenderEntity.h"
#include "PhysicalEntity.h"
#include "PhysicalManager.h"

namespace Symp {

/**
	Manager of RenderEntities and PhysicalEntities.
*/
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
	//delete entities
	void deleteAllEntities();

	//getters
	std::vector<RenderEntity*> 		getRenderEntityArray() const {return m_renderEntityArray;}
	std::vector<PhysicalEntity*> 	getPhysicalEntityArray() const {return m_physicalEntityArray;}
	IND_Entity2dManager* 			getIND_Entity2dManager() const {return m_pEntity2dManager;}
	PhysicalManager*				getPhysicalManager() const {return m_pPhysicalManager;}
	int 							getNbEntities() const {return m_renderEntityArray.size();}
	RenderEntity*					getLastAddedRenderEntity() const {return m_renderEntityArray[m_renderEntityArray.size()-1];}
	PhysicalEntity*					getLastAddedPhysicalEntity() const {return m_physicalEntityArray[m_physicalEntityArray.size()-1];}

private:
	//all ***EntityArray have always the same size
	//this enable to always have a correspondance between the vectors.
	std::vector<RenderEntity*>		m_renderEntityArray;
	std::vector<PhysicalEntity*>	m_physicalEntityArray;

	IND_Entity2dManager*			m_pEntity2dManager;
	PhysicalManager*				m_pPhysicalManager;
};

}

#endif //_H_SYMPTOGEN_RENDER_LINK_PHYSICAL_ENTITY_MANAGER_H_