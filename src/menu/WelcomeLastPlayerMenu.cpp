#include "WelcomeLastPlayerMenu.h"
#include <sstream>

#include "ManageGamesMenu.h"

#include "../GameManager.h"

/** @namespace Symp */
namespace Symp {

extern int g_WindowHeight;
extern int g_WindowWidth;

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

	m_background = new Image("../assets/menu/main-menu-background.png");
	m_background->setWidth(g_WindowWidth);
	m_background->setHeight(g_WindowHeight);
	m_background->setAspectRatio(AspectRatio::IGNORE_ASPECT_RATIO);
	MenuManager::getInstance()->addGuiComponent(m_background, 0);
	m_background->update();

	//Create the layout
	int layoutWidth = g_WindowWidth*0.25;
	int layoutHeight = g_WindowHeight*0.5;
	m_buttonLayout = new Layout(g_WindowWidth/2 - layoutWidth/2, g_WindowHeight/2 - layoutHeight/2, layoutWidth, layoutHeight);

	//Create a new game button
	m_resumeGameButton = new Image("../assets/menu/resume-game.png");
	m_resumeGameButton->setColor(Color::BLUEDINO);
	m_resumeGameButton->enable();
	m_resumeGameButton->setAspectRatio(AspectRatio::IGNORE_ASPECT_RATIO);
	m_buttonLayout->addComponent(m_resumeGameButton, 0, 0);

	//Last Player text
	int textPosX = (m_resumeGameButton->getPosX() + m_resumeGameButton->getWidth()/2);
	int textPosY = (m_resumeGameButton->getPosY() + m_resumeGameButton->getHeight()/3);
	m_lastPlayerName = new Text(m_pLastPlayer->getName() + "'s", Color::GREY, textPosX, textPosY, true);
	MenuManager::getInstance()->addGuiComponent(m_lastPlayerName, 1);

	//Manage games button (disabled)
	m_manageGamesButton = new Image("../assets/menu/manage-games.png");
	m_manageGamesButton->setColor(Color::BLUEDINO);
	m_manageGamesButton->enable();
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