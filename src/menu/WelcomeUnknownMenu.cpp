#include "WelcomeUnknownMenu.h"

#include "NewGameMenu.h"

namespace Symp {

/**
* @brief WelcomeUnknownMenu constructor
* Responsible for the initialization of the private attributes of the #WelcomeUnknownMenu class. This function
* is not responsible for drawing the graphical elements that compose the menu, the #init() function is.
* @param pMenuManager the reference to the #MenuManager
* @see Player
* @see MenuManager
* @see State
* @see init()
* @see ~WelcomeUnknownMenu())
*/
WelcomeUnknownMenu::WelcomeUnknownMenu(MenuManager* pMenuManager)
	:State(pMenuManager) {

}

/**
* @brief WelcomeUnknownMenu elements initialization
* The elements that compose the menu are created in this function.
* @see Player
* @see MenuManager
* @see State
* @see end()
* @see WelcomeUnknownMenu()
*/
void WelcomeUnknownMenu::init(){
	//Title
	m_titleImage = new Image("../assets/menu/title.png", 200, 50);
	m_pMenuManager->addGuiComponent(m_titleImage, 0);

	//Create the layout
	m_buttonLayout = new Layout(200, 150, 400, 350);

	//Create a new game button
	m_createGameButton = new Button("Create A New Game", Symp::Color::RED);
	m_buttonLayout->addComponent(m_createGameButton, 0, 0);

	//Manage games button (disabled)
	m_manageGamesButton = new Button("Manage Games", Symp::Color::GREY);
	m_manageGamesButton->disable();
	m_buttonLayout->addComponent(m_manageGamesButton, 0, 1);

	m_buttonLayout->insertSpace(0, 2);

	//Quit button
	m_quitButton = new Button("Quit Game", Symp::Color::GREY);
	m_buttonLayout->addComponent(m_quitButton, 0, 3);

	//Settle the layout
	m_pMenuManager->addGuiLayout(m_buttonLayout, 0);
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
void WelcomeUnknownMenu::handleMouseClic(int mouseX, int mouseY){
	if (m_createGameButton->isTargetedByMouse(mouseX, mouseY)){
		//Create new game
		NewGameMenu* newGameMenu = new NewGameMenu(m_pMenuManager);
		m_pMenuManager->setState(newGameMenu);
	}
	else if(m_quitButton->isTargetedByMouse(mouseX, mouseY)){
		//Quit the application
		m_pMenuManager->setIsAboutToQuit(true);
	}
}

/**
* @brief Handle key down event
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void WelcomeUnknownMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

/**
* @brief Handle key up event
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void WelcomeUnknownMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}

}