#include "SoundEntity.h"

#include <iterator>
#include <string.h>

namespace Symp{

SoundEntity::SoundEntity(size_t indexSound) {
	m_indexSound = indexSound;
	m_bHasToBeDestroyed = false;
}

  
SoundEntity::~SoundEntity() {
	// Not implemented yet
}


void SoundEntity::hasToBeDestroyed(bool flag) {
	m_bHasToBeDestroyed = flag;
}

void SoundEntity::setIndexSound(size_t indexSound){
	m_indexSound = indexSound;
}

}