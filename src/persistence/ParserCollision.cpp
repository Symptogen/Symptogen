#include "ParserCollision.h"

#include <iostream>

/***********************************************************************************************************************************/
/*                                                      PARSE COLLISION                                                            */
/***********************************************************************************************************************************/

namespace Symp {

ParserCollision::ParserCollision() {
	m_vertexArray = std::vector<b2Vec2>();
	m_bIsParsingElementVertex = false;
	m_bIsParsingElementOrigin = false;
	m_tmpX = 0;
	m_tmpY = 0;
}

std::vector<b2Vec2> ParserCollision::loadCollision(const char* collisionFileName, const b2Vec2 hitboxDimension) {
	reset(hitboxDimension);
	
	TiXmlDocument doc;
	bool success = doc.LoadFile(collisionFileName);
	if (!success) {
		std::cerr << "Error when loading " << collisionFileName << ". " << doc.ErrorDesc() << std::endl;
		std::cerr << "The hitbox of the PhysicalEntity will be the default hitbox." << std::endl;
	}

    doc.Accept(this);

    return m_vertexArray;
}

bool ParserCollision::VisitEnter(const TiXmlElement& element, const TiXmlAttribute* attribute ) {

	std::string elementValue = element.Value();

	if(0 == elementValue.compare("origin")) {
		m_bIsParsingElementOrigin = true;
	}
	else if(0 == elementValue.compare("vertices")) {
		m_bIsParsingElementVertex = true;
	}
	//origin
	else if(0 == elementValue.compare("x") && m_bIsParsingElementOrigin) {
		m_tmpOriginX = atof(element.GetText());
	}
	else if(0 == elementValue.compare("y") && m_bIsParsingElementOrigin) {
		m_tmpOriginY = atof(element.GetText());
	}
	//vertex
	else if(0 == elementValue.compare("x") && m_bIsParsingElementVertex) {
		m_tmpX = atof(element.GetText()) * (m_tmpHitBoxDimensions.x);
	}
	else if(0 == elementValue.compare("y") && m_bIsParsingElementVertex) {
		m_tmpY = atof(element.GetText()) * (m_tmpHitBoxDimensions.x);
		
	}

	return true; // If you return false, no children of this node or its siblings will be visited.
}

bool ParserCollision::VisitExit(const TiXmlElement& element) {

	std::string elementValue = element.Value();

	if(0 == elementValue.compare("origin") && m_bIsParsingElementOrigin) {
		m_bIsParsingElementOrigin = false;
	}
	if(0 == elementValue.compare("vertices") && m_bIsParsingElementVertex) {
		m_bIsParsingElementVertex = false;
		m_vertexArray.push_back(b2Vec2(m_tmpX - m_tmpHitBoxDimensions.x/2.f, -(m_tmpY - m_tmpHitBoxDimensions.y/2.f)));
		m_tmpX = 0;
		m_tmpY = 0;
	}

	return true; // If you return false, no children of this node or its siblings will be visited.
}

void ParserCollision::reset(const b2Vec2 hitboxDimension){
	m_vertexArray.clear();

	m_bIsParsingElementOrigin = false;
	m_bIsParsingElementVertex = false;

	m_tmpHitBoxDimensions = hitboxDimension;

	m_tmpX = 0;
	m_tmpY = 0;
}

}