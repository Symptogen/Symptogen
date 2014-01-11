#include "EntityManager.h"
#include "LevelManager.h"
#include "tinyxml2.h"

namespace symptogen {
	
using namespace tinyxml2;

void LevelManager::loadLevel(EntityManager& em, const char* mapFileName) {

	

	XMLDocument doc;
	if (doc.LoadFile(mapFileName) != tinyxml2::XML_NO_ERROR) {
		std::cerr << "Error when loading " << mapFileName << ". The program will close." << std::endl;
		exit(EXIT_FAILURE);
	}

	XMLElement* pLevelElement = doc.RootElement();
	m_pCurrentLevelName = pLevelElement->Name();

	parseChildren(pLevelElement, 0);

}

void LevelManager::parseChildren(XMLNode* node, unsigned int depth) {

	XMLNode* child = node->FirstChild();
	while(child != nullptr) {
		for(int t = 0; t < depth+1; ++t) {
			std::cerr << "\t";
		}
		std::cerr << child->Value() << std::endl;
		parseChildren(child, depth+1);
		child = child->NextSibling();
	}

	
}


} // End of namespace symptogen
