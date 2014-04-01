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
#include <map>

#include "../util/Singleton.h"

/* *************************************************************************************** */
/* COMMON */
/* *************************************************************************************** */

namespace Symp {

enum SoundType {
    JUMP,
    SNEEZE,
    FLAMES,
    HEADACHE,
    DEATH,
    COLDFEVER_DEATH,
    HOTFEVER_DEATH,
    DESTRUCTIBLE_OBJECT,
    MENU_1,
    MENU_2,
    BG_BLOOM,
    BG_COLDTRAP,
    BG_TRAUMATIC,
    BG_SYMPTOGEN
};

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

    void loadAllSounds();
    void playSound(SoundType type);
    void stopSound(SoundType type);
    void setLoop(SoundType type, bool loop);
    void setVolume(SoundType type, float volume);
    void update();

private:

    /**
    *   Load a sound from a filename
    */
    void loadSound(SoundType type);

    void attributeChannel(SoundType type);

    inline void errCheck() {
         if (m_result != FMOD_OK)
        {
            std::cerr << "FMOD error! (" << m_result << ") " << FMOD_ErrorString(m_result) << std::endl;
            exit(0);
        }
    };

    FMOD_RESULT         m_result;

    /** 
    * FMOD object which contains all the information needed to play a sound.
    */
    FMOD::System*               m_pSystem;

    unsigned int                m_uiVersion;

    std::map<SoundType, std::pair<FMOD::Sound*, FMOD::Channel*>> m_soundsMap;

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