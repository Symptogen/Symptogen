#ifndef _H_SYMPTOGEN_ENTITYMANAGER_H
#define _H_SYMPTOGEN_ENTITYMANAGER_H

#include <iostream>
#include <cstdlib>
#include <list>

#include "LevelManager.h"

namespace symptogen {

struct EntityManager {
private:
	std::list<std::pair<int, int>> m_entities; // TODO : replace by std::list<std::pair<RenderEntity, PhysicalEntity>> and the other stuff
	friend struct LevelManager;
};

} // End of namespace symptogen



#endif