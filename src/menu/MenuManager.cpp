#include "MenuManager.h"
#include "WelcomeUnknownMenu.h"

#include <Indie.h>

namespace Symp {

MenuManager::MenuManager(Render* pRender){
	m_bIsLevelChoosen = false;
	m_pEntity2dManager = new IND_Entity2dManager();
	m_pEntity2dManager->init(pRender->getIND_Render());
	GuiComponent::init(pRender);

	//Temporary ! 
	//Initialize the first menu
	WelcomeUnknownMenu* welcomeMenu = new WelcomeUnknownMenu(this);
	setState(welcomeMenu);

}

MenuManager::~MenuManager(){
	m_pEntity2dManager->end();
    DISPOSE(m_pEntity2dManager);
    GuiComponent::end();
	delete m_pCurrentState;
}

bool MenuManager::addGuiComponent(GuiComponent* pGuiComponent, int layer){
	m_guiComponentArray.push_back(pGuiComponent);
	return m_pEntity2dManager->add(layer, pGuiComponent->getIND_Entity2d());
}

void MenuManager::addGuiLayout(Layout* layout, int layer){
	for (unsigned int i = 0; i < layout->getComponents().size(); ++i){
		addGuiComponent(layout->getComponents()[i], layer);
	}
}

void MenuManager::renderEntities(){
	m_pEntity2dManager->renderEntities2d();
	// for (unsigned int i =0; i < m_guiComponentArray.size(); ++i) {
	// 	m_guiComponentArray[i]->update();
	// }
}

void MenuManager::handleMouseHover(int mouseX, int mouseY) {
	//m_pCurrentState->handleMouseHover(mouseX, mouseY);
	for (unsigned int i =0; i < m_guiComponentArray.size(); ++i) {
		m_guiComponentArray[i]->setHovered(false);
		if (m_guiComponentArray[i]->isTargetedByMouse(mouseX, mouseY) && m_guiComponentArray[i]->isEnabled()){
			m_guiComponentArray[i]->setHovered(true);
		}
	}
}

void MenuManager::handleMouseClic(int mouseX, int mouseY){
	std::cout << "menu manager handle !" <<std::endl;
	m_pCurrentState->handleMouseClic(mouseX, mouseY);
}

void MenuManager::handleKeyPressed(std::string key){
	if(key == "KEYDOWN" ){
		m_pCurrentState->keyDownPressed();
	}
	else if (key == "KEYUP"	){
		m_pCurrentState->keyUpPressed();
	}
}


}