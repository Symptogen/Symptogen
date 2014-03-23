#include "ChooseYourLevelMenu.h"
#include <sstream>

/** @namespace Symp */
namespace Symp {

extern int g_WindowHeight;
extern int g_WindowWidth;


/**
* @brief ChooseYourLevel constructor
* Responsible for the initialization of the private attributes of the #ChooseYourLevelMenu class. This function
* is not responsible for drawing the graphical elements that compose the menu, the #init() function is.
* @param pPlayer the reference to the #Player
* @param pMenuManager the reference to the #MenuManager
* @see Player
* @see MenuManager
* @see State
* @see init()
* @see ~ChooseYourLevelMenu()
*/
ChooseYourLevelMenu::ChooseYourLevelMenu(Player* pPlayer) 
	: State(), m_pPlayer(pPlayer) {
}

/**
* @brief ChooseYourLevel elements initialization
* The elements that compose the menu are created in this function.
* @see Player
* @see MenuManager
* @see State
* @see end()
* @see ChooseYourLevelMenu()
*/
void ChooseYourLevelMenu::init(){

	m_background = new Image("../assets/menu/level-background.png");
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


	// Avatar choice panel
	m_pPlayerLayout = new Layout(g_WindowWidth*0.32, g_WindowHeight*0.28, g_WindowWidth*0.35, g_WindowHeight*0.15);

		//Retrive the index and turn it into a string
		std::ostringstream oss;
		oss << m_pPlayer->getAvatarIndex();
		std::string avatarIndex = oss.str();

		// Display the avatar registered with this player
		Image* image = new Image(std::string("../assets/menu/avatar" + avatarIndex + ".png").c_str());
		m_pPlayerLayout->addComponent(image, 0, 0);

		// Retrieve the name of the player
		Text* name = new Text(m_pPlayer->getName(), Color::BLUEDINO, g_WindowWidth*0.4,g_WindowHeight*0.3);
		name->getIND_Entity2d()->setAlign(IND_LEFT);
		MenuManager::getInstance()->addGuiComponent(name, 1);

		// Retrieve the last level unlocked
		Text* lastLevel = new Text("Last level unlocked : " + getLevelName(m_pPlayer->getCurrentLevel()), Color::BLUEDINO, g_WindowWidth*0.48,g_WindowHeight*0.41, true);
		lastLevel->getIND_Entity2d()->setAlign(IND_LEFT);
		MenuManager::getInstance()->addGuiComponent(lastLevel, 1);

	MenuManager::getInstance()->addGuiLayout(m_pPlayerLayout, 1);
		
	// Player progress bar
	m_pSliderLayout = new Layout(g_WindowWidth*0.4, g_WindowHeight*0.38, g_WindowWidth*0.3, g_WindowHeight*0.1);

		int level = m_pPlayer->getCurrentLevel();
		int iVMargin = m_pSliderLayout->getVerticalMargin();
		int iHMargin = m_pSliderLayout->getHorizontalMargin();
		int sliderHeight = 20;

		//Create the Slider
		Slider* slider = new Slider((float)level/gTotalLevelNumber, m_pSliderLayout->getPosX() + iHMargin, m_pSliderLayout->getPosY() + iVMargin,
			m_pSliderLayout->getWidth() - 2*iHMargin, sliderHeight);
		
		// Add the Slider background to the Layout
		m_pSliderLayout->addComponent(slider, 1, 0, false);
		// Add the Slider foreground directly to the MenuManager
		MenuManager::getInstance()->addGuiComponent(slider->getImage(), 2);
		//Update
		slider->update();

	MenuManager::getInstance()->addGuiLayout(m_pSliderLayout, 1);

	// Display the label "Choose your level"
	m_pChooseLabel = new Image("../assets/menu/load_a_game_black.png", g_WindowWidth/2 - (g_WindowWidth*0.3)/3.8, g_WindowHeight*0.5, 0.5);
	MenuManager::getInstance()->addGuiComponent(m_pChooseLabel, 2);

	// Set up the layout of level buttons
	int nbColumns = 3;
	int row = 1;
	int column = 1;
	m_pButtonLayout = new Layout(g_WindowWidth/2 - (g_WindowWidth*0.3)/2, 2*g_WindowHeight/3 - (g_WindowHeight*0.25)/2, g_WindowWidth*0.3, g_WindowHeight*0.25);

	// Retrieve all the levels
	for (unsigned int i = 1; i < gTotalLevelNumber; ++i){

		//Create a Button that will display the level number
		Button* button = new Button("", Color::WHITE);
		button->update();
		//Disable buttons that the Player hasn't finished yet
		if (i > m_pPlayer->getCurrentLevel()){
			button->disable();
		}

		for (int j =1; j<nbColumns+1; ++j){
			if(static_cast<int>(i) - j*3 > 0 ){
				column = static_cast<int>(i)-j*3;
			}else {
				column = static_cast<int>(i) - (j-1)*3 -1;
				row = j-1;
				break;
			}
		}
		m_pButtonLayout->addComponent(button, column, row);
		m_levelButtonVector.push_back(button);
	}

	MenuManager::getInstance()->addGuiLayout(m_pButtonLayout, 1);

	// Set up the layout of level buttons
	m_pTextLayout = new Layout(g_WindowWidth/2 - (g_WindowWidth*0.2)/2, 2*g_WindowHeight/3 - (g_WindowHeight*0.25)/2, g_WindowWidth*0.3, g_WindowHeight*0.25);
	// Retrieve all the levels
	for (unsigned int i = 1; i < gTotalLevelNumber; ++i){

		// transform the level index into a string
 		std::ostringstream oss;
		oss << i;
		std::string levelIndex = oss.str();

		//Add the level text to the Button
		Text* text = new Text(levelIndex, Color::BLUEDINO);
		text->getIND_Entity2d()->setAlign(IND_RIGHT);

		//Disable buttons that the Player hasn't finished yet
		if (i > m_pPlayer->getCurrentLevel()){
			text->disable();
		}

		//Place the Button into the layout
		for (int j =1; j<nbColumns+1; ++j){
			if(static_cast<int>(i) - j*3 > 0 ){
				column = static_cast<int>(i)-j*3;
			}else {
				column = static_cast<int>(i) - (j-1)*3 -1;
				row = j-1;
				break;
			}
		}
		m_pTextLayout->addComponent(text, column, row);

	}
	MenuManager::getInstance()->addGuiLayout(m_pTextLayout, 2);

	
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
void ChooseYourLevelMenu::handleMouseClic(int mouseX, int mouseY){
	// Handle a click on the go back button
	if (m_pBackButton->isTargetedByMouse(mouseX, mouseY)){
		MenuManager::getInstance()->goBack();
	}
	// Handle a clic on a level button
	for (unsigned int i = 0; i < m_levelButtonVector.size(); ++i){
		//If the player clic on a level he can play
		if (m_levelButtonVector[i]->isTargetedByMouse(mouseX, mouseY) && i < m_pPlayer->getCurrentLevel()) {

			//Convert int to string
			std::ostringstream oss;
			oss << i+1;
			std::string levelNumber = oss.str();

			MenuManager::getInstance()->setLevelToLoad("../assets/map/level"+ levelNumber +".xml");
			MenuManager::getInstance()->setLevelChoosen(true);
		}
	}
}

std::string ChooseYourLevelMenu::getLevelName(int level){
	std::string chapter = " CHAPTER ";
	std::string index = " LEVEL ";
	for (int i =1; i<4; ++i){
		if(level- i*3 > 0 ){
			level = level-i*3;
		}else if(level- i*3 < 0){
			std::ostringstream oss;
			oss << level;
			index += oss.str();

			std::ostringstream ossBis;
			ossBis << i;
			chapter += ossBis.str();
			break;
		}
	}
	return chapter + "-" + index;	
}

/**
* @brief Handle key down event
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void ChooseYourLevelMenu::keyDownPressed(){
}

/**
* @brief Handle key up event
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void ChooseYourLevelMenu::keyUpPressed(){
}

}