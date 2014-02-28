#ifndef _H_SYMPTOGEN_PARSER_H
#define _H_SYMPTOGEN_PARSER_H

#include "tinyxml.h"

#include "../menu/Player.h"
#include "../EntityManager.h"

namespace Symp {

struct MetaEntity {
	
	const char* m_name;
	std::string m_textureName;
	
	bool m_isVisible;
	bool m_isPhysic, m_isOnPhysicalLayer; // TODO : decide how we choose if an entity is physical
	
	int m_posX, m_posY, m_originX, m_originY, m_tintR, m_tintG, m_tintB, m_tintA;
	float m_scaleX, m_scaleY;
	int m_width, m_height;
	double m_rotation;

	bool m_flipHorizontaly, m_flipVerticaly;

	PhysicalType m_physicalType;

	//to know what power will be in the level
	static bool bIsSneezePower, bIsFeverPower, bIsHeadachePower;
	static bool bIsPowersSet;

	void reset();
};


struct LevelManager : public TiXmlVisitor {
	LevelManager();
	void loadLevel(const char* mapFileName);
	bool VisitEnter(const TiXmlElement& element, const TiXmlAttribute* attribute);
	bool VisitExit(const TiXmlElement& element);
private:
	const char* m_pCurrentParsedFile;
	MetaEntity m_currentMetaEntity;
	unsigned int m_layer;
	int entityCountInCurrentLayer;
	bool m_bIsParsingElementPosition;	// Used to identity to witch Item the values X and Y are corresponding 
	bool m_bIsParsingElementScale;		// Used to identity to witch Item the values X and Y are corresponding 
	bool m_bIsParsingElementOrigin;		// Used to identity to witch Item the values X and Y are corresponding 
	bool m_bIsParsingEnterArea;
	bool m_bIsParsingExitArea;
	bool m_bIsParsingCustomProperties;
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