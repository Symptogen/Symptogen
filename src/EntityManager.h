#ifndef _H_SYMPTOGEN_RENDER_LINK_PHYSICAL_ENTITY_MANAGER_H_
#define _H_SYMPTOGEN_RENDER_LINK_PHYSICAL_ENTITY_MANAGER_H_

#include <vector>

#include <Indie.h>
#include <IND_Entity2d.h>
#include <IND_Entity2dManager.h>

#include "Render.h"
#include "RenderEntity.h"

/**
	Manager of RenderEntities and PhysicalEntities.
*/
class EntityManager {
public:
	EntityManager(Render* pRender);
	~EntityManager();

	bool addRenderEntity(RenderEntity* pRenderEntity, int layer);
	void renderEntities();

	// getters
	std::vector<RenderEntity*> getRenderEntityArray(){return m_RenderEntityArray;}
	IND_Entity2dManager* getIND_Entity2dManager(){return m_pEntity2dManager;}

private:
	std::vector<RenderEntity*>	m_RenderEntityArray;
	IND_Entity2dManager*		m_pEntity2dManager;
};

#endif //_H_SYMPTOGEN_RENDER_LINK_PHYSICAL_ENTITY_MANAGER_H_