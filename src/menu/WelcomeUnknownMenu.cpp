#include "WelcomeUnknownMenu.h"

namespace Symp {

WelcomeUnknownMenu::WelcomeUnknownMenu(MenuManager* pMenuManager) 
	: State(pMenuManager)
{
	
	//Create the layout
	m_buttonLayout = new Layout(200, 100, 400, 200);
	m_guiComponentArray.push_back(m_buttonLayout);

	//First button
	m_createGameButton = new Button("Create A New Game", Symp::Color::RED);
	m_buttonLayout->addComponent(m_createGameButton, 0, 0);
	m_guiComponentArray.push_back(m_createGameButton);

	//Second button
	m_manageGamesButton = new Button("Manage Games", Symp::Color::GREY);
	m_manageGamesButton->disable();
	m_buttonLayout->addComponent(m_manageGamesButton, 0, 1);
	m_guiComponentArray.push_back(m_manageGamesButton);

	//Third button
	m_quitButton = new Button("Quit Game", Symp::Color::GREY);
	m_buttonLayout->addComponent(m_quitButton, 0, 2);
	m_guiComponentArray.push_back(m_quitButton);

	//Settle the layout
	m_pMenuManager->addGuiLayout(m_buttonLayout, 0);
}

WelcomeUnknownMenu::~WelcomeUnknownMenu()
{
}

void WelcomeUnknownMenu::handleMouseClic(int mouseX, int mouseY){
	for (unsigned int i = 0; i < m_guiComponentArray.size(); ++i ){
		if (m_guiComponentArray[i]->isTargetedByMouse(mouseX, mouseY)){
			
			if (m_guiComponentArray[i] == m_createGameButton){
				m_pMenuManager->setLevelChoosen();
			}
		}
	}
}

void WelcomeUnknownMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

void WelcomeUnknownMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}

}