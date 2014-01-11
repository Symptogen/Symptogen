#include "Level.h"
#include "tinyxml2.h"

namespace symptogen {

	void Level::load() {
		
		tinyxml2::XMLDocument doc;
		doc.LoadFile(m_xmlMapFileName);

	}

}