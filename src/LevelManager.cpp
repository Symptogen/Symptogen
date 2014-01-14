#include "LevelManager.h"
#include "EntityManager.h"


namespace Symp {

LevelManager::LevelManager(EntityManager* entityManager) {
	m_pEntityManager = entityManager;
}

void LevelManager::loadLevel(const char* mapFileName) {

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(mapFileName) != tinyxml2::XML_NO_ERROR) {
		std::cerr << "Error when loading " << mapFileName << ". The program will close." << std::endl;
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

		bool isEntityVisible;
		if(element.Attribute("Visible") == "true" || element.Attribute("Visible") == false ) {
			isEntityVisible = element.Attribute("Visible") == "true" ? true : false;
		}
		else {
			std::cerr << "Warning ! Parsing " << m_pCurrentParsedFile << " : The item " << element.Name() << " has no correct \"Visible\" attribute. The default value is true" << std::endl;
			isEntityVisible = true;
		}
		// Set the entity visibility
		// ...
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
		float rotateAngle = strtod (element.GetText(), nullptr);
		// ...
	}
	else if(element.Value() == "X") {
		int x = atoi(element.GetText());
		if(m_bIsParsingElementPosition) {
			// Set the x parameter of the entity position...
		}
		else if(m_bIsParsingElementScale) {
			// Set the x parameter of the entity scale...
		}
		else if(m_bIsParsingElementOrigin) {
			// Set the x parameter of the entity origin...
		}
	}
	else if(element.Value() == "Custom Properties") {
		// ...
	}
	else if(element.Value() == "texture_filename") {
		const char* textureFilename = element.GetText();
		// ...
	}
	else if(element.Value() == "R") {
		int r = atoi(element.GetText());
		// Tint Color...
	}
	else if(element.Value() == "G") {
		int g = atoi(element.GetText());
		// Tint Color...
	}
	else if(element.Value() == "B") {
		int b = atoi(element.GetText());
		// Tint Color...
	}
	else if(element.Value() == "A") {
		int a = atoi(element.GetText());
		// Tint Color...
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
	}

	return true; // If you return false, no children of this node or its siblings will be visited.
}



} // End of namespace symptogen