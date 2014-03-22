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

    fprintf(stderr, "ALSA = %d\n", FMOD_OUTPUTTYPE_ALSA);
    fprintf(stderr, "AUTODETECT = %d\n", FMOD_OUTPUTTYPE_AUTODETECT);

    m_result = m_pSystem->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);
    errCheck();

    // Initialize system
    m_result = m_pSystem->init(32, FMOD_INIT_STREAM_FROM_UPDATE, 0);
    errCheck();

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

void SoundManager::playSound(SoundEntity* sound){
	FMOD::Channel* pChannel;
    m_result = m_pSystem->playSound(FMOD_CHANNEL_FREE, sound->getSound(), 0, &pChannel);
    m_ChannelArray.push_back(pChannel);
    sound->setChannel(pChannel);
    errCheck();
}

//NOT WORKING !
void SoundManager::stopSound(SoundEntity* sound) {
	removeLoop(sound);

	if(sound->getChannel() != nullptr) {
		sound->getChannel()->setPaused(true);
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

void SoundManager::updateState(void) {

	m_pSystem->update();

	for(unsigned int i = 0; i < m_ChannelArray.size(); ++i) {
		if (m_ChannelArray[i]){
			FMOD::Sound *currentsound = 0;

			m_result = m_ChannelArray[i]->isPlaying(&m_bIsPlaying);
			if ((m_result != FMOD_OK) && (m_result != FMOD_ERR_INVALID_HANDLE) && (m_result != FMOD_ERR_CHANNEL_STOLEN)){
				errCheck();
			}

			m_result = m_ChannelArray[i]->getPaused(&m_bIsPaused);
			if ((m_result != FMOD_OK) && (m_result != FMOD_ERR_INVALID_HANDLE) && (m_result != FMOD_ERR_CHANNEL_STOLEN)){
				errCheck();
			}

			m_result = m_ChannelArray[i]->getPosition(&m_uiMs, FMOD_TIMEUNIT_MS);
			if ((m_result != FMOD_OK) && (m_result != FMOD_ERR_INVALID_HANDLE) && (m_result != FMOD_ERR_CHANNEL_STOLEN)){
				errCheck();
			}

			m_ChannelArray[i]->getCurrentSound(&currentsound);
			if (currentsound){
				m_result = currentsound->getLength(&m_uiLenms, FMOD_TIMEUNIT_MS);
				if ((m_result != FMOD_OK) && (m_result != FMOD_ERR_INVALID_HANDLE) && (m_result != FMOD_ERR_CHANNEL_STOLEN)){
					errCheck();
				}
			}
		}
		m_pSystem->getChannelsPlaying(&m_iChannelsplaying);
	}
	
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