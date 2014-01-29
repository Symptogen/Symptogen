#include "WelcomeUnknownMenu.h"

namespace Symp {

WelcomeUnknownMenu::WelcomeUnknownMenu(MenuManager* pMenuManager) 
	: State(pMenuManager)
{

	//Create a image
	m_titleImage = new Image("../assets/title.png", 200, 50);
	m_pMenuManager->addGuiComponent(m_titleImage, 0);

	//Create the layout
	m_buttonLayout = new Layout(200, 150, 400, 200);

	//First button
	m_createGameButton = new Button("Create A New Game", Symp::Color::RED);
	m_buttonLayout->addComponent(m_createGameButton, 0, 0);

	//Second button
	m_manageGamesButton = new Button("Manage Games", Symp::Color::GREY);
	m_manageGamesButton->disable();
	m_buttonLayout->addComponent(m_manageGamesButton, 0, 1);

	//Third button
	m_quitButton = new Button("Quit Game", Symp::Color::GREY);
	m_buttonLayout->addComponent(m_quitButton, 0, 2);

	//Settle the layout
	m_pMenuManager->addGuiLayout(m_buttonLayout, 0);
}

WelcomeUnknownMenu::~WelcomeUnknownMenu()
{
}

void WelcomeUnknownMenu::handleMouseClic(int mouseX, int mouseY){
	if (m_createGameButton->isTargetedByMouse(mouseX, mouseY)){
			m_pMenuManager->setLevelChoosen();
	}
	if (m_testButton->isTargetedByMouse(mouseX, mouseY)){
		std::cout << "plop" << std::endl;
	}
}

void WelcomeUnknownMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

void WelcomeUnknownMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}

}