#include "NewGameMenu.h"

namespace Symp {

NewGameMenu::NewGameMenu(MenuManager* pMenuManager) 
	: State(pMenuManager)
{

}

NewGameMenu::~NewGameMenu()
{
}

void NewGameMenu::init(){

	m_pBackButton = new Button("../assets/back.png");
	m_pMenuManager->addGuiComponent(m_pBackButton, 0);

	//Title
	m_pTitleImage = new Image("../assets/newGame.png", 200, 50);
	m_pMenuManager->addGuiComponent(m_pTitleImage, 0);

	//Create the layout
	m_pButtonLayout = new Layout(300, 150, 400, 350);

	m_pLaunchButton = new Button("Create and Launch", Symp::Color::GREY);
	m_pButtonLayout->addComponent(m_pLaunchButton, 0, 0);

	//Settle the layout
	m_pMenuManager->addGuiLayout(m_pButtonLayout, 0);
}

void NewGameMenu::handleMouseClic(int mouseX, int mouseY){
	if (m_pLaunchButton->isTargetedByMouse(mouseX, mouseY)){
		std::cout << "launch clicked " << std::endl;
		m_pMenuManager->setLevelToLoad("../assets/map/map1.xml");
		m_pMenuManager->setLevelChoosen(true);
	}
	else if(m_pBackButton->isTargetedByMouse(mouseX, mouseY)){
		m_pMenuManager->goBack();
	}
}

void NewGameMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

void NewGameMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}

}