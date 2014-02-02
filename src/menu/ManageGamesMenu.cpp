#include "ManageGamesMenu.h"

#include "NewGameMenu.h"

namespace Symp {
	
ManageGamesMenu::ManageGamesMenu(MenuManager* pMenuManager)
	:State(pMenuManager){
	
}

ManageGamesMenu::~ManageGamesMenu()
{
}

void ManageGamesMenu::init(){

	//Title
	m_pTitleImage = new Image("../assets/title.png", 200, 50);
	m_pMenuManager->addGuiComponent(m_pTitleImage, 0);

	//Create the layout
	m_pButtonLayout = new Layout(200, 150, 400, 350);

	//Button
	m_pCreateNewGameButton = new Button("Manage Games", Symp::Color::GREY);
	m_pButtonLayout->addComponent(m_pCreateNewGameButton, 0, 0);

	//Settle the layout
	m_pMenuManager->addGuiLayout(m_pButtonLayout, 0);

}

void ManageGamesMenu::handleMouseClic(int mouseX, int mouseY){
	if (m_pCreateNewGameButton->isTargetedByMouse(mouseX, mouseY)){
		std::cout << "create new game clicked" << std::endl;
		NewGameMenu* newGameMenu = new NewGameMenu(m_pMenuManager);
		m_pMenuManager->setState(newGameMenu);
	}
}

void ManageGamesMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

void ManageGamesMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}

}