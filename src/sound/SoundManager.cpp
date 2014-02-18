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
/* CLASS VARIABLE DECLARATIONS */
/* *************************************************************************************** */


FMOD::Channel* 	SoundManager::s_pChannel 			= 0;
unsigned int 	SoundManager::s_uiMs 				= 0;
unsigned int 	SoundManager::s_uiLenms 			= 0;
bool 			SoundManager::s_bIsPlaying 			= 0;
bool 			SoundManager::s_bIsPaused 			= 0;
int 			SoundManager::s_iChannelsplaying 	= 0;


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

    if (m_uiVersion < FMOD_VERSION){
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", m_uiVersion, FMOD_VERSION);
        //getch();
        exit(-1);
    }

    m_result = m_pSystem->setOutput(FMOD_OUTPUTTYPE_ALSA);
    errCheck();

    // Initialize system
    m_result = m_pSystem->init(32, FMOD_INIT_NORMAL, 0);
    errCheck();
}

SoundManager::~SoundManager(void){
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

size_t SoundManager::loadSound(const char * filename){
	// Allocate memory
	size_t index = m_soundArray.size();
	FMOD::Sound * sound;
	m_soundArray.push_back(sound);

	// Create sound from filename
    m_result = m_pSystem->createSound(filename, FMOD_SOFTWARE, 0, &m_soundArray[index]);
    errCheck();
    m_result = m_soundArray[index]->setMode(FMOD_LOOP_OFF);
    errCheck();

    return index;
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

void SoundManager::play(size_t index){
    m_result = m_pSystem->playSound(FMOD_CHANNEL_FREE, m_soundArray[index], 0, &s_pChannel);
    errCheck();
}

void SoundManager::updateState(void){
	m_pSystem->update();

	if (s_pChannel){
		FMOD::Sound *currentsound = 0;

		m_result = s_pChannel->isPlaying(&s_bIsPlaying);
		if ((m_result != FMOD_OK) && (m_result != FMOD_ERR_INVALID_HANDLE) && (m_result != FMOD_ERR_CHANNEL_STOLEN)){
			errCheck();
		}

		m_result = s_pChannel->getPaused(&s_bIsPaused);
		if ((m_result != FMOD_OK) && (m_result != FMOD_ERR_INVALID_HANDLE) && (m_result != FMOD_ERR_CHANNEL_STOLEN)){
			errCheck();
		}

		m_result = s_pChannel->getPosition(&s_uiMs, FMOD_TIMEUNIT_MS);
		if ((m_result != FMOD_OK) && (m_result != FMOD_ERR_INVALID_HANDLE) && (m_result != FMOD_ERR_CHANNEL_STOLEN)){
			errCheck();
		}

		s_pChannel->getCurrentSound(&currentsound);
		if (currentsound){
			m_result = currentsound->getLength(&s_uiLenms, FMOD_TIMEUNIT_MS);
			if ((m_result != FMOD_OK) && (m_result != FMOD_ERR_INVALID_HANDLE) && (m_result != FMOD_ERR_CHANNEL_STOLEN)){
				errCheck();
			}
		}
	}
	m_pSystem->getChannelsPlaying(&s_iChannelsplaying);
}

void SoundManager::loop(size_t index){
	if(index > m_soundArray.size()) {
		exit(-1);
	}
    m_result = m_soundArray[index]->setMode(FMOD_LOOP_NORMAL);
    errCheck();
}

void SoundManager::removeLoop(size_t index){
	if(index > m_soundArray.size()) {
		exit(-1);
	}
    m_result = m_soundArray[index]->setMode(FMOD_LOOP_OFF);
    errCheck();
}

void SoundManager::toggleLoop(size_t index){
	if(index > m_soundArray.size()) {
		exit(-1);
	}

    FMOD_MODE * mode = NULL;
    m_soundArray[index]->getMode(mode);
    
    if(*mode == FMOD_LOOP_OFF) {
    	loop(index);
    }
    else {
    	removeLoop(index);
    }
}

}