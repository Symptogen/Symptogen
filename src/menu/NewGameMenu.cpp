#include "NewGameMenu.h"

/** @namespace Symp */
namespace Symp {

/**
* @brief NewGameMenu constructor
* Responsible for the initialization of the private attributes of the #NewGameMenuMenu class. This function
* is not responsible for drawing the graphical elements that compose the menu, the #init() function is.
* @see Player
* @see MenuManager
* @see State
* @see init()
* @see ~NewGameMenu()
*/
NewGameMenu::NewGameMenu() 
	: State()
{

	// Temporary
	Image* image1 = new Image("../assets/menu/avatar1.png", 150, 150);
	m_avatarVector.push_back(image1);

	Image* image2 = new Image("../assets/menu/avatar2.png", 150, 150);
	m_avatarVector.push_back(image2);

}

/**
* @brief NewGameMenu elements initialization
* The elements that compose the menu are created in this function.
* @see Player
* @see MenuManager
* @see State
* @see end()
* @see NewGameMenuMenu()
*/
void NewGameMenu::init(){

	// The go back button up-left of the window
	m_pBackButton = new Button("../assets/menu/back.png");
	MenuManager::getInstance()->addGuiComponent(m_pBackButton, 0);

	//The title image
	m_pTitleImage = new Image("../assets/menu/new_game.png", 200, 50);
	MenuManager::getInstance()->addGuiComponent(m_pTitleImage, 0);

	// All the Avatars are initialized hidden
	for (unsigned int i = 0; i < m_avatarVector.size(); ++i){
		m_avatarVector[i]->setWidth(100);
		m_avatarVector[i]->setHeight(100);
		m_avatarVector[i]->update();
		m_avatarVector[i]->hide();
		MenuManager::getInstance()->addGuiComponent(m_avatarVector[i], 0);
	}
	// Only show one
	m_pCurrentAvatar = m_avatarVector[1]; // TODO : change this  ! temporary !
	m_pCurrentAvatar->show();

	//Arrows for naviguation between avatars
	m_pArrowLayout = new Layout(160, 250, 80, 50);

		m_pLeftArrow = new Button("../assets/menu/left_arrow.png");
		m_pArrowLayout->addComponent(m_pLeftArrow, 0, 0);

		m_pRightArrow = new Button("../assets/menu/right_arrow.png");
		m_pArrowLayout->addComponent(m_pRightArrow, 1, 0);

	MenuManager::getInstance()->addGuiLayout(m_pArrowLayout, 1);

	// Main layout
	m_pButtonLayout = new Layout(350, 200, 250, 150);
		//Line edit
		m_pLineEdit = new LineEdit(350, 200, 250, 60);
		//m_pButtonLayout->addComponent(m_pLineEdit, 0, 0);
		MenuManager::getInstance()->addGuiComponent(m_pLineEdit, 0);
		MenuManager::getInstance()->addGuiComponent(m_pLineEdit->getCursor(), 2);

		//Launch button
		m_pLaunchButton = new Button("Create and Launch", Symp::Color::GREY);
		m_pButtonLayout->addComponent(m_pLaunchButton, 0, 2);

	//Settle the layout
	MenuManager::getInstance()->addGuiLayout(m_pButtonLayout, 0);
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
void NewGameMenu::handleMouseClic(int mouseX, int mouseY){
	if (m_pLaunchButton->isTargetedByMouse(mouseX, mouseY)){
		// Launch the game and save the new player
		MenuManager::getInstance()->setLevelToLoad("../assets/map/level1.xml");
		MenuManager::getInstance()->setLevelChoosen(true);

		//TODO : save data (m_sName and m_pCurrentAvatar)
	}
	else if(m_pLineEdit->isTargetedByMouse(mouseX, mouseY)){
		//Trigger the focus
		m_pLineEdit->triggerFocus();
	}
	else if(m_pBackButton->isTargetedByMouse(mouseX, mouseY)){
		// Go back
		MenuManager::getInstance()->goBack();
	}
	else if (m_pLeftArrow->isTargetedByMouse(mouseX, mouseY)){
		// Display the previous avatar in the list
		for (unsigned int i = 1; i < m_avatarVector.size(); ++i){
			if (m_avatarVector[i] == m_pCurrentAvatar){
				m_avatarVector[i]->hide();
				m_avatarVector[i-1]->show();
				m_pCurrentAvatar = m_avatarVector[i-1];
			}
		}
	}
	else if (m_pRightArrow->isTargetedByMouse(mouseX, mouseY)){
		// Display the following avatar in the list
		for (unsigned int i = 0; i < m_avatarVector.size() -1 ; ++i){
			if (m_avatarVector[i] == m_pCurrentAvatar){
				m_avatarVector[i]->hide();
				m_avatarVector[i+1]->show();
				m_pCurrentAvatar = m_avatarVector[i+1];
			}
		}
	}
}

/**
* @brief Handle key down event
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void NewGameMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

/**
* @brief Handle key up event
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void NewGameMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}

}