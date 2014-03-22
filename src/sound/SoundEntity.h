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

#include "fmod.hpp"


#ifndef _H_SYMPTOGEN_SOUND_SOUND_ENTITY_H_
#define _H_SYMPTOGEN_SOUND_SOUND_ENTITY_H_


namespace Symp {


/* *************************************************************************************** */
/* CLASS DEFINITION */
/* *************************************************************************************** */
/**
*   A sound entity is the representation of any sound element during the game. 
*   It represents a background music, or a sound effect linked to a render element, or anything which makes a noise.
*/
class SoundEntity {
public:
    /**
    *   Constructor
    *   @see ~SoundEntity()
    *   @param fileName : name of the sound's file.
    */
    SoundEntity(const char* fileName);

    /**
    *   Destructor
    */
    ~SoundEntity();

    /**
    *   Getters
    */
    inline FMOD::Sound*     getSound() const { return m_sound;}
    inline FMOD::Channel*   getChannel() const { return m_pChannel;}
    inline void             setChannel(FMOD::Channel* pChannel) { m_pChannel = pChannel;}

private:
    /**
    *   Index of the corresponding music, stored in the list of sounds in the SoundManager.
    */
    FMOD::Sound* m_sound;
    FMOD::Channel* m_pChannel;

};

}


#endif // _H_SYMPTOGEN_SOUND_SOUND_ENTITY_H_