#include "GameManager.h"

#include <Indie.h>

GameManager::GameManager(const char *title, int width, int height, int bpp, bool vsync, bool fs, bool dBuffer){
	IndieLib::init(IND_DEBUG_MODE);
	m_pWindow = new Window();
	m_pRender = new Render();
	m_pWindow->setWindow(m_pRender->init(title, width, height, bpp, vsync, fs, dBuffer));
	m_pWindow->setCursor(true);
	m_pInputManager = new InputManager(m_pRender);
 	m_pEntityManager = new EntityManager(m_pRender);
 	m_pPhysicalManager = new PhysicalManager(0.f, -10.f);

 	m_bIsInGame = true;
}

GameManager::~GameManager(){
	IndieLib::end();
	delete m_pWindow;
	delete m_pRender;
	delete m_pInputManager;
	delete m_pEntityManager;
}

void GameManager::update(){
	// ----- Main Loop -----
	while (!m_pInputManager->onKeyPress (IND_ESCAPE) && !m_pInputManager->quit())
	{
		// ----- Input Update ----
		m_pInputManager->update();
 
 		if(m_bIsInGame)
			updateGame();
		else
			updateMenu();
	}
}

void GameManager::updateGame(){
	m_pPhysicalManager->updatePhysics();
	m_pEntityManager->updateEntities();

	/***** Déplacements Rabbit1 *****/
	int stepMov = 5;

	// TMP : this is dangerous... (do not add delete of add other entities in the main for now !!!)
	// PhysicalEntity* pRabbit = m_pEntityManager->getPhysicalEntityArray()[1];
	// PhysicalEntity* pRabbit2 = m_pEntityManager->getPhysicalEntityArray()[2];
	RenderEntity* rRabbit = m_pEntityManager->getRenderEntityArray()[1];
	RenderEntity* rRabbit2 = m_pEntityManager->getRenderEntityArray()[2];

	if (m_pInputManager->isKeyPressed(IND_KEYRIGHT)){
		if (!m_pEntityManager->getIND_Entity2dManager()->isCollision(rRabbit->getIND_Entity2d(), "rabbit_right", rRabbit2->getIND_Entity2d(), "*")){
			rRabbit->setPosition(rRabbit->getPosX() + stepMov, rRabbit->getPosY(), 0);
		}
	}
	if (m_pInputManager->isKeyPressed(IND_KEYLEFT)){
		if (!m_pEntityManager->getIND_Entity2dManager()->isCollision(rRabbit->getIND_Entity2d(), "rabbit_left", rRabbit2->getIND_Entity2d(), "*")){
			rRabbit->setPosition(rRabbit->getPosX() - stepMov, rRabbit->getPosY(), 0);
		}
	}
	if (m_pInputManager->isKeyPressed(IND_KEYUP)){
		if (!m_pEntityManager->getIND_Entity2dManager()->isCollision(rRabbit->getIND_Entity2d(), "rabbit_up", rRabbit2->getIND_Entity2d(), "*")){
			rRabbit->setPosition(rRabbit->getPosX(), rRabbit->getPosY() - stepMov, 0);
		}
	}
	if (m_pInputManager->isKeyPressed(IND_KEYDOWN)){
		if (!m_pEntityManager->getIND_Entity2dManager()->isCollision(rRabbit->getIND_Entity2d(), "rabbit_down", rRabbit2->getIND_Entity2d(), "*")){
			rRabbit->setPosition(rRabbit->getPosX(), rRabbit->getPosY() + stepMov, 0);
		}
	}

	/***** Déplacements Rabbit2 *****/
	if (m_pInputManager->isKeyPressed(IND_D)){
		if (!m_pEntityManager->getIND_Entity2dManager()->isCollision(rRabbit2->getIND_Entity2d(), "rabbit_right", rRabbit->getIND_Entity2d(), "*")){
			rRabbit2->setPosition(rRabbit2->getPosX() + stepMov, rRabbit2->getPosY(), 0);
		}
	}
	if (m_pInputManager->isKeyPressed(IND_Q)){
		if (!m_pEntityManager->getIND_Entity2dManager()->isCollision(rRabbit2->getIND_Entity2d(), "rabbit_left", rRabbit->getIND_Entity2d(), "*")){
			rRabbit2->setPosition(rRabbit2->getPosX() - stepMov, rRabbit2->getPosY(), 0);
		}
	}
	if (m_pInputManager->isKeyPressed(IND_Z)){
		if (!m_pEntityManager->getIND_Entity2dManager()->isCollision(rRabbit2->getIND_Entity2d(), "rabbit_up", rRabbit->getIND_Entity2d(), "*")){
			rRabbit2->setPosition(rRabbit2->getPosX(), rRabbit2->getPosY() - stepMov, 0);
		}
	}
	if (m_pInputManager->isKeyPressed(IND_S)){
		if (!m_pEntityManager->getIND_Entity2dManager()->isCollision(rRabbit2->getIND_Entity2d(), "rabbit_down", rRabbit->getIND_Entity2d(), "*")){
			rRabbit2->setPosition(rRabbit2->getPosX(), rRabbit2->getPosY() + stepMov, 0);
		}
	}

	// NEED THE FMOD FACADE
	//Lecture du son test
	// if (m_pInputManager->isKeyPressed(IND_SPACE)){
	// 	FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, test, 0, NULL);
	// }

	m_pRender->clearViewPort(60, 60, 60);
	m_pRender->beginScene();
	m_pEntityManager->renderEntities();
	//_entity2dManager->renderCollisionAreas(255, 0, 0, 255); // To see the collisions areas
	m_pRender->endScene();
}

void GameManager::updateMenu(){

}
