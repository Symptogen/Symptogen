#include "Parser.h"
#include <cstdio>
#include <sstream>

namespace Symp {

void MetaEntity::reset() {
	m_name = "";
	m_textureName = std::string("");
	m_isVisible = true;
	m_isPhysic = false;
	m_flipHorizontaly = false;
	m_flipVerticaly = false;
	m_posX = 0;
	m_posY = 0;
	m_scaleX = 0;
	m_scaleY = 0;
	m_originX = 0;
	m_originY = 0;
	m_tintR = 0;
	m_tintG = 0;
	m_tintB = 0;
	m_tintA = 0;
	m_rotation = 0;
}

LevelManager::LevelManager(EntityManager* entityManager) {
	m_pEntityManager = entityManager;
	m_currentMetaEntity = MetaEntity();
}



void LevelManager::loadLevel(const char* mapFileName) {

	tinyxml2::XMLDocument doc;
	int error = doc.LoadFile(mapFileName);
	if (error != tinyxml2::XML_NO_ERROR) {
		std::cerr << "Error when loading " << mapFileName << ". Code Error : " << error << std::endl;
		std::cerr << "The program will close." << std::endl;
		exit(EXIT_FAILURE);
	}

	m_pCurrentParsedFile = mapFileName;

	m_bIsParsingElementPosition = false;
	m_bIsParsingElementScale = false;
	m_bIsParsingElementOrigin = false;

    doc.Accept(this);

}

bool LevelManager::VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* attribute ) {

	std::string elementValue = element.Value();

	if(0 == elementValue.compare("Item")) {

		m_currentMetaEntity.reset();

		//bool isEntityVisible;
		if(strcmp(element.Attribute("Visible"), "true") == 0 || strcmp (element.Attribute("Visible"), "false") == 0 ) {
			m_currentMetaEntity.m_isVisible = strcmp (element.Attribute("Visible"), "true" ) == 0 ? true : false;
		}
		else {
			std::cerr << "Warning ! Parsing " << m_pCurrentParsedFile << " : The item " << element.Name() << " has no correct \"Visible\" attribute. The default value is true" << std::endl;
			m_currentMetaEntity.m_isVisible = true;
		}
	}
	else if(0 == elementValue.compare("Position")) {
		m_bIsParsingElementPosition = true;
	}
	else if(0 == elementValue.compare("Scale")) {
		m_bIsParsingElementScale = true;
	}
	else if(0 == elementValue.compare("Origin")) {
		m_bIsParsingElementOrigin = true;
	}
	else if(0 == elementValue.compare("Rotation")) {
		m_currentMetaEntity.m_rotation = strtod (element.GetText(), nullptr);
	}
	else if(0 == elementValue.compare("X")) {
		int x = atoi(element.GetText());
		if(m_bIsParsingElementPosition) {
			m_currentMetaEntity.m_posX = x;
		}
		else if(m_bIsParsingElementScale) {
			m_currentMetaEntity.m_scaleX = x;
		}
		else if(m_bIsParsingElementOrigin) {
			m_currentMetaEntity.m_originX = x;
		}
	}
	else if(0 == elementValue.compare("Y")) {
		int y = atoi(element.GetText());
		if(m_bIsParsingElementPosition) {
			m_currentMetaEntity.m_posY = y;
		}
		else if(m_bIsParsingElementScale) {
			m_currentMetaEntity.m_scaleY = y;
		}
		else if(m_bIsParsingElementOrigin) {
			m_currentMetaEntity.m_originY = y;
		}
	}
	else if(0 == elementValue.compare("Custom Properties")) {
		// ...
	}
	else if(0 == elementValue.compare("texture_filename")) {
		m_currentMetaEntity.m_textureName = element.GetText();
		size_t found = m_currentMetaEntity.m_textureName.rfind("\\");
		if(found != std::string::npos) {
			//std::cerr << "separator found" << std::endl;
			std::stringstream ss;
			ss << "../assets/map/sprites/";
			m_currentMetaEntity.m_textureName.replace(0, found+1, ss.str());
			//std::cerr << "replace : " << m_currentMetaEntity.m_textureName << std::endl;
		} else {
			m_currentMetaEntity.m_textureName = "";
		}
	}
	else if(0 == elementValue.compare("R")) {
		m_currentMetaEntity.m_tintR = atoi(element.GetText());
	}
	else if(0 == elementValue.compare("G")) {
		m_currentMetaEntity.m_tintG = atoi(element.GetText());
	}
	else if(0 == elementValue.compare("B")) {
		m_currentMetaEntity.m_tintB = atoi(element.GetText());
	}
	else if(0 == elementValue.compare("A")) {
		m_currentMetaEntity.m_tintA = atoi(element.GetText());
	}
	else if(0 == elementValue.compare("boolean")) { // Note : better to make a boolean class variable if we have different boolean in the xml mapfile.
		m_currentMetaEntity.m_isPhysic = strcmp(element.GetText(), "true" ) == 0 ? true : false;
	}
	else if(0 == elementValue.compare("FlipHorizontally")) {
		m_currentMetaEntity.m_flipHorizontaly = strcmp(element.GetText(), "true") == 0 ? true : false;
	}
	else if(0 == elementValue.compare("FlipVertically")) {
		m_currentMetaEntity.m_flipVerticaly = strcmp(element.GetText(),"true") == 0 ? true : false;
	}

	return true; // If you return false, no children of this node or its siblings will be visited.
}

