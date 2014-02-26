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
	m_width = 0;
	m_height = 0;
	m_originX = 0;
	m_originY = 0;
	m_tintR = 0;
	m_tintG = 0;
	m_tintB = 0;
	m_tintA = 0;
	m_rotation = 0;
	m_physicalType = PhysicalType::Ground;
}

LevelManager::LevelManager() {
	m_currentMetaEntity = MetaEntity();
}



void LevelManager::loadLevel(const char* mapFileName) {

	fprintf(stderr, "load level %s\n", mapFileName);

	TiXmlDocument doc;
	bool success = doc.LoadFile(mapFileName);
	if (!success) {
		std::cerr << "Error when loading " << mapFileName << ". " << doc.ErrorDesc() << std::endl;
		std::cerr << "The program will close." << std::endl;
		exit(EXIT_FAILURE);
	}

	m_pCurrentParsedFile = mapFileName;

	m_bIsParsingElementPosition = false;
	m_bIsParsingElementScale = false;
	m_bIsParsingElementOrigin = false;
	m_bIsParsingEnterArea = false;
	m_bIsParsingExitArea = false;
	m_bIsParsingCustomProperties = false;

	m_layer = 0;

    doc.Accept(this);

}

bool LevelManager::VisitEnter(const TiXmlElement& element, const TiXmlAttribute* attribute ) {

	std::string elementValue = element.Value();

	if(0 == elementValue.compare("Layer")) {

		if(m_layer > 0) {
			m_layer++;
		}
		entityCountInCurrentLayer = 0;

		if((strcmp(element.Attribute("Name"), "physic") == 0) || (strcmp(element.Attribute("Name"), "Physic") == 0)) {
			m_currentMetaEntity.m_isOnPhysicalLayer = true;
		}
		else {
			m_currentMetaEntity.m_isOnPhysicalLayer = false;
		}

	}
	else if(0 == elementValue.compare("Item")) {

		m_currentMetaEntity.reset();

		// Check entity visibility
		if(strcmp(element.Attribute("Visible"), "true") == 0 || strcmp (element.Attribute("Visible"), "false") == 0 ) {
			m_currentMetaEntity.m_isVisible = strcmp (element.Attribute("Visible"), "true" ) == 0 ? true : false;
		}
		else {
			std::cerr << "Warning ! Parsing " << m_pCurrentParsedFile << " : The item " << element.Value() << " has no correct \"Visible\" attribute. The default value is true" << std::endl;
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

		if(m_bIsParsingElementPosition) {
			m_currentMetaEntity.m_posX = atoi(element.GetText());
		}
		else if(m_bIsParsingElementScale) {
			m_currentMetaEntity.m_scaleX = atof(element.GetText());
		}
		else if(m_bIsParsingElementOrigin) {
			m_currentMetaEntity.m_originX = atoi(element.GetText());
		}

	}
	else if(0 == elementValue.compare("Y")) {

		if(m_bIsParsingElementPosition) {
			m_currentMetaEntity.m_posY = atoi(element.GetText());
		}
		else if(m_bIsParsingElementScale) {
			m_currentMetaEntity.m_scaleY = atof(element.GetText());
		}
		else if(m_bIsParsingElementOrigin) {
			m_currentMetaEntity.m_originY = atoi(element.GetText());
		}

	}
	else if(0 == elementValue.compare("texture_filename")) {

		m_currentMetaEntity.m_textureName = element.GetText();
		size_t found = m_currentMetaEntity.m_textureName.rfind("\\");
		if(found != std::string::npos) {
			std::stringstream ss;
			ss << "../assets/map/sprites/";
			m_currentMetaEntity.m_textureName.replace(0, found+1, ss.str());
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
	else if(0 == elementValue.compare("FlipHorizontally")) {
		m_currentMetaEntity.m_flipHorizontaly = strcmp(element.GetText(), "true") == 0 ? true : false;
	}
	else if(0 == elementValue.compare("FlipVertically")) {
		m_currentMetaEntity.m_flipVerticaly = strcmp(element.GetText(),"true") == 0 ? true : false;
	}
	else if(0 == elementValue.compare("Property")) {

		// Check for Enter Area
		if(strcmp(element.Attribute("Name"), "enter") == 0) {
			m_bIsParsingEnterArea = true;
		}
		// Check for Exit Area
		if(strcmp(element.Attribute("Name"), "exit") == 0) {
			m_bIsParsingExitArea = true;
		}
		//Custom Properties : PhysicalType
		if(strcmp(element.Attribute("Name"), "PhysicalType") == 0) {
			m_bIsParsingCustomProperties = true;
		}

	}
	else if(0 == elementValue.compare("Width")) {
		m_currentMetaEntity.m_width = atoi(element.GetText());
	}
	else if(0 == elementValue.compare("Height")) {
		m_currentMetaEntity.m_height = atoi(element.GetText());
	}

	else if(0 == elementValue.compare("string") && m_bIsParsingCustomProperties) {
		std::string stPhysicalType = element.GetText();
		if(stPhysicalType.compare("Dino") == 0)
			m_currentMetaEntity.m_physicalType = PhysicalType::Dino;
		else if(stPhysicalType.compare("Ground") == 0)
			m_currentMetaEntity.m_physicalType = PhysicalType::Ground;
		else if(stPhysicalType.compare("Flower") == 0)
			m_currentMetaEntity.m_physicalType = PhysicalType::Flower;
		else if(stPhysicalType.compare("MovableObject") == 0)
			m_currentMetaEntity.m_physicalType = PhysicalType::MovableObject;
		else //if the physical type is unknow, we will consider it as a ground.
			m_currentMetaEntity.m_physicalType = PhysicalType::Ground;
	}

	return true; // If you return false, no children of this node or its siblings will be visited.
}

bool LevelManager::VisitExit(const TiXmlElement& element) {

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
	if(0 == elementValue.compare("Property")) {
		m_bIsParsingCustomProperties = false;
	}
	else if(0 == elementValue.compare("Item")) {

		// Check for the enter area
		if(m_bIsParsingEnterArea) {
			int dinoCenterX = m_currentMetaEntity.m_posX + m_currentMetaEntity.m_width/2;
			int dinoCenterY = m_currentMetaEntity.m_posY + m_currentMetaEntity.m_height/2;
			int enterHeight = m_currentMetaEntity.m_height;
			EntityManager::getInstance()->addDino(dinoCenterX, dinoCenterY, enterHeight);
		}
		else if(m_bIsParsingExitArea) {
			// ...
		}
		else {
			 
			/*****************/
			/*     Render    */
			/*****************/
			std::vector<RenderEntity*> renderEntityArray;
			RenderEntity* rEntity = new RenderEntity(m_currentMetaEntity.m_textureName.c_str(), Symp::Surface);
			// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
			rEntity->setHotSpot(0.5, 0.5);
			rEntity->setScale(m_currentMetaEntity.m_scaleX, m_currentMetaEntity.m_scaleY);
			renderEntityArray.push_back(rEntity);
			
			/*****************/
			/*   Physical    */
			/*****************/
			PhysicalEntity* pEntity = NULL;
			if(m_currentMetaEntity.m_isOnPhysicalLayer) {
				float32 physicalWidth = rEntity->getWidth();
				float32 physicalHeight = rEntity->getHeight();
				float physicalCenterX = m_currentMetaEntity.m_posX + physicalWidth/2;
				float physicalCenterY = m_currentMetaEntity.m_posY + physicalHeight/2;
				//TODO : add balise in xml to know the PhysicalType of the entity (for know all is PhysicalType::Ground).
				pEntity = new PhysicalEntity(
					EntityManager::getInstance()->getPhysicalWorld()->getWorld(), 
					b2Vec2(physicalCenterX, physicalCenterY), 
					b2Vec2(physicalWidth, physicalHeight),
					m_currentMetaEntity.m_physicalType
					);		
			}

			/*****************/
			/*     Sound     */
			/*****************/
			std::vector<SoundEntity*> soundEntityArray;
			
			if(entityCountInCurrentLayer > 10) {
				entityCountInCurrentLayer = 0;
				m_layer++;
			}

			bool result = EntityManager::getInstance()->addEntity(renderEntityArray, m_layer, pEntity, soundEntityArray);
			fprintf(stderr, "%d\n", entityCountInCurrentLayer);
			entityCountInCurrentLayer++;

			if(!result) {
				fprintf(stderr, "error when adding Entity\n");
			}

		}

		

		m_bIsParsingElementPosition = false;
		m_bIsParsingElementScale = false;
		m_bIsParsingElementOrigin = false;
		m_bIsParsingEnterArea = false;
		m_bIsParsingExitArea = false;


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
	TiXmlDocument doc;
	bool loadingValue = doc.LoadFile(m_sPlayerDataPath.c_str());

	if (loadingValue){
		//Load last player data
		TiXmlElement *element = 0;
		element = doc.FirstChildElement("last");

		// std::string name = element->Attribute("name");
		std::string name = element->Attribute("avatar");
		int avatar = atoi(doc.FirstChildElement("last")->ToElement()->Attribute("avatar"));
		unsigned int level = atoi(doc.FirstChildElement("last")->ToElement()->Attribute("level"));
		//Create the player
		lastPlayer = new Player(name, avatar, level);


		//Load player list
		TiXmlElement* root = doc.FirstChildElement("players");
		for(TiXmlElement* e = root->FirstChildElement("player"); e != NULL; e = e->NextSiblingElement()){
			std::string name=e->ToElement()->Attribute("name");
			int avatar = atoi(e->ToElement()->Attribute("avatar"));
			unsigned int level = atoi(e->ToElement()->Attribute("level"));
			//Create the player and add it to the player vector
			Player* player = new Player(name, avatar, level);
			playerVector.push_back(player);

		}
	}else{
		std::cout << "Error while loading the players data." << std::endl;
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
	
		TiXmlDocument doc;

		// XML Declaration
		TiXmlDeclaration * declaration = new TiXmlDeclaration( "1.0", "", "" );
		doc.LinkEndChild(declaration);

		// Save the last known player
		TiXmlElement* player = new TiXmlElement("last");
		doc.LinkEndChild(player);

		player->SetAttribute ("name", playerData.first->getName().c_str());
		player->SetAttribute("avatar", playerData.first->getAvatarIndex());
		player->SetAttribute("level", playerData.first->getCurrentLevel());
		doc.LinkEndChild(player);

		// Players tag
		TiXmlElement* players = new TiXmlElement("players");

		// Save all the others players
		for (unsigned int i =0; i < playerData.second.size(); ++i) {
			TiXmlElement* player = new TiXmlElement("player");
			player->SetAttribute ("name", playerData.second[i]->getName().c_str());
			player->SetAttribute("avatar", playerData.second[i]->getAvatarIndex());
			player->SetAttribute("level", playerData.second[i]->getCurrentLevel());
			doc.LinkEndChild(player);
		}
		doc.LinkEndChild(players);

	bool returnValue = doc.SaveFile(m_sPlayerDataPath.c_str());
	if (returnValue){
		std::cout << "failed to save the players data in " << m_sPlayerDataPath << std::endl;
	}

}

} // End of namespace symptogen