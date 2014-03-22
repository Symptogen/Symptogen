#ifndef _H_SYMPTOGEN_SOUND_SOUND_MANAGER_H_
#define _H_SYMPTOGEN_SOUND_SOUND_MANAGER_H_

#include "fmod.hpp"
#include "fmod_errors.h"

#include <iostream>
#include <cstdlib>
#include <vector>

#include <time.h>
#include <sys/types.h>


#ifdef _WIN32
	#include <io.h>
#elif __linux__
	#include <unistd.h>
#endif
//#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include "../util/Singleton.h"
#include "SoundEntity.h"

/* *************************************************************************************** */
/* COMMON */
/* *************************************************************************************** */

namespace Symp {

void ERRCHECK(FMOD_RESULT result);

/* *************************************************************************************** */
/* CLASS DEFINITION */
/* *************************************************************************************** */
/**
*   Facade of FMOD.
*   SoundManager class is a facade of the library FMOD Ex. 
*   It manages all sound effects of the game.
*/
class SoundManager : public Singleton<SoundManager>{

    // Friend to use private constructor/destructor
    friend class Singleton<SoundManager>;

public:

    /**
    *   Load a sound from a filename
    */
    FMOD::Sound* loadSound(const char* filename);

    /**
    *   Load a sound from a directory
    */
    void loadFromFolder(const char* directory);

    /**
    *   Play the indicated sound
    */
    void playSound(SoundEntity* sound);

    /**
    *   Pause the indicated sound
    */
    void stopSound(SoundEntity* sound);

    /*
    * Delete the indicated sound 
    */
    void deleteSound(SoundEntity* sound);

    /*
    * Clean the sound array
    */
    void clearSoundArray();

    /**
    *    Update state of the FMOD system 
    */
    void updateState();

    /** 
    * Setters
    */
    void loop(SoundEntity* sound);
    void removeLoop(SoundEntity* sound);
    void toggleLoop(SoundEntity* sound);

    /**
    * Getters
    */
    size_t getSoundsCount(){return m_soundArray.size();}

    inline void errCheck(){ERRCHECK(m_result);};

private:

    FMOD_RESULT         m_result;
    unsigned int        m_uiMs;
    unsigned int        m_uiLenms;
    bool                m_bIsPlaying;
    bool                m_bIsPaused;
    int                 m_iChannelsplaying;

    /** 
    * FMOD object which contains all the information needed to play a sound.
    */
    FMOD::System*               m_pSystem;

    /**
    * Array of FMOD sound object which represents a sound element.
    */
    std::vector<FMOD::Sound*>   m_soundArray;

    unsigned int                m_uiVersion;

    /**
    * FMOD channel object. One sound can be played in one channel. 
    * It provides the possibility to play several sounds at the same time, on several channels.
    */
    std::vector<FMOD::Channel*>       m_ChannelArray;

    /** 
    * Private constructor (because it is a singleton)
    * Check FMOD version and init it
    */
    SoundManager();

    /** 
    * Private destructor (because it is a singleton)
    */
    ~SoundManager();
};

}


#endif // _H_SYMPTOGEN_SOUND_SOUND_MANAGER_H_