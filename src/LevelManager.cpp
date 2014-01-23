#include "LevelManager.h"
#include "EntityManager.h"
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

		bool isEntityVisible;
		if(element.Attribute("Visible") == "true" || element.Attribute("Visible") == false ) {
			m_currentMetaEntity.m_isVisible = element.Attribute("Visible") == "true" ? true : false;
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
			std::cerr << "separator found" << std::endl;
			std::stringstream ss;
			ss << "../assets/map/sprites/";
			m_currentMetaEntity.m_textureName.replace(0, found+1, ss.str());
			std::cerr << "replace : " << m_currentMetaEntity.m_textureName << std::endl;
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
		m_currentMetaEntity.m_isPhysic = element.GetText() == "true" ? true : false;
	}
	else if(0 == elementValue.compare("FlipHorizontally")) {
		m_currentMetaEntity.m_flipHorizontaly = element.GetText() == "true" ? true : false;
	}
	else if(0 == elementValue.compare("FlipVertically")) {
		m_currentMetaEntity.m_flipVerticaly = element.GetText() == "true" ? true : false;
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

		std::cerr << "create render entity" << std::endl;
		std::cerr << "the current surface is : " << m_currentMetaEntity.m_textureName << std::endl;
		RenderEntity* rEntity = new RenderEntity(m_currentMetaEntity.m_textureName.c_str(), Symp::Surface);
		rEntity->setPosition(0.f, 300.f, 0.f);
		rEntity->setHotSpot(0.5f, 0.5f); // TODO : calculate the hotspot using Origin and the width of the sprite.
		m_pEntityManager->addRenderEntity(rEntity, 0); // TODO : set the layer from XML
		if(m_currentMetaEntity.m_isPhysic) {
			std::cerr << "create physic entity" << std::endl;
			PhysicalEntity* pEntity = new PhysicalEntity(m_pEntityManager->getPhysicalManager()->getWorld(), b2Vec2(m_currentMetaEntity.m_posX, m_currentMetaEntity.m_posY));
			m_pEntityManager->addPhysicalEntity(pEntity);
		}

	}

	return true; // If you return false, no children of this node or its siblings will be visited.
}



} // End of namespace symptogen