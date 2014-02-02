#include "WelcomeLastPlayerMenu.h"
#include <sstream>

#include "ManageGamesMenu.h"

namespace Symp {

WelcomeLastPlayerMenu::WelcomeLastPlayerMenu(Player* lastPlayer, MenuManager* pMenuManager) 
	: State(pMenuManager), m_pLastPlayer(lastPlayer)
{

}

WelcomeLastPlayerMenu::~WelcomeLastPlayerMenu()
{
}

void WelcomeLastPlayerMenu::init(){

	//Title
	m_titleImage = new Image("../assets/title.png", 200, 50);
	m_pMenuManager->addGuiComponent(m_titleImage, 0);

	//Create the layout
	m_buttonLayout = new Layout(200, 150, 400, 350);

	//First button
	m_resumeGameButton = new Button("Resume " + m_pLastPlayer->getName() + "\'s Game", Symp::Color::GREEN);
	m_buttonLayout->addComponent(m_resumeGameButton, 0, 0);

	//Second button
	m_manageGamesButton = new Button("Manage Games", Symp::Color::GREY);
	m_buttonLayout->addComponent(m_manageGamesButton, 0, 1);

	m_buttonLayout->insertSpace(0, 2);

	//Third button
	m_quitButton = new Button("Quit Game", Symp::Color::GREY);
	m_buttonLayout->addComponent(m_quitButton, 0, 3);

	//Settle the layout
	m_pMenuManager->addGuiLayout(m_buttonLayout, 0);
}

void WelcomeLastPlayerMenu::handleMouseClic(int mouseX, int mouseY){
	if (m_resumeGameButton->isTargetedByMouse(mouseX, mouseY)){

		//Convert int to string
		std::ostringstream oss;
		oss << m_pLastPlayer->getCurrentLevel();
		std::string levelNumber = oss.str();

		m_pMenuManager->setLevelToLoad("../assets/map/map" + levelNumber + ".xml");
		m_pMenuManager->setLevelChoosen(true);
		std::cout << "Level to be loaded : " << m_pMenuManager->getLevelToLoad() << std::endl;
	}
	else if(m_manageGamesButton->isTargetedByMouse(mouseX, mouseY)){
		ManageGamesMenu* manageGamesMenu = new ManageGamesMenu(m_pMenuManager);
		m_pMenuManager->setState(manageGamesMenu);
	}
	else if(m_quitButton->isTargetedByMouse(mouseX, mouseY)){
		//TODO : QUIT
	}
}

void WelcomeLastPlayerMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

void WelcomeLastPlayerMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}

}