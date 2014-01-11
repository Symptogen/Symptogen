#include "EntityManager.h"


namespace symptogen {

void EntityManager::printEntities() const {
	std::list<const char*>::const_iterator it;
	for(it = m_entitiesNames.begin(); it != m_entitiesNames.end(); ++it) {
		std::cerr << *it << std::endl;
	}
}

} // End of namespace symptogen
