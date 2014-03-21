#include "SoundManager.h"

#include <iterator>
#include <string.h>

namespace Symp{

void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        std::cerr << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
        exit(0);
    }
}

/* *************************************************************************************** */
/* METHODS' IMPLEMENTATIONS */
/* *************************************************************************************** */

SoundManager::SoundManager() {
	// Create system
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

    m_result = m_pSystem->setOutput(FMOD_OUTPUTTYPE_ALSA);
    errCheck();

    // Initialize system
    m_result = m_pSystem->init(32, FMOD_INIT_NORMAL, 0);
    errCheck();

    m_pChannel = 0;
    m_uiMs = 0;
    m_uiLenms = 0;
    m_bIsPlaying = 0;
    m_bIsPaused = 0;
    m_iChannelsplaying = 0;
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

FMOD::Sound* SoundManager::loadSound(const char * filename){
	// Allocate memory
	size_t index = m_soundArray.size();
	FMOD::Sound * sound;
	m_soundArray.push_back(sound);

	// Create sound from filename
    m_result = m_pSystem->createSound(filename, FMOD_SOFTWARE, 0, &m_soundArray[index]);
    errCheck();
    m_result = m_soundArray[index]->setMode(FMOD_LOOP_OFF);
    errCheck();

    return m_soundArray[index];
}

void SoundManager::loadFromFolder(const char* directory){
	// Go to directory
	struct dirent *lecture;
	DIR *rep;
	rep = opendir(directory);

	// Load sound
	while ((lecture = readdir(rep))) {
		std::string musicName = lecture->d_name;
		std::string fullName = directory;
		if(musicName.find(".") != 0 && lecture->d_type != DT_DIR){
			fullName.append("/").append(musicName);
			std::cerr << "Loading sound " << lecture->d_name << " ... " << std::endl;
			loadSound(fullName.c_str());
			std::cerr << "DONE !" << std::endl;
		}
	}
	closedir(rep);
}

void SoundManager::playSound(FMOD::Sound* sound){
    m_result = m_pSystem->playSound(FMOD_CHANNEL_FREE, sound, 0, &m_pChannel);
    errCheck();
}

//NOT WORKING !
void SoundManager::stopSound(FMOD::Sound* sound) {
	removeLoop(sound);
	m_pChannel->stop(); //?
    errCheck();
}

void SoundManager::deleteSound(FMOD::Sound* sound) {
	removeLoop(sound);
    m_result = sound->release();

    errCheck();
}

void SoundManager::clearSoundArray() {
	m_soundArray.clear();
}

void SoundManager::updateState(void){
	m_pSystem->update();

	if (m_pChannel){
		FMOD::Sound *currentsound = 0;

		m_result = m_pChannel->isPlaying(&m_bIsPlaying);
		if ((m_result != FMOD_OK) && (m_result != FMOD_ERR_INVALID_HANDLE) && (m_result != FMOD_ERR_CHANNEL_STOLEN)){
			errCheck();
		}

		m_result = m_pChannel->getPaused(&m_bIsPaused);
		if ((m_result != FMOD_OK) && (m_result != FMOD_ERR_INVALID_HANDLE) && (m_result != FMOD_ERR_CHANNEL_STOLEN)){
			errCheck();
		}

		m_result = m_pChannel->getPosition(&m_uiMs, FMOD_TIMEUNIT_MS);
		if ((m_result != FMOD_OK) && (m_result != FMOD_ERR_INVALID_HANDLE) && (m_result != FMOD_ERR_CHANNEL_STOLEN)){
			errCheck();
		}

		m_pChannel->getCurrentSound(&currentsound);
		if (currentsound){
			m_result = currentsound->getLength(&m_uiLenms, FMOD_TIMEUNIT_MS);
			if ((m_result != FMOD_OK) && (m_result != FMOD_ERR_INVALID_HANDLE) && (m_result != FMOD_ERR_CHANNEL_STOLEN)){
				errCheck();
			}
		}
	}
	m_pSystem->getChannelsPlaying(&m_iChannelsplaying);
}

void SoundManager::loop(FMOD::Sound* sound){
    m_result = sound->setMode(FMOD_LOOP_NORMAL);
    errCheck();
}

void SoundManager::removeLoop(FMOD::Sound* sound){
    m_result = sound->setMode(FMOD_LOOP_OFF);
    errCheck();
}

void SoundManager::toggleLoop(FMOD::Sound* sound){
    FMOD_MODE * mode = NULL;
    sound->getMode(mode);
    
    if(*mode == FMOD_LOOP_OFF) {
    	loop(sound);
    }
    else {
    	removeLoop(sound);
    }
}

}