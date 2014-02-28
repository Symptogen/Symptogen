#include "ChooseYourLevelMenu.h"
#include <sstream>

/** @namespace Symp */
namespace Symp {

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
	//Back button top left corner
	m_pBackButton = new Button("../assets/menu/back.png");
	MenuManager::getInstance()->addGuiComponent(m_pBackButton, 0);

	// Avatar choice panel
	m_pPlayerLayout = new Layout(210, 40, 380, 120);

		//Retrive the index and turn it into a string
		std::ostringstream oss;
		oss << m_pPlayer->getAvatarIndex();
		std::string avatarIndex = oss.str();

		// Display the avatar registered with this player
		Image* image = new Image(std::string("../assets/menu/avatar" + avatarIndex + ".png").c_str());
		m_pPlayerLayout->addComponent(image, 0, 0);

		// Retrieve the name of the player
		std::string name = m_pPlayer->getName();
		//TODO : write it

	MenuManager::getInstance()->addGuiLayout(m_pPlayerLayout, 1);
		
	// Player progress bar
	m_pSliderLayout = new Layout(200, 180, 400, 60);

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
	m_pChooseLabel = new Image("../assets/menu/load_a_game.png", 200, 250);
	MenuManager::getInstance()->addGuiComponent(m_pChooseLabel, 2);

	// Set up the layout of level buttons
	int nbColumns = 3;
	int row, column;
	m_pButtonLayout = new Layout(230, 280, 250, 250);

	// Retrieve all the levels
	for (unsigned int i = 1; i < gTotalLevelNumber; ++i){

		// transform the level index into a string
 		std::ostringstream oss;
		oss << i;
		std::string levelIndex = oss.str();

		//Create a Button that will display the level number
		Button* button = new Button(levelIndex, Symp::Color::GREY);

		//Disable buttons that the Player hasn't finished yet
		if (i > m_pPlayer->getCurrentLevel()) button->disable();

		//Place the Button into the layout
		row = (int)(i - 1)/nbColumns;
		column = (int)(i%nbColumns)-1;
		if (column == -1) column = nbColumns-1;
		m_pButtonLayout->addComponent(button, row, column);
		//TODO : this part is a bit confuse, simplify it
	}

	MenuManager::getInstance()->addGuiLayout(m_pButtonLayout, 2);
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
		if (m_levelButtonVector[i]->isTargetedByMouse(mouseX, mouseY)){
			MenuManager::getInstance()->setLevelToLoad("../assets/map/map"+ m_levelButtonVector[i]->getText() +".xml");
			MenuManager::getInstance()->setLevelChoosen(true);
			//TODO : display an error message if the map file doesn't exists
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
void ChooseYourLevelMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

/**
* @brief Handle key up event
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void ChooseYourLevelMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}

}