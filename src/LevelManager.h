#ifndef _H_SYMPTOGEN_LEVELMANAGER_H
#define _H_SYMPTOGEN_LEVELMANAGER_H

#include "tinyxml2.h"
#include "EntityManager.h"

namespace Symp {

struct MetaEntity {
	const char* m_name;
	const char* m_texture_name;
	bool m_isVisible, m_isPhysic, m_flipHorizontaly, m_flipVerticaly;
	int m_posX, m_posY, m_scaleX;
	int m_scaleY, m_originX, m_originY;
	int m_tintR, m_tintG, m_tintB, m_tintA;
	int m_rotation;
	void reset();
};

struct LevelManager : public tinyxml2::XMLVisitor {
	LevelManager(EntityManager* entityManager);
	void loadLevel(const char* mapFileName);
	bool VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* attribute);
	bool VisitExit(const tinyxml2::XMLElement& element);
private:
	const char* m_pCurrentParsedFile;
	EntityManager* m_pEntityManager;
	MetaEntity m_currentMetaEntity;
	bool m_bIsParsingElementPosition;	// Used to identity to witch Item the values X and Y are corresponding 
	bool m_bIsParsingElementScale;		// Used to identity to witch Item the values X and Y are corresponding 
	bool m_bIsParsingElementOrigin;		// Used to identity to witch Item the values X and Y are corresponding 
};

} // End of namespace symptogen

#endif