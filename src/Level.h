#ifndef _H_SYMPTOGEN_LEVEL_H_
#define _H_SYMPTOGEN_LEVEL_H_

namespace symptogen {

struct Level {

private:
	unsigned int m_levelNum;
	const char* m_xmlMapFileName;

public:
	Level(unsigned int levelNum, const char* xmlMapFileName) :
		m_levelNum(levelNum), m_xmlMapFileName(xmlMapFileName){}
	inline unsigned int getLevelNum() const {
		return m_levelNum;
	}
	const char* getXmlMapFileName() const {
		return m_xmlMapFileName;
	}
	void loadLevel();
};

}

#endif