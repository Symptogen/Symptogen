#include "PauseMenu.h"

/** @namespace Symp */
namespace Symp {

extern int g_WindowHeight;
extern int g_WindowWidth;

/**
* @brief PauseMenu constructor
* Responsible for the initialization of the private attributes of the #PauseMenuMenu class. 
* @see MenuManager
* @see State
* @see init()
* @see ~PauseMenuMenu()
*/
PauseMenu::PauseMenu(float posX, float posY)
	:State(){
	m_posX = posX;
	m_posY = posY;
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
	//m_buttonLayout = NULL;
	MenuManager::getInstance()->setIsDisplayingPauseState(false);
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
	MenuManager::getInstance()->setIsDisplayingPauseState(true);

	//Create the layout
	Color backgroundColor = Color(150, 150, 150, 50);
	int layoutWidth = g_WindowWidth * 0.35 ;
	int layoutHeight = g_WindowHeight * 0.8;
	m_buttonLayout = new Layout(m_posX- (layoutWidth/2), m_posY- (layoutHeight/2), layoutWidth, layoutHeight, backgroundColor, LayoutFillAttribute::BACKGROUND);
	m_buttonLayout->setVerticalMargin(10);
	m_buttonLayout->setHorizontalMargin(80);

	m_buttonLayout->insertSpace(0, 0);

	//Resume game button
	m_resumeGameButton = new Image("../assets/menu/resume-game-ingame.png");
	m_resumeGameButton->setColor(Color::BLUEDINO);
	m_resumeGameButton->enable();
	m_resumeGameButton->setAspectRatio(AspectRatio::IGNORE_ASPECT_RATIO);
	m_buttonLayout->addComponent(m_resumeGameButton, 0, 1);

	//Second button
	m_optionsButton = new Image("../assets/menu/options.png");
	m_optionsButton->setColor(Color::BLUEDINO);
	m_optionsButton->enable();
	m_optionsButton->setAspectRatio(AspectRatio::IGNORE_ASPECT_RATIO);
	m_buttonLayout->addComponent(m_optionsButton, 0, 2);

	//Go back to main menu button
	m_backToMenuButton = new Image("../assets/menu/back-to-menu.png");
	m_backToMenuButton->setColor(Color::YELLOWDINO);
	m_backToMenuButton->enable();
	m_backToMenuButton->setAspectRatio(AspectRatio::IGNORE_ASPECT_RATIO);
	m_buttonLayout->addComponent(m_backToMenuButton, 0, 3);

	m_buttonLayout->insertSpace(0, 4);

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
void PauseMenu::handleMouseClic(int mouseX, int mouseY){
	if (m_resumeGameButton->isTargetedByMouse(mouseX, mouseY)){
		// Resume game
		MenuManager::getInstance()->setLevelChoosen(true);
	}
	else if(m_optionsButton->isTargetedByMouse(mouseX, mouseY)){
		//TODO : options ?
	}
	else if (m_backToMenuButton->isTargetedByMouse(mouseX, mouseY)){
		// Quit game and display main menu
		MenuManager::getInstance()->setIsGoingBackToMenu(true);
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