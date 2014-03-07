#include "SoundEntity.h"

#include <iterator>
#include <string.h>

namespace Symp{

SoundEntity::SoundEntity(size_t indexSound) {
	m_indexSound = indexSound;
}

  
SoundEntity::~SoundEntity() {
	// Not implemented yet
}

void SoundEntity::setIndexSound(size_t indexSound){
	m_indexSound = indexSound;
}

}