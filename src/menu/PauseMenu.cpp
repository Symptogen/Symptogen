#include "PauseMenu.h"

namespace Symp {

PauseMenu::PauseMenu(MenuManager* pMenuManager)
	:State(pMenuManager) {

}

PauseMenu::~PauseMenu(){
	m_pMenuManager->setIsDisplayingPauseState(false);
}

void PauseMenu::init(){
	m_pMenuManager->setIsDisplayingPauseState(true);

	//Create the layout
	Color backgroundColor = Color(150, 150, 150, 50);
	m_buttonLayout = new Layout(250, 100, 350, 350, backgroundColor, LayoutFillAttribute::BACKGROUND);
	m_buttonLayout->setVerticalMargin(10);
	m_buttonLayout->setHorizontalMargin(50);

	m_buttonLayout->insertSpace(0, 0);

	//First button
	m_resumeGameButton = new Button("Resume Game", Symp::Color::RED, 1);
	m_buttonLayout->addComponent(m_resumeGameButton, 0, 1);

	//Second button
	m_optionsButton = new Button("Options", Symp::Color::WHITE, 1);
	m_buttonLayout->addComponent(m_optionsButton, 0, 2);

	//Third button
	m_backToMenuButton = new Button("Back to Menus", Symp::Color::WHITE, 1);
	m_buttonLayout->addComponent(m_backToMenuButton, 0, 3);

	m_buttonLayout->insertSpace(0, 4);

	//Settle the layout
	m_pMenuManager->addGuiLayout(m_buttonLayout, 1);
}

void PauseMenu::handleMouseClic(int mouseX, int mouseY){
	if (m_resumeGameButton->isTargetedByMouse(mouseX, mouseY)){
		m_pMenuManager->setLevelChoosen(true);
	}
	else if(m_optionsButton->isTargetedByMouse(mouseX, mouseY)){
		//TODO : options ?
	}
	else if (m_backToMenuButton->isTargetedByMouse(mouseX, mouseY)){
		m_pMenuManager->setIsGoingBackToMenu(true);
		//TODO : a pop up to ask the user if he is absolutely sure to quit
	}
}

void PauseMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

void PauseMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}
}