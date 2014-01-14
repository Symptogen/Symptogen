#ifndef _H_SYMPTOGEN_LEVELMANAGER_H
#define _H_SYMPTOGEN_LEVELMANAGER_H

#include "tinyxml2.h"
#include "EntityManager.h"

namespace Symp {

struct LevelManager : public tinyxml2::XMLVisitor {
	LevelManager(EntityManager* entityManager);
	void loadLevel(const char* mapFileName);
	bool VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* attribute);
	bool VisitExit(const tinyxml2::XMLElement& element);
private:
	const char* m_pCurrentParsedFile;
	EntityManager* m_pEntityManager;
	bool m_bIsParsingElementPosition;	// Used to identity to witch Item the values X and Y are corresponding 
	bool m_bIsParsingElementScale;		// Used to identity to witch Item the values X and Y are corresponding 
	bool m_bIsParsingElementOrigin;		// Used to identity to witch Item the values X and Y are corresponding 
};

} // End of namespace symptogen

#endif