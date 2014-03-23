#include "SoundEntity.h"
#include "SoundManager.h"

#include <iterator>
#include <string.h>

namespace Symp{

SoundEntity::SoundEntity(const char* fileName) {
	SoundManager::getInstance()->loadSound(fileName, &m_sound);
	m_pChannel = nullptr;
}

  
SoundEntity::~SoundEntity() {
	SoundManager::getInstance()->deleteSound(this);
	m_pChannel = nullptr;
}

}