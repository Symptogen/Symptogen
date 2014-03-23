#include "NewGameMenu.h"
#include "Player.h"
#ifdef _WIN32
#include <string>
#endif

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

	int avatarX = g_WindowWidth*0.298;
	int avatarY = g_WindowHeight*0.4;

	m_avatarVector.clear();

	//Load the avatar available
	Image* image0 = new Image("../assets/menu/avatar0.png", avatarX, avatarY);
	m_avatarVector.push_back(image0);

	Image* image1 = new Image("../assets/menu/avatar1.png", avatarX, avatarY);
	m_avatarVector.push_back(image1);

	Image* image2 = new Image("../assets/menu/avatar2.png", avatarX, avatarY);
	m_avatarVector.push_back(image2);

	Image* image3 = new Image("../assets/menu/avatar3.png", avatarX, avatarY);
	m_avatarVector.push_back(image3);

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

	m_background = new Image("../assets/menu/new-game.png");
	m_background->setWidth(g_WindowWidth);
	m_background->setHeight(g_WindowHeight);
	m_background->setAspectRatio(AspectRatio::IGNORE_ASPECT_RATIO);
	MenuManager::getInstance()->addGuiComponent(m_background, 0);
	m_background->update();

	// The go back button up-left of the window
	m_pBackButton = new Image("../assets/menu/back-to-menu-outgame.png", g_WindowWidth*0.05, g_WindowHeight*0.05, 0.5);
	m_pBackButton->setColor(Color::YELLOWDINO);
	m_pBackButton->enable();
	m_pBackButton->setAspectRatio(AspectRatio::EXPAND_ASPECT_RATIO);
	MenuManager::getInstance()->addGuiComponent(m_pBackButton, 1);

	//Explanations
	m_pExplanation1 = new Text("NAME : ", Color::WHITE, g_WindowWidth*0.4, g_WindowHeight*0.42, true);
	m_pExplanation1->getIND_Entity2d()->setAlign(IND_LEFT);
	MenuManager::getInstance()->addGuiComponent(m_pExplanation1, 1);

	// All the Avatars are initialized hidden
	for (unsigned int i = 0; i < m_avatarVector.size(); ++i){
		m_avatarVector[i]->setWidth(g_WindowWidth*0.2);
		m_avatarVector[i]->setHeight(g_WindowHeight*0.2);
		m_avatarVector[i]->update();
		m_avatarVector[i]->hide();
		MenuManager::getInstance()->addGuiComponent(m_avatarVector[i], 1);
	}
	// Only show one
	m_iCurrentAvatar = 0;
	m_avatarVector[0]->show();

	//Arrows for naviguation between avatars
	m_pArrowLayout = new Layout(g_WindowWidth*0.305, g_WindowHeight*0.6, g_WindowWidth*0.1, g_WindowHeight*0.2);

		m_pLeftArrow = new Button("../assets/menu/left_arrow.png");
		m_pArrowLayout->addComponent(m_pLeftArrow, 0, 0);

		m_pRightArrow = new Button("../assets/menu/right_arrow.png");
		m_pArrowLayout->addComponent(m_pRightArrow, 1, 0);

	MenuManager::getInstance()->addGuiLayout(m_pArrowLayout, 1);

	//Explanations
	m_pExplanation2 = new Text("Select your avatar", Color::WHITE, g_WindowWidth*0.34, g_WindowHeight*0.64, true);
	m_pExplanation2->getIND_Entity2d()->setAlign(IND_CENTER);
	MenuManager::getInstance()->addGuiComponent(m_pExplanation2, 1);

	//Line edit
	m_pLineEdit = new LineEdit(g_WindowWidth*0.4, g_WindowHeight*0.45, g_WindowWidth*0.3, g_WindowHeight*0.1);
	MenuManager::getInstance()->addGuiComponent(m_pLineEdit, 1);
	MenuManager::getInstance()->addGuiComponent(m_pLineEdit->getCursor(), 2);
	MenuManager::getInstance()->addGuiComponent(m_pLineEdit->getTextEntity(), 1);

	//Launch button
	m_pLaunchButton = new Image("../assets/menu/create-new-game.png", g_WindowWidth*0.75, g_WindowHeight*0.45);
	m_pLaunchButton->setColor(Color::BLUEDINO);
	m_pLaunchButton->setHeight(g_WindowHeight*0.1);
	m_pLaunchButton->setAspectRatio(AspectRatio::KEEP_ASPECT_RATIO);
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
		
		int index = MenuManager::getInstance()->getPlayerIndex();
		MenuManager::getInstance()->setPlayerIndex(index + 1);
		
		Player* player = new Player(index, m_pLineEdit->getText(), m_iCurrentAvatar, 1);
		MenuManager::getInstance()->setLastPlayer(player);
		MenuManager::getInstance()->setHasPlayerDataChanged(true);
		MenuManager::getInstance()->setLevelToLoad("../assets/map/level1.xml");
		MenuManager::getInstance()->setLevelChoosen(true);

	}
	else if(m_pLineEdit->isTargetedByMouse(mouseX, mouseY)){
		//Trigger the focus
		m_pLineEdit->triggerFocus();
	}else if(m_pLineEdit->hasFocus() && !m_pLineEdit->isTargetedByMouse(mouseX, mouseY)){
		//Trigger the focus
		m_pLineEdit->triggerFocus();
	}else if(m_pBackButton->isTargetedByMouse(mouseX, mouseY)){
		// Go back
		MenuManager::getInstance()->goBack();
	}
	else if (m_pLeftArrow->isTargetedByMouse(mouseX, mouseY)){
		// Display the previous avatar in the list
		if(m_iCurrentAvatar != 0){
			m_avatarVector[m_iCurrentAvatar]->hide();
			m_avatarVector[m_iCurrentAvatar-1]->show();
			m_iCurrentAvatar = m_iCurrentAvatar-1;
		}
	}
	else if (m_pRightArrow->isTargetedByMouse(mouseX, mouseY)){
		// Display the following avatar in the list
		if(m_iCurrentAvatar != m_avatarVector.size()-1){
			m_avatarVector[m_iCurrentAvatar]->hide();
			m_avatarVector[m_iCurrentAvatar+1]->show();
			m_iCurrentAvatar = m_iCurrentAvatar+1;
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
	m_pLineEdit->setText(m_pLineEdit->getText() + key.c_str());
}

/**
* @brief Erase the character which is before the cursor index
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void NewGameMenu::erasePreviousCharacter(){
	m_pLineEdit->erasePreviousToCursor();
}

/**
* @brief Erase the character which is after the cursor index
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void NewGameMenu::eraseNextCharacter(){
	m_pLineEdit->eraseNextToCursor();
}

/**
* @brief Move the cursor of the #LineEdit to the left
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void NewGameMenu::moveCursorLeft(){
	m_pLineEdit->moveCursorLeft();
}

/**
* @brief Move the cursor of the #LineEdit to the right
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void NewGameMenu::moveCursorRight(){
	m_pLineEdit->moveCursorRight();
}

/**
* @brief Handle key down event
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void NewGameMenu::keyDownPressed(){
}

/**
* @brief Handle key up event
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void NewGameMenu::keyUpPressed(){
}

}