#ifndef _H_SYMPTOGEN_LEVELMANAGER_H
#define _H_SYMPTOGEN_LEVELMANAGER_H

#include "tinyxml2.h"

namespace symptogen {

struct EntityManager;

struct LevelManager : public tinyxml2::XMLVisitor {
	LevelManager() : m_EntityManager(nullptr){}
	void loadLevel(EntityManager& em, const char* mapFileName); // Friend class with EntityManager
	bool VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* attribute);
	bool VisitExit(const tinyxml2::XMLElement& element);
	bool Visit(tinyxml2::XMLText* txt);
private:
	const char* m_pCurrentParsedFile;
	EntityManager& m_EntityManager;
	bool m_bIsParsingElementPosition;	// Used to identity to witch Item the values X and Y are corresponding 
	bool m_bIsParsingElementScale;		// Used to identity to witch Item the values X and Y are corresponding 
	bool m_bIsParsingElementOrigin;		// Used to identity to witch Item the values X and Y are corresponding 
};

} // End of namespace symptogen

#endif