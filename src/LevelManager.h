#ifndef _H_SYMPTOGEN_LEVELMANAGER_H
#define _H_SYMPTOGEN_LEVELMANAGER_H

#include "tinyxml2.h"

namespace symptogen {

struct EntityManager;

struct LevelManager {
	void loadLevel(EntityManager& em, const char* mapFileName); // Friend class with EntityManager. Defined in EntityManager.cpp.
	void parseChildren(tinyxml2::XMLNode* node, unsigned int depth);
private:
	const char* m_pCurrentLevelName;
};

} // End of namespace symptogen

#endif