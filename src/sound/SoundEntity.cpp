#include "SoundEntity.h"
#include "SoundManager.h"

#include <iterator>
#include <string.h>

namespace Symp{

SoundEntity::SoundEntity(const char* fileName) {
	//fprintf(stderr, "Sound Entity Constructor\n");
	m_sound = SoundManager::getInstance()->loadSound(fileName);
	m_pChannel = nullptr;
}

  
SoundEntity::~SoundEntity() {
	SoundManager::getInstance()->deleteSound(this);
	//delete m_pChannel;

}

}