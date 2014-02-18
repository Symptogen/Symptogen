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


#ifndef _H_SYMPTOGEN_SOUND_SOUND_MANAGER_H_
#define _H_SYMPTOGEN_SOUND_SOUND_MANAGER_H_


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
class SoundManager {
public:
    FMOD_RESULT         m_result;
    static unsigned int s_uiMs;
    static unsigned int s_uiLenms;
    static bool         s_bIsPlaying;
    static bool         s_bIsPaused;
    static int          s_iChannelsplaying;

    /** 
    *   Constructor 
    *   Check FMOD version and init it
    */
    SoundManager();

    /** 
    *   Destructor
    */
    ~SoundManager();

    /**
    *   Loads a sound from a filename
    */
    size_t loadSound(const char* filename);

    /**
    *   Loads a sound from a directory
    */
    void loadFromFolder(const char* directory);

    /**
    *   Plays the sound at the indicated index
    */
    void play(size_t index);

    /**
    *    Updates state of the FMOD system 
    */
    void updateState();

    /** 
    * Setters
    */
    void loop(size_t index);
    void removeLoop(size_t index);
    void toggleLoop(size_t index);

    /**
    * Getters
    */
    size_t getSoundsCount();

    inline void errCheck(){ERRCHECK(m_result);};

private:

    /** 
    *   FMOD object which contains all the information needed to play a sound.
    */
    FMOD::System*               m_pSystem;

    /**
    *   Array of FMOD sound object which represents a sound element.
    */
    std::vector<FMOD::Sound*>  m_soundArray;

    unsigned int                m_uiVersion;

    /**
    *   FMOD channel object. One sound can be played in one channel. It provides the possibility to play
several sounds at the same time, on several channels.

    */
    static FMOD::Channel*       s_pChannel;
};
}


#endif // _H_SYMPTOGEN_SOUND_SOUND_MANAGER_H_