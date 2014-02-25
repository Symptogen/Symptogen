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
    *   @see SoundEntity()
    *   @see ~SoundEntity()
    *   @param indexSound : index of the sound stored in the list of sounds in the SoundManager.
    */
    SoundEntity(size_t indexSound);

    /**
    *   Destructor
    */
    ~SoundEntity();

    /**
    *   Getters
    */
    inline bool hasToBeDestroyed() const { return m_bHasToBeDestroyed;}
    inline size_t getIndexSound() const { return m_indexSound;}

    /**
    *   Setters
    */
    void hasToBeDestroyed(bool flag);
    void setIndexSound(size_t indexSound);

private:
    bool m_bHasToBeDestroyed;

    /**
    *   Index of the corresponding music, stored in the list of sounds in the SoundManager.
    */
    size_t m_indexSound;

};

}


#endif // _H_SYMPTOGEN_SOUND_SOUND_ENTITY_H_