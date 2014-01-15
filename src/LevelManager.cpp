#include "LevelManager.h"
#include "EntityManager.h"


namespace Symp {

void MetaEntity::reset() {
	m_name = "";
	m_texture_name = "";
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
		std::cerr << "Error when loading " << mapFileName << ". " << error << std::endl;
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
	
	if(element.Value() == "Item") {

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
	else if(element.Value() == "Position") {
		m_bIsParsingElementPosition = true;
	}
	else if(element.Value() == "Scale") {
		m_bIsParsingElementScale = true;
	}
	else if(element.Value() == "Origin") {
		m_bIsParsingElementOrigin = true;
	}
	else if(element.Value() == "Rotation") {
		m_currentMetaEntity.m_rotation = strtod (element.GetText(), nullptr);
	}
	else if(element.Value() == "X") {
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
	else if(element.Value() == "Y") {
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
	else if(element.Value() == "Custom Properties") {
		// ...
	}
	else if(element.Value() == "texture_filename") {
		m_currentMetaEntity.m_texture_name = element.GetText();
	}
	else if(element.Value() == "R") {
		m_currentMetaEntity.m_tintR = atoi(element.GetText());
	}
	else if(element.Value() == "G") {
		m_currentMetaEntity.m_tintG = atoi(element.GetText());
	}
	else if(element.Value() == "B") {
		m_currentMetaEntity.m_tintB = atoi(element.GetText());
	}
	else if(element.Value() == "A") {
		m_currentMetaEntity.m_tintA = atoi(element.GetText());
	}
	else if(element.Value() == "boolean") { // Note : better to make a boolean class variable if we have different boolean in the xml mapfile.
		m_currentMetaEntity.m_isPhysic = element.GetText() == "true" ? true : false;
	}
	else if(element.Value() == "FlipHorizontally") {
		m_currentMetaEntity.m_flipHorizontaly = element.GetText() == "true" ? true : false;
	}
	else if(element.Value() == "FlipVertically") {
		m_currentMetaEntity.m_flipVerticaly = element.GetText() == "true" ? true : false;
	}

	return true; // If you return false, no children of this node or its siblings will be visited.
}

bool LevelManager::VisitExit(const tinyxml2::XMLElement& element) {

	if(element.Value() == "Position") {
		m_bIsParsingElementPosition = false;
	}
	else if(element.Value() == "Scale") {
		m_bIsParsingElementScale = false;
	}
	else if(element.Value() == "Origin") {
		m_bIsParsingElementOrigin = false;
	}
	else if(element.Value() == "Item") { // For security
		m_bIsParsingElementPosition = false;
		m_bIsParsingElementScale = false;
		m_bIsParsingElementOrigin = false;

		RenderEntity* rEntity = new RenderEntity(m_currentMetaEntity.m_texture_name, Symp::RenderType::Surface);
		m_pEntityManager->addRenderEntity(rEntity, 0); // TODO : set the layer from XML
		if(m_currentMetaEntity.m_isPhysic) {
			PhysicalEntity* pEntity = new PhysicalEntity(m_pEntityManager->getPhysicalManager()->getWorld(), b2Vec2(m_currentMetaEntity.m_posX, m_currentMetaEntity.m_posY));
			m_pEntityManager->addPhysicalEntity(pEntity);
		}

	}

	return true; // If you return false, no children of this node or its siblings will be visited.
}



} // End of namespace symptogen