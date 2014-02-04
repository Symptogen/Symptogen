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

	//Back button
	m_pBackButton = new Button("../assets/back.png");
	m_pMenuManager->addGuiComponent(m_pBackButton, 0);

	//Title
	m_pTitleImage = new Image("../assets/manage_game.png", 200, 50);
	m_pMenuManager->addGuiComponent(m_pTitleImage, 0);

	//Create the layout
	m_pButtonLayout = new Layout(200, 150, 400, 350);

	//Image Current Game Label
	m_pCurrentGameLabel = new Image("../assets/current_game.png");
	m_pButtonLayout->addComponent(m_pCurrentGameLabel, 0, 0);

	m_pLoadAnotherGameLabel = new Image("../assets/load_another_game.png");
	m_pButtonLayout->addComponent(m_pLoadAnotherGameLabel, 0, 1);

	//Button
	m_pCreateNewGameButton = new Button("Manage Games", Symp::Color::GREY);
	m_pButtonLayout->addComponent(m_pCreateNewGameButton, 0, 2);

	//Settle the layout
	m_pMenuManager->addGuiLayout(m_pButtonLayout, 0);

}

void ManageGamesMenu::handleMouseClic(int mouseX, int mouseY){
	if (m_pCreateNewGameButton->isTargetedByMouse(mouseX, mouseY)){
		std::cout << "create new game clicked" << std::endl;
		NewGameMenu* newGameMenu = new NewGameMenu(m_pMenuManager);
		m_pMenuManager->setState(newGameMenu);
	}
	else if (m_pBackButton->isTargetedByMouse(mouseX, mouseY)){
		m_pMenuManager->goBack();
	}
}

void ManageGamesMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

void ManageGamesMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}

}