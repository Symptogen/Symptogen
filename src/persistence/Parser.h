#ifndef _H_SYMPTOGEN_PARSER_H
#define _H_SYMPTOGEN_PARSER_H

#include "tinyxml.h"
#include <Box2D/Box2D.h>

#include "../menu/Player.h"
#include "../EntityManager.h"

namespace Symp {

/***********************************************************************************************************************************/
/*                                                          PARSE LEVEL                                                             */
/***********************************************************************************************************************************/

inline bool file_exists (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }  
}

/**
* The MetaEntity is used to store the information about one entity during the parsing of it.
* The reason of the existence of this strut is because the ParserLevel uses a visitor pattern to parse the data, so the different informations we get from different XML elements needs to exists over the process.
* When both entities (physical and render) are created, the MetaEntity is reseted to be ready to save datas of the next entity.
*/
struct MetaEntity {
	
	MetaEntity() : 
		m_bIsSneezePower(false),
		m_bIsFeverPower(false),
		m_bIsHeadachePower(false),
		m_bIsPowersSet(false) {}


	/**
	*	The name of the texture used to render the entity.
	*/
	std::string m_textureName;
	
	/**
	*	The visibility of the entity.
	*/
	bool m_isVisible;
	
	/**
	* The position of the center of the entity.
	*/
	int m_posX, m_posY, m_originX, m_originY;

	/**
	* The scale factor from the original texture size.
	*/
	float m_scaleX, m_scaleY;

	/**
	* The opacity of the entity
	*/
	float m_opacity;

	/**
	* The Z-axis rotation of the entity (in radian).
	*/
	float m_zRotation;

	/**
	* The filp states of the entity.
	*/
	bool m_flipHorizontaly, m_flipVerticaly;

	/**
	*	If the entity currently parsed is on a "physical layer", it means this entity has a Physical component. A physical layer is a layer with "physic" or "Physic0 name".
	*/
	bool m_isOnPhysicalLayer; // TODO : decide how we choose if an entity is physical

	/**
	* The physical type of the entity (flower, ground...)
	*/
	PhysicalType m_physicalType;

	/**
	* Different states to know is the different powers are available in this level.
	*/
	bool m_bIsSneezePower, m_bIsFeverPower, m_bIsHeadachePower;
	bool m_bIsPowersSet;

	/**
	* The width and the height of the entity (after scale)
	*/
	int m_width, m_height;

	/**
	* @brief Reset the MetaEntity in order to parse a new XML element.
	*/
	void reset();
};

/**
* @class ParserLevel class
* The #ParserLevel class is responsible to read and load a level from a XML file. It inherit from TiXmlVisitor class.
*/
struct ParserLevel : public TiXmlVisitor {

	/**
	* @brief #ParserLevel constructor.
	*/
	ParserLevel();

	/**
	* @brief Load a level from an XML file.
	* @param mapFileName : the name of the file to load
	* @return value to set the zoom of the game in GameManager
	*/
	float loadLevel(const char* mapFileName);

	/**
	* @brief Inherited from TinyXML Visitor class.
	* This method is called when the visitor enters an XML element.
	* The main use of the #VisitEnter method is to get the data into an XML.
	* It can be a "pure" data like position value or "state information" data like if the entity is on a physical layer.
	* @param mapFileName : the name of the file to load
	*/
	bool VisitEnter(const TiXmlElement& element, const TiXmlAttribute* attribute);

	/**
	* @brief Inherited from TinyXML Visitor class.
	* This method is called when the visitor leaves an XML element.
	* The aim of the method is to create the different entities depending on the #MetaEntity data.
	*/
	bool VisitExit(const TiXmlElement& element);

private:
	
	/**
	* The #MetaEntity used to store information over #VisitEnter calls.
	* @see MetaEntity
	*/
	MetaEntity 		m_currentMetaEntity;

	/**
	* The current layer we are adding #RenderEntity on
	*/
	unsigned int 	m_layer;

	/**
	* The number of entities in the current layer.
	*/
	int 			entityCountInCurrentLayer;
	
	/**
	* The following fields are used to identity the current parsed property.
	* Example 1 : when we encounters the element X or Y, we ca know which entity we are parsing by testing the values of this fields.
	* Example 2 : when we are parsing the "enter" or the "exit" entity, the process into #VisitExit method is different.
	*/
	bool m_bIsParsingElementPosition;
	bool m_bIsParsingElementScale;	
	bool m_bIsParsingElementOrigin;	
	bool m_bIsParsingEnterArea;
	bool m_bIsParsingExitArea;
	bool m_bIsParsingHotZone;
	bool m_bIsParsingColdZone;
	bool m_bIsParsingCustomProperties;

	/**
	* Used to always have the same dimensions for the levels
	*/
	float m_fScaleOfLevel;
};

/***********************************************************************************************************************************/
/*                                                        PARSE PLAYER                                                             */
/***********************************************************************************************************************************/

/**
* @class ParserPlayer class
* The #ParserPlayer class is responsible the reading and writtent of xml exernal data. Its main use is to manage the #Player
* data. The #ParserPlayer use the TinyXML library.
* @see GameManager
* @see Player
*/
class ParserPlayer {

public:

	/**
	* @brief #ParserPlayer constructor.
	*/
	ParserPlayer(std::string sPlayerDataPath);

	/**
	* @brief #ParserPlayer destructor.
	*/
	~ParserPlayer();

	/**
	* @brief Load persistent data from an XML file.
	* This method read the content of an XML file and create the differents players games. The current player is defined into the file.
	* @return a pair <#Player*, std::vector<#Player*>>. The first component is the current player and the vector of #Player* is corresponding to the list of the other players.
	*/
	std::pair<Player*, std::vector<Player*>> loadPlayerData();

	/**
	* @brief Save the input #Player datas into an XML file.
	* @param playerData : a pair of <#Player*, std::vector<#Player*>>. The first component is corresponding to the current player and the vector of #Player* is corresponding to the list of the other players.
	*/
	void savePlayerData(std::pair<Player*, std::vector<Player*>> playerData);

private:

	/**
	* The relative path to the xml file that contain the player's data
	*/
	std::string m_sPlayerDataPath;

};

} // End of namespace symptogen

#endif