bool LevelManager::VisitExit(const tinyxml2::XMLElement& element) {

	std::string elementValue = element.Value();

	if(0 == elementValue.compare("Position")) {
		m_bIsParsingElementPosition = false;
	}
	else if(0 == elementValue.compare("Scale")) {
		m_bIsParsingElementScale = false;
	}
	else if(0 == elementValue.compare("Origin")) {
		m_bIsParsingElementOrigin = false;
	}
	else if(0 == elementValue.compare("Item")) {
		
		m_bIsParsingElementPosition = false;
		m_bIsParsingElementScale = false;
		m_bIsParsingElementOrigin = false;

		// std::cerr << "create render entity" << std::endl;
		// std::cerr << "the current surface is : " << m_currentMetaEntity.m_textureName << std::endl;
		RenderEntity* rEntity = new RenderEntity(m_currentMetaEntity.m_textureName.c_str(), Symp::Surface);
		rEntity->setPosition(0.f, 300.f);
		rEntity->setHotSpot(0.5f, 0.5f); // TODO : calculate the hotspot using Origin and the width of the sprite.
		m_pEntityManager->addRenderEntity(rEntity, 0); // TODO : set the layer from XML
		if(m_currentMetaEntity.m_isPhysic) {
			// std::cerr << "create physic entity" << std::endl;
			PhysicalEntity* pEntity = new PhysicalEntity(m_pEntityManager->getPhysicalWorld()->getWorld(), b2Vec2((float32)m_currentMetaEntity.m_posX, (float32)m_currentMetaEntity.m_posY));
			m_pEntityManager->addPhysicalEntity(pEntity);
		}

	}

	return true; // If you return false, no children of this node or its siblings will be visited.
}

//------------------------------------------------------------------------------------------------//
/**
* @brief Parser constructor
* @param sPlayerDataPath the relative path to the xml file that contain the player's data
* @see GameManager
* @see ~Parser()
* @see Player
*/
Parser::Parser(std::string sPlayerDataPath) {
	m_sPlayerDataPath = sPlayerDataPath;
}

/**
* @brief Load the player data from the xml file
* @return std::pair a std::pair that contain the last player and the vector of the others players
* @see Parser
* @see ~Parser()
* @see savePlayerData()
* @see Player
*/
std::pair<Player*, std::vector<Player*>> Parser::loadPlayerData() {
	std::vector<Player*> playerVector;
	Player* lastPlayer;

	// New tinyxml doc
	tinyxml2::XMLDocument doc;
	bool loadingValue = doc.LoadFile(m_sPlayerDataPath.c_str());

	if (!loadingValue){

		//Load last player data
		std::string name = doc.FirstChildElement( "last" )->ToElement()->Attribute("name");
		int avatar = atoi(doc.FirstChildElement( "last" )->ToElement()->Attribute("avatar"));
		unsigned int level = atoi(doc.FirstChildElement( "last" )->ToElement()->Attribute("level"));
		//Create the player
		lastPlayer = new Player(name, avatar, level);

		//Load player list
		tinyxml2::XMLElement* root = doc.FirstChildElement("players");
		for(tinyxml2::XMLElement* e = root->FirstChildElement("player"); e != NULL; e = e->NextSiblingElement()){
    		name=e->ToElement()->Attribute("name");
			avatar = atoi(e->ToElement()->Attribute("avatar"));
			level = atoi(e->ToElement()->Attribute("level"));
			//Create the player and add it to the player vector
			Player* player = new Player(name, avatar, level);
			playerVector.push_back(player);
		}
	}
	return std::make_pair(lastPlayer, playerVector);
}

/**
* @brief Save the player's data into a xml file
* @param playerData a std::pair that contain the last player and the vector of the others players
* @see Parser
* @see ~Parser()
* @see loadPlayerData()
* @see Player
*/
void Parser::savePlayerData(std::pair<Player*, std::vector<Player*>> playerData){
	tinyxml2::XMLDocument doc;

		// XML Declaration
		tinyxml2::XMLDeclaration* declaration = doc.NewDeclaration();
		doc.InsertEndChild(declaration);

		// Save the last known player
		tinyxml2::XMLElement* player = doc.NewElement("last");
		player->SetAttribute ("name", playerData.first->getName().c_str());
		player->SetAttribute("avatar", playerData.first->getAvatarIndex());
		player->SetAttribute("level", playerData.first->getCurrentLevel());
		doc.InsertEndChild(player);

		// Players tag
		tinyxml2::XMLElement* players = doc.NewElement("players");

		// Save all the others players
		for (unsigned int i =0; i < playerData.second.size(); ++i){
			tinyxml2::XMLElement* element = doc.NewElement("player");
			element->SetAttribute ("name", playerData.second[i]->getName().c_str());
			element->SetAttribute("avatar", playerData.second[i]->getAvatarIndex());
			element->SetAttribute("level", playerData.second[i]->getCurrentLevel());
			players->InsertEndChild(element);
		}
		doc.InsertEndChild(players);

	bool returnValue = doc.SaveFile(m_sPlayerDataPath.c_str());
	if (returnValue){
		std::cout << "failed to save the players data in " << m_sPlayerDataPath << std::endl;
	}
}

} // End of namespace symptogen