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
    void loadSound(const char* filename, FMOD::Sound** sound);

    /**
    *   Play the indicated sound
    */
    void playSound(SoundEntity* sound);

    /**
    *   Stop the indicated sound
    */
    void stopSound(SoundEntity* sound);

    /**
    *   Set the volume of the indicated sound
    */
    void setVolume(SoundEntity* sound, float volume);

    /*
    * Delete the indicated sound 
    */
    void deleteSound(SoundEntity* sound);

    /*
    * Clean the sound array
    */
    void clearSoundArray();

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

    inline void errCheck() {
         if (m_result != FMOD_OK)
        {
            std::cerr << "FMOD error! (" << m_result << ") " << FMOD_ErrorString(m_result) << std::endl;
            exit(0);
        }
    };

private:

    FMOD_RESULT         m_result;
    unsigned int        m_uiMs;
    unsigned int        m_uiLenms;
    bool                m_bIsPlaying;
    bool                m_bIsPaused;

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