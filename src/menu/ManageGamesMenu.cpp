#include "ManageGamesMenu.h"
#include <sstream>

#include "NewGameMenu.h"
#include "ChooseYourLevelMenu.h"

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
	m_pTitleImage = new Image("../assets/manage_game.png", 220, 10);
	m_pMenuManager->addGuiComponent(m_pTitleImage, 0);


	//Image Current Game Label
	m_pCurrentGameLabel = new Image("../assets/current_game.png", 200, 100);
	m_pMenuManager->addGuiComponent(m_pCurrentGameLabel, 2);

	// Last Player panel
	Color borderColor = Color(180, 100, 100);
	m_pLastPlayerLayout = new Layout(200, 130, 400, 80, borderColor, 1);

		//Button 
		Color backgroundColor = Color(120, 120, 180, 50);
		m_TestButton = new Button(backgroundColor, m_pLastPlayerLayout->getPosX(), m_pLastPlayerLayout->getPosY(),
			m_pLastPlayerLayout->getWidth(), m_pLastPlayerLayout->getHeight());
		m_pMenuManager->addGuiComponent(m_TestButton, 0);

		//Avatar
		std::ostringstream oss;
		oss << m_pMenuManager->getLastPlayer()->getAvatarIndex();
		std::string avatarIndex = oss.str();

		Image* image = new Image(std::string("../assets/dino" + avatarIndex + ".png").c_str());
		m_pLastPlayerLayout->addComponent(image, 0, 0);

		std::string name = m_pMenuManager->getLastPlayer()->getName();
		int level = m_pMenuManager->getLastPlayer()->getCurrentLevel();

		//Slider
		Slider* slider = new Slider(level * gTotalLevelNumber);
		m_pLastPlayerLayout->addComponent(slider, 0, 1);
		m_pMenuManager->addGuiComponent(slider->getImage(), 2);

		m_pMenuManager->addGuiLayout(m_pLastPlayerLayout, 1);


	// Load another label
	m_pLoadAnotherGameLabel = new Image("../assets/load_another_game.png", 200, 250);
	m_pMenuManager->addGuiComponent(m_pLoadAnotherGameLabel, 2);

	//Button
	m_pCreateNewGameButton = new Button(Symp::Color::GREY, 250, 480, 350, 80);
	m_pMenuManager->addGuiComponent(m_pCreateNewGameButton,0);

}

void ManageGamesMenu::handleMouseClic(int mouseX, int mouseY){
	if (m_pCreateNewGameButton->isTargetedByMouse(mouseX, mouseY)){
		std::cout << "create new game clicked" << std::endl;
		NewGameMenu* newGameMenu = new NewGameMenu(m_pMenuManager);
		m_pMenuManager->setState(newGameMenu);
	}
	else if (m_TestButton->isTargetedByMouse(mouseX, mouseY)){
		ChooseYourLevelMenu* chooseYourLevelMenu = new ChooseYourLevelMenu(m_pMenuManager->getLastPlayer(), m_pMenuManager);
		m_pMenuManager->setState(chooseYourLevelMenu);
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