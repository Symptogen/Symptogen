#ifndef _H_SYMPTOGEN_PARSER_H
#define _H_SYMPTOGEN_PARSER_H

#include "menu/Player.h"
#include "tinyxml.h"
#include "EntityManager.h"

namespace Symp {

struct MetaEntity {
	const char* m_name;
	std::string m_textureName;
	bool m_isVisible, m_isPhysic, m_flipHorizontaly, m_flipVerticaly;
	int m_posX, m_posY, m_scaleX;
	int m_scaleY, m_originX, m_originY;
	int m_tintR, m_tintG, m_tintB, m_tintA;
	double m_rotation;
	void reset();
};


struct LevelManager : public TiXmlVisitor {
	LevelManager(EntityManager* entityManager);
	void loadLevel(const char* mapFileName);
	bool VisitEnter(const TiXmlElement& element, const TiXmlAttribute* attribute);
	bool VisitExit(const TiXmlElement& element);
private:
	const char* m_pCurrentParsedFile;
	MetaEntity m_currentMetaEntity;
	bool m_bIsParsingElementPosition;	// Used to identity to witch Item the values X and Y are corresponding 
	bool m_bIsParsingElementScale;		// Used to identity to witch Item the values X and Y are corresponding 
	bool m_bIsParsingElementOrigin;		// Used to identity to witch Item the values X and Y are corresponding 
};

// ------------------------------------------------------------------------------------------------------//
/**
* @class Parser class
* The #Parser class is responsible the reading and writtent of xml exernal data. Its main use is to manage the #Player
* data. The #Parser use the TinyXML library.
* @see GameManager
* @see Player
*/
class Parser {
public:
	Parser(std::string sPlayerDataPath);
	~Parser();
	std::pair<Player*, std::vector<Player*>> loadPlayerData();
	void savePlayerData(std::pair<Player*, std::vector<Player*>> playerData);

private:
	std::string m_sPlayerDataPath; /** < the relative path to the xml file that contain the player's data */
};

} // End of namespace symptogen

#endif