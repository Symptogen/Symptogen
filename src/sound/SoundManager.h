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


/*************************************************************************************************/
/*************************************************************************************************/
/****************************************** COMMON  **********************************************/
/*************************************************************************************************/
/*************************************************************************************************/

namespace Symp {

/*#define _kbhit kbhit
#define _getch getch
#define stricmp strcasecmp
#define strnicmp strncasecmp

#define Sleep(x) usleep((x)*1000)

static int    inited = 0;
static struct termios ori;

static void tcatexit() {
   tcsetattr(0,TCSANOW,&ori);
}

static void init_terminal() {
   struct termios t;
   tcgetattr(0,&t);
   tcgetattr(0,&ori);
   t.c_lflag &= ~(ICANON);
   t.c_lflag &= ~(ECHO);
   tcsetattr(0,TCSANOW,&t);
   atexit(tcatexit);
}

static inline int kbhit(){
  fd_set rfds;
  struct timeval tv;

   if (!inited){
      inited=1;
      init_terminal();
   }

   FD_ZERO(&rfds);
   FD_SET(0, &rfds);
   tv.tv_sec = 0;
   tv.tv_usec = 10*1000;
   return select(1, &rfds, NULL, NULL, &tv)>0;
}

static inline int getch(){
   fd_set rfds;

   if (!inited){
      inited=1;
      init_terminal();
   }

   FD_ZERO(&rfds);
   FD_SET(0, &rfds);
   if (select(1, &rfds, NULL, NULL, NULL)>0)
     return getchar();
   else{
      printf("wincompat.h: select() on fd 0 failed\n");
      return 0xDeadBeef;
   }
}*/

void ERRCHECK(FMOD_RESULT result);

/*************************************************************************************************/
/*************************************************************************************************/
/************************************   CLASS DEFINITION    **************************************/
/*************************************************************************************************/
/*************************************************************************************************/

/**
    Facade of FMOD.
*/
class SoundManager {
public:
    FMOD_RESULT         m_result;
    static unsigned int s_uiMs;
    static unsigned int s_uiLenms;
    static bool         s_bIsPlaying;
    static bool         s_bIsPaused;
    static int          s_iChannelsplaying;

    //check FMOD version and init it
    SoundManager();
    ~SoundManager();

    //load every sound before playing them
    size_t loadSound(const char* filename);
    void loadFromFolder(const char* directory);
    void play(size_t index);

    void updateState();

    //setters
    void loop(size_t index);
    void removeLoop(size_t index);
    void toggleLoop(size_t index);

    //getters
    size_t getSoundsCount();

    inline void errCheck(){ERRCHECK(m_result);};

private:
    FMOD::System*               m_pSystem;
    std::vector<FMOD::Sound *>  m_soundArray;
    unsigned int                m_uiVersion;
    static FMOD::Channel*       s_pChannel;
};

}

#endif // _H_SYMPTOGEN_SOUND_SOUND_MANAGER_H_