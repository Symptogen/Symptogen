#include "SoundManager.h"

#include <iterator>
#include <string.h>

namespace Symp{


/* *************************************************************************************** */
/* METHODS' IMPLEMENTATIONS */
/* *************************************************************************************** */

SoundManager::SoundManager() {
    
    // Create system
    FMOD::Debug_SetLevel(FMOD_DEBUG_LEVEL_NONE);

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

    m_result = m_pSystem->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);
    errCheck();

    // Initialize system
    m_result = m_pSystem->init(32, FMOD_INIT_STREAM_FROM_UPDATE, 0);
    errCheck();
    
}

SoundManager::~SoundManager(void) {

    for (std::map<SoundType, std::pair<FMOD::Sound*, FMOD::Channel*>>::iterator it = m_soundsMap.begin(); it != m_soundsMap.end(); ++it)
    {
        std::pair<FMOD::Sound*, FMOD::Channel*> p = it->second;
        p.first->release();
    }

    // Close and release system
    m_result = m_pSystem->close();
    errCheck();
    m_result = m_pSystem->release();
    errCheck();
}

void SoundManager::loadSound(SoundType type) {  

    m_soundsMap.insert(std::make_pair(type, std::make_pair(nullptr, nullptr)));

    FMOD::Sound** s = &m_soundsMap[type].first;
    std::string filename;

    switch(type) {
        case JUMP:
            filename = "../assets/audio/jump.ogg";
            break;
        case SNEEZE:
            filename = "../assets/audio/sneeze.ogg";
            break;
        case FLAMES:
            filename = "../assets/audio/flames.ogg";
            break;
        case HEADACHE:
            filename = "../assets/audio/headache.ogg";
            break;
        case DEATH:
            filename = "../assets/audio/death.ogg";
            break;
        case COLDFEVER_DEATH:
            filename = "../assets/audio/deathColdFever.ogg";
            break;
        case HOTFEVER_DEATH:
            filename = "../assets/audio/deathHotFever.ogg";
            break;
        case DESTRUCTIBLE_OBJECT:
            filename = "../assets/audio/destructibleObject.ogg";
            break;
        case MENU_1:
            filename = "../assets/audio/menu-sound-3.ogg";
            break;
        case MENU_2:
            filename = "../assets/audio/menu-sound-13.ogg";
            break;
        case BG_BLOOM:
            filename = "../assets/audio/backgroundMusic/lvl_Sneeze.ogg";
            break;
        case BG_COLDTRAP:
            filename = "../assets/audio/backgroundMusic/lvl_Fever.ogg";
            break;
        case BG_TRAUMATIC:
            filename = "../assets/audio/backgroundMusic/lvl_Headache.ogg";
            break;
        case BG_SYMPTOGEN:
            filename = "../assets/audio/backgroundMusic/Symptogen.ogg";
            break;
    }

    // Create sound from filename
    m_result = m_pSystem->createSound(filename.c_str(), FMOD_LOOP_OFF, 0, s);
    errCheck();
}

void SoundManager::playSound(SoundType type) {
    FMOD_MODE mode;
    m_soundsMap[type].second->getMode(&mode);
    m_result = m_pSystem->playSound(FMOD_CHANNEL_REUSE, m_soundsMap[type].first, false, &m_soundsMap[type].second);
    m_soundsMap[type].second->setMode(mode);
    errCheck();
}

void SoundManager::attributeChannel(SoundType type) {
    m_result = m_pSystem->playSound(FMOD_CHANNEL_FREE, m_soundsMap[type].first, true, &m_soundsMap[type].second);
    errCheck();
}

void SoundManager::stopSound(SoundType type) {
    FMOD::Channel* c = m_soundsMap[type].second;
    c->stop();
    errCheck();
}

void SoundManager::setLoop(SoundType type, bool loop) {
    if(loop)
        m_soundsMap[type].second->setMode(FMOD_LOOP_NORMAL);
    else
        m_soundsMap[type].second->setMode(FMOD_LOOP_OFF);

    fprintf(stderr, "channel %d loop = %d\n", m_soundsMap[type].second, loop);
}

void SoundManager::setVolume(SoundType type, float volume) {
    m_soundsMap[type].second->setVolume(volume);
}

void SoundManager::loadAllSounds() {

    m_pSystem->setStreamBufferSize(32768, FMOD_TIMEUNIT_MS);

    loadSound(SoundType::JUMP);
    loadSound(SoundType::SNEEZE);
    loadSound(SoundType::FLAMES);
    loadSound(SoundType::HEADACHE);
    loadSound(SoundType::DEATH);
    loadSound(SoundType::COLDFEVER_DEATH);
    loadSound(SoundType::HOTFEVER_DEATH);
    loadSound(SoundType::DESTRUCTIBLE_OBJECT);
    loadSound(SoundType::MENU_1);
    loadSound(SoundType::MENU_2);
    loadSound(SoundType::BG_BLOOM);
    loadSound(SoundType::BG_COLDTRAP);
    loadSound(SoundType::BG_TRAUMATIC);
    loadSound(SoundType::BG_SYMPTOGEN);

    attributeChannel(SoundType::JUMP);
    attributeChannel(SoundType::SNEEZE);
    attributeChannel(SoundType::FLAMES);
    attributeChannel(SoundType::HEADACHE);
    attributeChannel(SoundType::DEATH);
    attributeChannel(SoundType::COLDFEVER_DEATH);
    attributeChannel(SoundType::HOTFEVER_DEATH);
    attributeChannel(SoundType::DESTRUCTIBLE_OBJECT);
    attributeChannel(SoundType::MENU_1);
    attributeChannel(SoundType::MENU_2);
    attributeChannel(SoundType::BG_BLOOM);
    attributeChannel(SoundType::BG_COLDTRAP);
    attributeChannel(SoundType::BG_TRAUMATIC);
    attributeChannel(SoundType::BG_SYMPTOGEN);

    fprintf(stderr, "loading sounds ok\n");

}

void SoundManager::update() {
    m_pSystem->update();
}

}