#ifndef _H_SYMPTOGEN_ENTITYMANAGER_H
#define _H_SYMPTOGEN_ENTITYMANAGER_H

#include <iostream>
#include <cstdlib>
#include <list>

#include "LevelManager.h"

namespace symptogen {

struct EntityManager {
private:
	std::list<const char*> m_entitiesNames; // TODO : replace by std::list<std::pair<RenderEntity, PhysicalEntity>> or the other stuff
	friend struct LevelManager;
public:
	void printEntities() const;
};



} // End of namespace symptogen



#endif