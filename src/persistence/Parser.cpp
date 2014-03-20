#include <cstdio>
#include <sstream>

#include "Parser.h"
#include "../power/Sneeze.h"
#include "../power/Fever.h"
#include "../power/Headache.h"

#include "../sound/SoundManager.h"

#include <cmath>

namespace Symp {

/***********************************************************************************************************************************/
/*                                                      PARSE LEVEL                                                                */
/***********************************************************************************************************************************/

void MetaEntity::reset() {

	m_textureName = std::string("");
	m_isVisible = true;
	
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
	m_opacity = 255;
	m_zRotation = 0;
	
	m_physicalType = PhysicalType::Ground;
}

ParserLevel::ParserLevel() {
	m_currentMetaEntity = MetaEntity();
	m_fScaleOfLevel = -1;
}

float ParserLevel::loadLevel(const char* mapFileName) {

	TiXmlDocument doc;
	bool success = doc.LoadFile(mapFileName);
	if (!success) {
		std::cerr << "Error when loading " << mapFileName << ". " << doc.ErrorDesc() << std::endl;
		std::cerr << "The program will close." << std::endl;
		exit(EXIT_FAILURE);
	}

	m_bIsParsingElementPosition = false;
	m_bIsParsingElementScale = false;
	m_bIsParsingElementOrigin = false;
	m_bIsParsingEnterArea = false;
	m_bIsParsingExitArea = false;
	m_bIsParsingHotZone = false;
	m_bIsParsingColdZone = false;
	m_bIsParsingCustomProperties = false;
	m_bIsParsingCustomFever = false;
	m_bIsParsingBackgroundMusic = false;

	m_layer = 0;
    doc.Accept(this);

    m_currentMetaEntity.m_bIsPowersAlreadyCreated = false;
    m_currentMetaEntity.m_bIsBackgroundMusicAlreadyCreated = false;

    //to set the zoom of the game in GameManager
    return m_fScaleOfLevel / 6.f;
}

bool ParserLevel::VisitEnter(const TiXmlElement& element, const TiXmlAttribute* attribute ) {

	std::string elementValue = element.Value();

	if(0 == elementValue.compare("Layer")) {

		if(m_layer > 0) {
			m_layer++;
		}
		entityCountInCurrentLayer = 0;

		if((strcmp(element.Attribute("Name"), "physic") == 0)
			|| (strcmp(element.Attribute("Name"), "Physic") == 0)
			|| (strcmp(element.Attribute("Name"), "Physic2") == 0)
			|| (strcmp(element.Attribute("Name"), "physic2") == 0)) {
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
		m_currentMetaEntity.m_zRotation = atof(element.GetText());
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
	else if(0 == elementValue.compare("A")) {
		m_currentMetaEntity.m_opacity = atoi(element.GetText());
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
	else if(0 == elementValue.compare("FlipHorizontally")) {
		m_currentMetaEntity.m_flipHorizontaly = !strcmp(element.GetText(), "true");
	}
	else if(0 == elementValue.compare("FlipVertically")) {
		m_currentMetaEntity.m_flipVerticaly = !strcmp(element.GetText(), "true");
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
		//Custom Properties : Power (which power in the level)
		if(strcmp(element.Attribute("Name"), "Power") == 0) {
			m_bIsParsingCustomProperties = true;
		}
		//Custom Properties : set specific properties of powers
		if(strcmp(element.Attribute("Name"), "FeverStartedTemperature") == 0){
			m_bIsParsingCustomProperties = true;
			m_bIsParsingCustomFever = true;
		}
		//Custom Properties : sound of the level
		if(strcmp(element.Attribute("Name"), "BackgroundMusic") == 0){
			m_bIsParsingCustomProperties = true;
			m_bIsParsingBackgroundMusic = true;
		}
	}
	else if(0 == elementValue.compare("Width")) {
		m_currentMetaEntity.m_width = atoi(element.GetText());
	}
	else if(0 == elementValue.compare("Height")) {
		m_currentMetaEntity.m_height = atoi(element.GetText());
	}
	else if(0 == elementValue.compare("string") && m_bIsParsingCustomProperties) {
		std::string stCustomProperty = element.GetText();

		//PhysicalType
		if(stCustomProperty.compare("Dino") == 0)
			m_currentMetaEntity.m_physicalType = PhysicalType::Dino;
		else if(stCustomProperty.compare("Ground") == 0)
			m_currentMetaEntity.m_physicalType = PhysicalType::Ground;
		else if(stCustomProperty.compare("Flower") == 0)
			m_currentMetaEntity.m_physicalType = PhysicalType::Flower;
		else if(stCustomProperty.compare("MovableObject") == 0)
			m_currentMetaEntity.m_physicalType = PhysicalType::MovableObject;
		else if(stCustomProperty.compare("DestructibleObject") == 0)
			m_currentMetaEntity.m_physicalType = PhysicalType::DestructibleObject;
		else if(stCustomProperty.compare("Spikes") == 0)
			m_currentMetaEntity.m_physicalType = PhysicalType::Spikes;
		else if(stCustomProperty.compare("HotZone") == 0)
			m_currentMetaEntity.m_physicalType = PhysicalType::HotZone;
		else if(stCustomProperty.compare("ColdZone") == 0)
			m_currentMetaEntity.m_physicalType = PhysicalType::ColdZone;
		
		//Power
		else if(stCustomProperty.compare("Sneeze") == 0)
			m_currentMetaEntity.m_bIsSneezePower = true;
		else if(stCustomProperty.compare("Fever") == 0)
			m_currentMetaEntity.m_bIsFeverPower = true;
		else if(stCustomProperty.compare("Headache") == 0)
			m_currentMetaEntity.m_bIsHeadachePower = true;

		//Custom powers
		if(m_bIsParsingCustomFever){
			m_currentMetaEntity.m_fFeverSartedTemperature = atof(element.GetText());
		}

		//Sound of the level
		if(m_bIsParsingBackgroundMusic){
			m_currentMetaEntity.m_backgroundMusicOfLevel = stCustomProperty;
		}
	}

	return true; // If you return false, no children of this node or its siblings will be visited.
}

bool ParserLevel::VisitExit(const TiXmlElement& element) {

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
	else if(0 == elementValue.compare("Property")) {
		m_bIsParsingCustomProperties = false;
		m_bIsParsingCustomFever = false;
		m_bIsParsingBackgroundMusic = false;
	}
	else if(0 == elementValue.compare("Item")) {

		// Check for the enter area
		if(m_bIsParsingEnterArea) {
			int dinoCenterX = m_currentMetaEntity.m_posX + m_currentMetaEntity.m_width/2;
			int dinoCenterY = m_currentMetaEntity.m_posY + m_currentMetaEntity.m_height/2;
			int enterWidth = m_currentMetaEntity.m_width;
			m_fScaleOfLevel = enterWidth;

			EntityManager::getInstance()->addDino(dinoCenterX, dinoCenterY, enterWidth);
		}
		else if(m_bIsParsingExitArea) {
			int exitPosX = m_currentMetaEntity.m_posX + m_currentMetaEntity.m_width/2;
			int exitPosY = m_currentMetaEntity.m_posY + m_currentMetaEntity.m_height/2;
			EntityManager::getInstance()->setExitCoordinates(exitPosX, exitPosY);
		}
		else {
			 
			/*****************/
			/*     Render    */
			/*****************/
			std::vector<RenderEntity*> renderEntityArray;
			RenderEntity* rEntityBasic = nullptr;

			// Animation for Flower
			if(m_currentMetaEntity.m_physicalType == PhysicalType::Flower) {

				// Normal image
				rEntityBasic = new RenderEntity("../assets/animation/Flower2.xml", Symp::Animation);

				rEntityBasic->setHotSpot(0.5, 0.5);
				rEntityBasic->setPosition(m_currentMetaEntity.m_posX, m_currentMetaEntity.m_posY);
				rEntityBasic->setScale(m_currentMetaEntity.m_scaleX, m_currentMetaEntity.m_scaleY);
				rEntityBasic->setOpacity(m_currentMetaEntity.m_opacity);
				renderEntityArray.insert(renderEntityArray.begin() + FlowerAction::Normal, rEntityBasic);

				// Animation when we collide flower
				RenderEntity* rEntity = new RenderEntity("../assets/animation/Flower.xml", Symp::Animation);

				rEntity->setHotSpot(0.5, 0.5);
				rEntity->setPosition(m_currentMetaEntity.m_posX, m_currentMetaEntity.m_posY);
				rEntity->setScale(m_currentMetaEntity.m_scaleX, m_currentMetaEntity.m_scaleY);
				rEntity->setOpacity(m_currentMetaEntity.m_opacity);
				rEntity->flipHorizontaly(m_currentMetaEntity.m_flipHorizontaly);
				rEntity->flipVerticaly(m_currentMetaEntity.m_flipVerticaly);
				renderEntityArray.insert(renderEntityArray.begin() + FlowerAction::CollideDino, rEntity);

				// Show the normal image
				renderEntityArray[FlowerAction::Normal]->setShow(true);
				renderEntityArray[FlowerAction::CollideDino]->setShow(false);

			}
			// Animation for DestructibleObject
			else if(m_currentMetaEntity.m_physicalType == PhysicalType::DestructibleObject) {
				rEntityBasic = new RenderEntity(m_currentMetaEntity.m_textureName.c_str(), Symp::Surface);
				rEntityBasic->setHotSpot(0.5, 0.5);
				rEntityBasic->setPosition(m_currentMetaEntity.m_posX, m_currentMetaEntity.m_posY);
				rEntityBasic->setScale(m_currentMetaEntity.m_scaleX, m_currentMetaEntity.m_scaleY);
				rEntityBasic->setOpacity(m_currentMetaEntity.m_opacity);
				rEntityBasic->setAngleXYZ(0, 0, m_currentMetaEntity.m_zRotation*360/(2*PI));
				rEntityBasic->flipHorizontaly(m_currentMetaEntity.m_flipHorizontaly);
				rEntityBasic->flipVerticaly(m_currentMetaEntity.m_flipVerticaly);
				renderEntityArray.insert(renderEntityArray.begin() + DestructibleObjectAction::NormalBox, rEntityBasic);
				rEntityBasic->setShow(true);

				// Animation when flames
				RenderEntity* rEntityByFlames = new RenderEntity("../assets/animation/DestructibleObjectByFlames.xml", Symp::Animation);
				rEntityByFlames->setHotSpot(0.5, 0.5);
				rEntityByFlames->setPosition(m_currentMetaEntity.m_posX, m_currentMetaEntity.m_posY);
				rEntityByFlames->setScale(m_currentMetaEntity.m_scaleX, m_currentMetaEntity.m_scaleY);
				rEntityByFlames->setOpacity(m_currentMetaEntity.m_opacity);
				rEntityByFlames->flipHorizontaly(m_currentMetaEntity.m_flipHorizontaly);
				rEntityByFlames->flipVerticaly(m_currentMetaEntity.m_flipVerticaly);
				renderEntityArray.insert(renderEntityArray.begin() + DestructibleObjectAction::ByFlames, rEntityByFlames);
				rEntityByFlames->setShow(false);

				// Animation when shivering
				RenderEntity* rEntityByShivering = new RenderEntity("../assets/animation/DestructibleObjectByShivering.xml", Symp::Animation);
				rEntityByShivering->setHotSpot(0.5, 0.5);
				rEntityByShivering->setPosition(m_currentMetaEntity.m_posX, m_currentMetaEntity.m_posY);
				rEntityByShivering->setScale(m_currentMetaEntity.m_scaleX, m_currentMetaEntity.m_scaleY);
				rEntityByShivering->setOpacity(m_currentMetaEntity.m_opacity);
				rEntityByShivering->flipHorizontaly(m_currentMetaEntity.m_flipHorizontaly);
				rEntityByShivering->flipVerticaly(m_currentMetaEntity.m_flipVerticaly);
				renderEntityArray.insert(renderEntityArray.begin() + DestructibleObjectAction::ByShivering, rEntityByShivering);
				rEntityByShivering->setShow(false);
			}
			else {
				rEntityBasic = new RenderEntity(m_currentMetaEntity.m_textureName.c_str(), Symp::Surface);

				// TODO : calculate the hotspot using Origin and the width and the scale factor of the sprite.
				rEntityBasic->setHotSpot(0.5, 0.5);
				rEntityBasic->setPosition(m_currentMetaEntity.m_posX, m_currentMetaEntity.m_posY);
				rEntityBasic->setScale(m_currentMetaEntity.m_scaleX, m_currentMetaEntity.m_scaleY);
				rEntityBasic->setOpacity(m_currentMetaEntity.m_opacity);
				rEntityBasic->setAngleXYZ(0, 0, m_currentMetaEntity.m_zRotation*360/(2*PI));
				rEntityBasic->flipHorizontaly(m_currentMetaEntity.m_flipHorizontaly);
				rEntityBasic->flipVerticaly(m_currentMetaEntity.m_flipVerticaly);
				renderEntityArray.push_back(rEntityBasic);
			}
			
			/*****************/
			/*   Physical    */
			/*****************/
			PhysicalEntity* pEntity = NULL;
			if(m_currentMetaEntity.m_isOnPhysicalLayer) {
				float32 physicalWidth = rEntityBasic->getWidth();
				float32 physicalHeight = rEntityBasic->getHeight();
				float physicalCenterX = m_currentMetaEntity.m_posX;
				float physicalCenterY = m_currentMetaEntity.m_posY;
				
				pEntity = new PhysicalEntity(
					EntityManager::getInstance()->getPhysicalWorld()->getWorld(), 
					b2Vec2(physicalCenterX, physicalCenterY), 
					b2Vec2(physicalWidth, physicalHeight),
					m_currentMetaEntity.m_physicalType
					);

				// Set custom shape if available

				std::string xmlCollisionFileName = m_currentMetaEntity.m_textureName;
				size_t found = m_currentMetaEntity.m_textureName.rfind("/");
				if(found != std::string::npos) {
					std::stringstream ss;
					ss << "../assets/collision/";
					xmlCollisionFileName.replace(0, found+1, ss.str());
					xmlCollisionFileName.replace(xmlCollisionFileName.end()-3, xmlCollisionFileName.end(), "xml");
				}

				if(file_exists(xmlCollisionFileName)) {
					pEntity->setCustomPolygonHitbox(xmlCollisionFileName.c_str());
				}

				// Set mass
				if(m_currentMetaEntity.m_physicalType == PhysicalType::MovableObject)
					pEntity->setMass(1.f, 0.f);
				else
					pEntity->setMass(0.f, 100.f);
			}
			/*****************/
			/*     Sound     */
			/*****************/
			std::vector<SoundEntity*> soundEntityArray;

			// DestructibleObject
			if(m_currentMetaEntity.m_physicalType == PhysicalType::DestructibleObject) {
				soundEntityArray.insert(soundEntityArray.begin() + DestructibleObjectAction::NormalBox, NULL);

				SoundEntity* sEntityByFlames = new SoundEntity("../assets/audio/destructableObject.ogg");
				soundEntityArray.insert(soundEntityArray.begin() + DestructibleObjectAction::ByFlames, sEntityByFlames);

				SoundEntity* sEntityByShivering = new SoundEntity("../assets/audio/destructableObject.ogg");
				soundEntityArray.insert(soundEntityArray.begin() + DestructibleObjectAction::ByShivering, sEntityByShivering);
			}
			
			//Bug in IndieLib : no more than 10 entities in the same layer !
			if(entityCountInCurrentLayer > 10) {
				entityCountInCurrentLayer = 0;
				m_layer++;
			}

			/*****************/
			/*  Add entity   */
			/*****************/
			bool result = EntityManager::getInstance()->addEntity(renderEntityArray, m_layer, pEntity, soundEntityArray);
			entityCountInCurrentLayer++;

			if(!result) {
				fprintf(stderr, "Error when adding Entity.\n");
			}


			/*****************/
			/*     Power     */
			/*****************/
			if(!m_currentMetaEntity.m_bIsPowersAlreadyCreated) {
				if(m_currentMetaEntity.m_bIsSneezePower) {
					Sneeze* pSneeze = new Sneeze();
				 	pSneeze->setRepulsionStrength(500);
				 	pSneeze->setTimeToTriggerRandomSneeze(5);
					EntityManager::getInstance()->addPower(pSneeze, PowerType::SneezeType);
					m_currentMetaEntity.m_bIsPowersAlreadyCreated = true;
				}
				if(m_currentMetaEntity.m_bIsFeverPower) {
 					Fever* pFever = new Fever();
 					pFever->setCurrentTemperature(m_currentMetaEntity.m_fFeverSartedTemperature);
 					EntityManager::getInstance()->addPower(pFever, PowerType::FeverType);
					m_currentMetaEntity.m_bIsPowersAlreadyCreated = true;

					//add thermometer entity
					EntityManager::getInstance()->addThermometer();
				}
				if(m_currentMetaEntity.m_bIsHeadachePower){
					Headache* pHeadache = new Headache();
					pHeadache->setTimeToTriggerRandomHeadache(5);
 					EntityManager::getInstance()->addPower(pHeadache, PowerType::HeadacheType);
					m_currentMetaEntity.m_bIsPowersAlreadyCreated = true;
				}
			}

			/**************************/
			/*     Background music   */
			/**************************/
			if(!m_currentMetaEntity.m_bIsBackgroundMusicAlreadyCreated) {
				std::vector<SoundEntity*> soundEntityArray;
				std::string stFilePath;
				stFilePath.append("../assets/audio/backgroundMusic/");
				stFilePath.append(m_currentMetaEntity.m_backgroundMusicOfLevel);
				SoundEntity* sEntity = new SoundEntity(stFilePath.c_str());
				soundEntityArray.push_back(sEntity);

				bool result = EntityManager::getInstance()->addSoundEntity(soundEntityArray);
				if(!result) {
					fprintf(stderr, "Error when adding Entity.\n");
				}

				m_currentMetaEntity.m_bIsBackgroundMusicAlreadyCreated = true;
			}
		}

		m_bIsParsingEnterArea = false;
		m_bIsParsingExitArea = false;
		m_bIsParsingHotZone = false;
		m_bIsParsingColdZone = false;
	}

	return true; // If you return false, no children of this node or its siblings will be visited.
}

/***********************************************************************************************************************************/
/*                                                        PARSE PLAYER                                                             */
/***********************************************************************************************************************************/

/**
* @brief ParserPlayer constructor
* @param sPlayerDataPath the relative path to the xml file that contain the player's data
* @see GameManager
* @see ~ParserPlayer()
* @see Player
*/
ParserPlayer::ParserPlayer(std::string sPlayerDataPath) {
	m_sPlayerDataPath = sPlayerDataPath;
}

/**
* @brief Load the player data from the xml file
* @return std::pair a std::pair that contain the last player and the vector of the others players
* @see ParserPlayer
* @see ~ParserPlayer()
* @see savePlayerData()
* @see Player
*/
std::pair<Player*, std::vector<Player*>> ParserPlayer::loadPlayerData() {
	std::vector<Player*> playerVector;
	int lastPlayerId;
	Player* lastPlayer;

	// New tinyxml doc
	TiXmlDocument doc;
	bool loadingValue = doc.LoadFile(m_sPlayerDataPath.c_str());

	if (loadingValue){
		//Load last player data
		TiXmlElement *element = 0;
		element = doc.FirstChildElement("last");
 
 		if(element->Attribute("id") != NULL){
			lastPlayerId = atoi(element->Attribute("id"));
		}

		//Load player list
		TiXmlElement* root = doc.FirstChildElement("players");
		for(TiXmlElement* e = root->FirstChildElement("player"); e != NULL; e = e->NextSiblingElement()){
			int id = atoi(e->ToElement()->Attribute("id"));
			std::string name=e->ToElement()->Attribute("name");
			int avatar = atoi(e->ToElement()->Attribute("avatar"));
			unsigned int level = atoi(e->ToElement()->Attribute("level"));
			//Create the player and add it to the player vector
			Player* player = new Player(id, name, avatar, level);
			playerVector.push_back(player);
			if(id ==lastPlayerId){
				lastPlayer = player;
			}

		}
	}else{
		std::cout << "No player data to load. " << std::endl;
	}
	return std::make_pair(lastPlayer, playerVector);
}

/**
* @brief Save the player's data into a xml file
* @param playerData a std::pair that contain the last player and the vector of the others players
* @see ParserPlayer
* @see ~ParserPlayer()
* @see loadPlayerData()
* @see Player
*/
void ParserPlayer::savePlayerData(std::pair<Player*, std::vector<Player*>> playerData){
	
		std::cout << "saving ..." << std::endl;
		TiXmlDocument doc;

		// XML Declaration
		TiXmlDeclaration * declaration = new TiXmlDeclaration( "1.0", "", "" );
		doc.LinkEndChild(declaration);

		// Save the last known player
		TiXmlElement* player = new TiXmlElement("last");
		doc.LinkEndChild(player);

		if(!playerData.second.empty()){
			
			player->SetAttribute ("id", playerData.first->getId());
			doc.LinkEndChild(player);
		}

		// Players tag
		TiXmlElement* players = new TiXmlElement("players");

		// Save all the others players
		for (unsigned int i =0; i < playerData.second.size(); ++i) {
			TiXmlElement* player = new TiXmlElement("player");
			player->SetAttribute ("id", playerData.second[i]->getId());
			player->SetAttribute ("name", playerData.second[i]->getName().c_str());
			player->SetAttribute("avatar", playerData.second[i]->getAvatarIndex());
			player->SetAttribute("level", playerData.second[i]->getCurrentLevel());
			players->LinkEndChild(player);
		}
		doc.LinkEndChild(players);

	bool returnValue = doc.SaveFile(m_sPlayerDataPath.c_str());
	if (!returnValue){
		std::cout << "failed to save the players data in " << m_sPlayerDataPath << std::endl;
	}

}

} // End of namespace symptogen