#include "WelcomeLastPlayerMenu.h"
#include <sstream>

#include "ManageGamesMenu.h"

/** @namespace Symp */
namespace Symp {

/**
* @brief WelcomeLastPlayerMenu constructor
* Responsible for the initialization of the private attributes of the #WelcomeLastPlayerMenu class. This function
* is not responsible for drawing the graphical elements that compose the menu, the #init() function is.
* @param pLastPlayer the reference to the last known #Player
* @see Player
* @see MenuManager
* @see State
* @see init()
* @see ~WelcomeLastPlayerMenu()
*/
WelcomeLastPlayerMenu::WelcomeLastPlayerMenu(Player* pLastPlayer) 
	: State(), m_pLastPlayer(pLastPlayer)
{
}

/**
* @brief WelcomeLastPlayerMenu elements initialization
* The elements that compose the menu are created in this function.
* @see Player
* @see MenuManager
* @see State
* @see end()
* @see WelcomeLastPlayerMenu()
*/
void WelcomeLastPlayerMenu::init(){

	//Title
	m_titleImage = new Image("../assets/menu/title.png", 200, 50);
	MenuManager::getInstance()->addGuiComponent(m_titleImage, 0);

	//Main layout
	m_buttonLayout = new Layout(200, 150, 400, 350);

	//Resume last player game button
	m_resumeGameButton = new Button("Resume " + m_pLastPlayer->getName() + "\'s Game", Symp::Color::GREEN);
	m_buttonLayout->addComponent(m_resumeGameButton, 0, 0);
	MenuManager::getInstance()->addGuiComponent(m_resumeGameButton->getTextEntity(), 1);

	//Go to the ManageGamesMenu button
	m_manageGamesButton = new Button("Manage Games", Symp::Color::GREY);
	m_buttonLayout->addComponent(m_manageGamesButton, 0, 1);
	MenuManager::getInstance()->addGuiComponent(m_manageGamesButton->getTextEntity(), 1);

	m_buttonLayout->insertSpace(0, 2);

	//Quit button
	m_quitButton = new Button("Quit Game", Symp::Color::GREY);
	m_buttonLayout->addComponent(m_quitButton, 0, 3);
	MenuManager::getInstance()->addGuiComponent(m_quitButton->getTextEntity(), 1);

	//Settle the layout
	MenuManager::getInstance()->addGuiLayout(m_buttonLayout, 0);
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
void WelcomeLastPlayerMenu::handleMouseClic(int mouseX, int mouseY) {

	if (m_resumeGameButton->isTargetedByMouse(mouseX, mouseY)) {
		// Launch the correct level for the last known player

		//Convert int to string
		std::ostringstream oss;
		oss << m_pLastPlayer->getCurrentLevel();
		std::string levelNumber = oss.str();

		MenuManager::getInstance()->setLevelToLoad("../assets/map/level" + levelNumber + ".xml");
		MenuManager::getInstance()->setLevelChoosen(true);
		
	}
	else if(m_manageGamesButton->isTargetedByMouse(mouseX, mouseY)){
		// Display the ManageGamesMenu
		ManageGamesMenu* manageGamesMenu = new ManageGamesMenu();
		MenuManager::getInstance()->setState(manageGamesMenu);
	}
	else if(m_quitButton->isTargetedByMouse(mouseX, mouseY)){
		// Quit the application
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
void WelcomeLastPlayerMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

/**
* @brief Handle key up event
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void WelcomeLastPlayerMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}

}