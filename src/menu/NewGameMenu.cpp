#include "NewGameMenu.h"

/** @namespace Symp */
namespace Symp {

extern int g_WindowHeight;
extern int g_WindowWidth;

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

	int avatarX = g_WindowWidth*0.3;
	int avatarY = g_WindowHeight*0.4;

	//Load the avatar available
	Image* image1 = new Image("../assets/menu/avatar1.png", avatarX, avatarY);
	m_avatarVector.push_back(image1);

	Image* image2 = new Image("../assets/menu/avatar2.png", avatarX, avatarY);
	m_avatarVector.push_back(image2);

	Image* image3 = new Image("../assets/menu/avatar3.png", avatarX, avatarY);
	m_avatarVector.push_back(image3);

	Image* image4 = new Image("../assets/menu/avatar4.png", avatarX, avatarY);
	m_avatarVector.push_back(image4);

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
	m_pBackButton = new Image("../assets/menu/back-to-menu-outgame.png", g_WindowWidth*0.05, g_WindowHeight*0.05, 0.5);
	m_pBackButton->setColor(Color::YELLOWDINO);
	m_pBackButton->enable();
	m_pBackButton->setAspectRatio(AspectRatio::EXPAND_ASPECT_RATIO);
	MenuManager::getInstance()->addGuiComponent(m_pBackButton, 0);

	//Explanations
	m_pExplanations = new Text("Choose your avatar and type your name to create a new game ", Color::YELLOWDINO, g_WindowWidth*0.3, g_WindowHeight*0.3, true);
	m_pExplanations->getIND_Entity2d()->setAlign(IND_LEFT);
	MenuManager::getInstance()->addGuiComponent(m_pExplanations, 0);

	//The title image
	int titleWidth = g_WindowWidth*0.3 ;
	int titleHeight = g_WindowHeight*0.1;
	m_pTitleImage = new Image("../assets/menu/create-new-game.png", g_WindowWidth/2 - titleWidth/2, titleHeight );
	m_pTitleImage->setWidth(titleWidth);
	m_pTitleImage->setHeight(titleHeight);
	m_pTitleImage->setAspectRatio(AspectRatio::IGNORE_ASPECT_RATIO);
	m_pTitleImage->update();
	MenuManager::getInstance()->addGuiComponent(m_pTitleImage, 0);

	// All the Avatars are initialized hidden
	for (unsigned int i = 0; i < m_avatarVector.size(); ++i){
		m_avatarVector[i]->setWidth(g_WindowWidth*0.2);
		m_avatarVector[i]->setHeight(g_WindowHeight*0.2);
		m_avatarVector[i]->update();
		m_avatarVector[i]->hide();
		MenuManager::getInstance()->addGuiComponent(m_avatarVector[i], 0);
	}
	// Only show one
	m_pCurrentAvatar = m_avatarVector[0];
	m_pCurrentAvatar->show();

	//Arrows for naviguation between avatars
	m_pArrowLayout = new Layout(g_WindowWidth*0.3, g_WindowHeight*0.6, g_WindowWidth*0.1, g_WindowHeight*0.2);

		m_pLeftArrow = new Button("../assets/menu/left_arrow.png");
		m_pArrowLayout->addComponent(m_pLeftArrow, 0, 0);

		m_pRightArrow = new Button("../assets/menu/right_arrow.png");
		m_pArrowLayout->addComponent(m_pRightArrow, 1, 0);

	MenuManager::getInstance()->addGuiLayout(m_pArrowLayout, 1);

	//Line edit
	m_pLineEdit = new LineEdit(g_WindowWidth*0.4, g_WindowHeight*0.45, g_WindowWidth*0.3, g_WindowHeight*0.1);
	MenuManager::getInstance()->addGuiComponent(m_pLineEdit, 0);
	//MenuManager::getInstance()->addGuiComponent(m_pLineEdit->getCursor(), 2);
	MenuManager::getInstance()->addGuiComponent(m_pLineEdit->getTextEntity(), 1);

	//Launch button
	m_pLaunchButton = new Image("../assets/menu/create-new-game.png", g_WindowWidth/2 - titleWidth/2, g_WindowHeight - 3*titleHeight);
	m_pLaunchButton->setColor(Color::BLUEDINO);
	m_pLaunchButton->setWidth(titleWidth);
	m_pLaunchButton->setHeight(titleHeight);
	m_pLaunchButton->setAspectRatio(AspectRatio::IGNORE_ASPECT_RATIO);
	m_pLaunchButton->enable();
	MenuManager::getInstance()->addGuiComponent(m_pLaunchButton, 1);

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
* @brief Handle key event for the #LineEdit
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void NewGameMenu::receiveKeyEvent(std::string key){
	m_pLineEdit->setText(m_pLineEdit->getText() + key);
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