#include "PauseMenu.h"

/** @namespace Symp */
namespace Symp {

/**
* @brief PauseMenu constructor
* Responsible for the initialization of the private attributes of the #PauseMenuMenu class. 
* @param pMenuManager the reference to the #MenuManager
* @see MenuManager
* @see State
* @see init()
* @see ~PauseMenuMenu()
*/
PauseMenu::PauseMenu(MenuManager* pMenuManager)
	:State(pMenuManager) {

}

/**
* @brief PauseMenu destructor
* Responsible for telling the #MenuManager that the pause is no longer required, so the #GameManager will update 
* the application.
* @see MenuManager
* @see State
* @see init()
*/
PauseMenu::~PauseMenu(){
	m_pMenuManager->setIsDisplayingPauseState(false);
}

/**
* @brief PauseMenu elements initialization
* The elements that compose the menu are created in this function.
* @see Player
* @see MenuManager
* @see State
* @see end()
* @see PauseMenu()
*/
void PauseMenu::init(){
	m_pMenuManager->setIsDisplayingPauseState(true);

	//Create the layout
	Color backgroundColor = Color(150, 150, 150, 50);
	m_buttonLayout = new Layout(250, 100, 350, 350, backgroundColor, LayoutFillAttribute::BACKGROUND);
	m_buttonLayout->setVerticalMargin(10);
	m_buttonLayout->setHorizontalMargin(50);

	m_buttonLayout->insertSpace(0, 0);

	//Resume game button
	m_resumeGameButton = new Button("Resume Game", Symp::Color::RED, 1);
	m_buttonLayout->addComponent(m_resumeGameButton, 0, 1);

	//Second button
	m_optionsButton = new Button("Options", Symp::Color::WHITE, 1);
	m_buttonLayout->addComponent(m_optionsButton, 0, 2);

	//Go back to main menu button
	m_backToMenuButton = new Button("Back to Menus", Symp::Color::WHITE, 1);
	m_buttonLayout->addComponent(m_backToMenuButton, 0, 3);

	m_buttonLayout->insertSpace(0, 4);

	//Settle the layout
	m_pMenuManager->addGuiLayout(m_buttonLayout, 1);
}

/**
* @brief Handle mouse clic events
* @param mouseX the x coordinate of the mouse position
* @param mouseY the y coordinate of the mouse position
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void PauseMenu::handleMouseClic(int mouseX, int mouseY){
	if (m_resumeGameButton->isTargetedByMouse(mouseX, mouseY)){
		// Resume game
		m_pMenuManager->setLevelChoosen(true);
	}
	else if(m_optionsButton->isTargetedByMouse(mouseX, mouseY)){
		//TODO : options ?
	}
	else if (m_backToMenuButton->isTargetedByMouse(mouseX, mouseY)){
		// Quit game and display main menu
		m_pMenuManager->setIsGoingBackToMenu(true);
	}
}

/**
* @brief Handle key down event
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void PauseMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

/**
* @brief Handle key up event
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void PauseMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}
}