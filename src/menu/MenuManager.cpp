#include "MenuManager.h"
#include "WelcomeUnknownMenu.h"

#include <Indie.h>

namespace Symp {

MenuManager::MenuManager(Render* pRender){
	
	m_pEntity2dManager = new IND_Entity2dManager();
	m_pEntity2dManager->init(pRender->getIND_Render());

	//Initialize the first menu
	WelcomeUnknownMenu* welcomeMenu = new WelcomeUnknownMenu();
	welcomeMenu->setMenuManager(this);
	setState(welcomeMenu);
	m_pCurrentState->init();

}

MenuManager::~MenuManager(){
	m_pEntity2dManager->end();
    DISPOSE(m_pEntity2dManager);
	delete m_pCurrentState;
}

bool MenuManager::addGuiComponent(GuiComponent* pGuiComponent, int layer){
	m_guiComponentArray.push_back(pGuiComponent);
	return m_pEntity2dManager->add(layer, pGuiComponent->getIND_Entity2d());
}

void MenuManager::renderEntities(){
	m_pEntity2dManager->renderEntities2d();
}

void MenuManager::handleKeyPressed(std::string key){

}


}