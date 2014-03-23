#include "SoundManager.h"

#include <iterator>
#include <string.h>

namespace Symp{


/* *************************************************************************************** */
/* METHODS' IMPLEMENTATIONS */
/* *************************************************************************************** */

SoundManager::SoundManager() {
	
    // Create system
    FMOD::Debug_SetLevel(FMOD_DEBUG_LEVEL_NONE);

	m_result = FMOD::System_Create(&m_pSystem);
	errCheck();

	// Get system's version
    m_result = m_pSystem->getVersion(&m_uiVersion);
    errCheck();

    if (m_uiVersion < FMOD_VERSION) {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", m_uiVersion, FMOD_VERSION);
        //getch();
        exit(-1);
    }

    m_result = m_pSystem->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);
    errCheck();

    // Initialize system
    m_result = m_pSystem->init(32, FMOD_INIT_STREAM_FROM_UPDATE, 0);
    errCheck();

    m_uiMs = 0;
    m_uiLenms = 0;
    m_bIsPlaying = 0;
    m_bIsPaused = 0;
    
}

SoundManager::~SoundManager(void) {
	// Release all sounds
	for(unsigned int i=0; i < m_soundArray.size(); ++i){
		m_result = m_soundArray[i]->release();
		errCheck();
	}

	// Close and release system
	m_result = m_pSystem->close();
	errCheck();
	m_result = m_pSystem->release();
	errCheck();
}

void SoundManager::loadSound(const char * filename, FMOD::Sound** sound) {	

	// Create sound from filename
    m_result = m_pSystem->createSound(filename, FMOD_LOOP_OFF, 0, sound);
    errCheck();
    m_soundArray.push_back(*sound);
}

void SoundManager::playSound(SoundEntity* sound) {
    FMOD::Channel* channel;
    m_result = m_pSystem->playSound(FMOD_CHANNEL_FREE, sound->getSound(), false, &channel);
    sound->setChannel(channel);
    errCheck();
}

void SoundManager::stopSound(SoundEntity* sound) {
	if(sound->getChannel() != nullptr) {
		m_result = sound->getChannel()->stop();
        sound->setChannel(nullptr);
	}
    errCheck();
}

void SoundManager::deleteSound(SoundEntity* sound) {
	removeLoop(sound);
    m_result = sound->getSound()->release();
    errCheck();
}

void SoundManager::clearSoundArray() {
	m_soundArray.clear();
}

void SoundManager::loop(SoundEntity* sound){
    m_result = sound->getSound()->setMode(FMOD_LOOP_NORMAL);
    errCheck();
}

void SoundManager::removeLoop(SoundEntity* sound){
    m_result = sound->getSound()->setMode(FMOD_LOOP_OFF);
    errCheck();
}

void SoundManager::toggleLoop(SoundEntity* sound){
    FMOD_MODE * mode = NULL;
    sound->getSound()->getMode(mode);
    
    if(*mode == FMOD_LOOP_OFF) {
    	loop(sound);
    }
    else {
    	removeLoop(sound);
    }
}

}