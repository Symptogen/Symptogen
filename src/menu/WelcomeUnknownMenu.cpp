#include "WelcomeUnknownMenu.h"

#include "NewGameMenu.h"

#include "../GameManager.h"

namespace Symp {

extern int g_WindowHeight;
extern int g_WindowWidth;

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
WelcomeUnknownMenu::WelcomeUnknownMenu()
	:State() {

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
	m_background = new Image("../assets/menu/main-menu-background.png");
	m_background->setWidth(g_WindowWidth);
	m_background->setHeight(g_WindowHeight);
	m_background->setAspectRatio(AspectRatio::IGNORE_ASPECT_RATIO);
	MenuManager::getInstance()->addGuiComponent(m_background, 0);

	//Create the layout
	int layoutWidth = g_WindowWidth*0.25;
	int layoutHeight = g_WindowHeight*0.5;
	m_buttonLayout = new Layout(g_WindowWidth/2 - layoutWidth/2, g_WindowHeight/2 - layoutHeight/2, layoutWidth, layoutHeight);

	//Create a new game button
	m_createGameButton = new Image("../assets/menu/create-new-game.png");
	m_createGameButton->setColor(Color::BLUEDINO);
	m_createGameButton->enable();
	m_createGameButton->setAspectRatio(AspectRatio::IGNORE_ASPECT_RATIO);
	m_buttonLayout->addComponent(m_createGameButton, 0, 0);

	//Manage games button (disabled)
	m_manageGamesButton = new Image("../assets/menu/manage-games.png");
	m_manageGamesButton->setColor(Color::BLACK);
	m_manageGamesButton->setAspectRatio(AspectRatio::IGNORE_ASPECT_RATIO);
	m_buttonLayout->addComponent(m_manageGamesButton, 0, 1);

	//Quit button
	m_quitButton = new Image("../assets/menu/quit-symptogen.png");
	m_quitButton->setColor(Color::YELLOWDINO);
	m_quitButton->enable();
	m_quitButton->setAspectRatio(AspectRatio::IGNORE_ASPECT_RATIO);
	m_buttonLayout->addComponent(m_quitButton, 0, 2);

	//Settle the layout
	MenuManager::getInstance()->addGuiLayout(m_buttonLayout, 1);
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
		NewGameMenu* newGameMenu = new NewGameMenu();
		MenuManager::getInstance()->setState(newGameMenu);
	}
	else if(m_quitButton->isTargetedByMouse(mouseX, mouseY)){
		//Quit the application
		MenuManager::getInstance()->setIsAboutToQuit(true);
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