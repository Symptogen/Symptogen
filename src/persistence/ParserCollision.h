#ifndef _H_SYMPTOGEN_PARSERCOLLISION_H
#define _H_SYMPTOGEN_PARSERCOLLISION_H


#ifdef _WIN32
#include "../libs/tinyxml/tinyxml.h"
#elif __linux__
#include "tinyxml.h"
#endif
#include <Box2D/Box2D.h>
#include <vector>


namespace Symp {

/***********************************************************************************************************************************/
/*                                                      PARSE COLLISION                                                            */
/***********************************************************************************************************************************/

/**
* @class ParserCollision class
* The #ParserCollision class is responsible to read and load a level from a XML file. It inherit from TiXmlVisitor class.
*/
struct ParserCollision : public TiXmlVisitor {

	/**
	* @brief #ParserCollision constructor.
	*/
	ParserCollision();

	/**
	* @brief Load a level from an XML file.
	* @param mapFileName : the name of the file to load
	*/
	std::vector<b2Vec2> loadCollision(const char* collisionFileName, const b2Vec2 hitBoxDimensions);

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
	* The aim of the method is to create the different b2Vec2.
	*/
	bool VisitExit(const TiXmlElement& element);

	/**
	* Clear the vector of vertices, and other data related to the previous shape.
	*/
	void reset(const b2Vec2 hitboxDimension);

private:
	/**
	*	List of verticies of the shape.
	*/
	std::vector<b2Vec2> m_vertexArray;

	/**
	* The following fields are used to identity the current parsed property.
	* Example 1 : when we encounters the element X or Y, we can know if it's a coord of a vertex or of the origin.
	*/
	bool m_bIsParsingElementVertex;
	bool m_bIsParsingElementOrigin;

	/**
	* Tmp value for each vertex.
	*/
	float m_tmpX;
	float m_tmpY;

	/**
	* Tmp value for each shape.
	*/
	float 	m_tmpOriginX;
	float	m_tmpOriginY;
	b2Vec2 	m_tmpHitBoxDimensions;
};

}

#endif