#include <iostream>

#include <fmod.h>
#include <fmod_errors.h>

#include <Box2D/Box2D.h>

#include "GameManager.h"

#ifdef _WIN32
int IndieLib()
#elif __linux__
Indielib_Main
#endif
{
	
	// Sound
	/*FMOD_SYSTEM *system;
    FMOD_SOUND *test;

    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);

	const char* soundFile = "../assets/audio/test.wav";
    FMOD_RESULT result = FMOD_System_CreateSound(system, soundFile, FMOD_CREATESAMPLE, 0, &test);
    if (result != FMOD_OK)
    {
        std::cerr << "Main : Impossible to read " << soundFile << ". The program will end" << std::endl;
        exit(EXIT_FAILURE);
    }*/

	// ----- Game intialization -----
	Symp::GameManager* pGameManager = new Symp::GameManager("Symptogen", 800, 600, 32, 0, 0, 1);
	pGameManager->startMainLoop();

	// Release Data
	/*FMOD_Sound_Release(test);
	FMOD_System_Close(system);
	FMOD_System_Release(system);*/

	return 0;
}