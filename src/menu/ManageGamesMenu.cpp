#include "ManageGamesMenu.h"
#include <sstream>

#include "NewGameMenu.h"
#include "ChooseYourLevelMenu.h"

/** @namespace Symp */
namespace Symp {

/**
* @brief ManageGamesMenu constructor
* Responsible for the initialization of the private attributes of the #ManageGamesMenuMenu class. This function
* is not responsible for drawing the graphical elements that compose the menu, the #init() function is.
* @see Player
* @see MenuManager
* @see State
* @see init()
* @see ~ManageGamesMenu()
*/
ManageGamesMenu::ManageGamesMenu()
	:State(){
	
}

/**
* @brief ManageGamesMenu elements initialization
* The elements that compose the menu are created in this function.
* @see Player
* @see MenuManager
* @see State
* @see end()
*/
void ManageGamesMenu::init(){

	//GoBack button top left of the window
	m_pBackButton = new Button("../assets/menu/back.png");
	MenuManager::getInstance()->addGuiComponent(m_pBackButton, 0);

	//Title
	m_pTitleImage = new Image("../assets/menu/manage_game.png", 220, 10);
	MenuManager::getInstance()->addGuiComponent(m_pTitleImage, 0);


	//Image that display the "Current Game" Label
	m_pCurrentGameLabel = new Image("../assets/menu/current_game.png", 200, 100);
	MenuManager::getInstance()->addGuiComponent(m_pCurrentGameLabel, 2);

	// Last Player panel
	Color borderColor = Color(180, 100, 100);
	Color backgroundColor = Color(120, 120, 180, 50);
	createPlayerPanel(MenuManager::getInstance()->getLastPlayer(), 200, 130, 400, 80, borderColor, backgroundColor);

	//Image that display the "Load another game" Label
	m_pLoadAnotherGameLabel = new Image("../assets/menu/load_another_game.png", 200, 250);
	MenuManager::getInstance()->addGuiComponent(m_pLoadAnotherGameLabel, 2);

	//Other players panel
	int posY = 280;
	for (unsigned int i = 0; i < MenuManager::getInstance()->getPlayers().size(); ++i){
		createPlayerPanel(MenuManager::getInstance()->getPlayers()[i], 200, posY + i*100, 400, 80, borderColor, backgroundColor);
	}

	//Create a new Player Button
	m_pCreateNewGameButton = new Button(Symp::Color::GREY, 250, 500, 350, 80);
	MenuManager::getInstance()->addGuiComponent(m_pCreateNewGameButton,0);

}

/**
* @brief Create a panel with the #Player data
* The elements that compose the #Player panel are created in this function. This panel
* is composed by a #Layout that group elements but that do not manage their position and shape, and a #Button
* that can receive events. The panel can be customized with two colors : one for the border of the #Layout component, one for the background of the #Button component.
* @param pPlayer the reference to the #Player this panel is made for
* @param iPosX the x coordinate of the upper-left corner of the panel in pixels
* @param iPosY the y coordinate of the upper-left corner of the panel in pixels
* @param iWidth the width of the panel in pixels
* @param iHeight the height of the panel in pixels
* @param borderColor the color of the border of the panel
* @param backgoundColor the color of the background of the panel
* @return pLayout the reference to the #Layout that contain the #Player graphcal data
* @see Player
* @see MenuManager
* @see State
* @see init()
*/
Layout* ManageGamesMenu::createPlayerPanel(Player* pPlayer, int iPosX, int iPosY, int iWidth, int iHeight, 
	Color borderColor, Color backgroundColor){
	//Creation of the main Layout
	Layout* pLayout = new Layout(iPosX, iPosY, iWidth, iHeight, borderColor, LayoutFillAttribute::BORDER);

	int iVMargin = pLayout->getVerticalMargin();
	int iHMargin = pLayout->getHorizontalMargin();

	//Creation of the Button
	Button* button = new Button(backgroundColor, iPosX, iPosY,iWidth, iHeight);
	MenuManager::getInstance()->addGuiComponent(button, 0);

	//Retrieve the avatar index
	std::ostringstream oss;
	oss << pPlayer->getAvatarIndex();
	std::string avatarIndex = oss.str();

	// Display the avatar 
	Image* image = new Image(std::string("../assets/menu/avatar" + avatarIndex + ".png").c_str(), iPosX + iHMargin, iPosY + iVMargin);
	image->setWidth(iWidth - 2*iHMargin);
	image->setHeight(iHeight - 2*iVMargin);
	pLayout->addComponent(image, 0, 0, false);
	image->update();

	//Retrieve the player's name
	std::string name = pPlayer->getName();
	//TODO : write the name

	// Display the Player progression bar
	int level = pPlayer->getCurrentLevel();
	int sliderHeight = 20;
	int sliderPosX = image->getIND_Entity2d()->getSurface()->getWidth()*image->getIND_Entity2d()->getScaleX() + image->getPosX() + 2 * iHMargin;
	int sliderPosY = iPosY + iHeight - (sliderHeight + iVMargin);
	int sliderWidth = iWidth - 4*iHMargin - image->getIND_Entity2d()->getSurface()->getWidth()*image->getIND_Entity2d()->getScaleY();
	Slider* slider = new Slider((float)level/gTotalLevelNumber, sliderPosX, sliderPosY, sliderWidth, sliderHeight);
	pLayout->addComponent(slider, 1, 0, false);
	MenuManager::getInstance()->addGuiComponent(slider->getImage(), 2);
	slider->update();
	MenuManager::getInstance()->addGuiLayout(pLayout, 1);

	//Associate the button with the player
	m_buttonMap.insert(std::pair<Button*, Player*>(button, pPlayer));

	//Return the main Layout
	return pLayout;
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
void ManageGamesMenu::handleMouseClic(int mouseX, int mouseY){
	if (m_pCreateNewGameButton->isTargetedByMouse(mouseX, mouseY)){
		//Creates a new Player
		NewGameMenu* newGameMenu = new NewGameMenu();
		MenuManager::getInstance()->setState(newGameMenu);
	}
	else if (m_pBackButton->isTargetedByMouse(mouseX, mouseY)){
		//Go back
		MenuManager::getInstance()->goBack();
	}
	else{
		// Click on a Player, then display its data in the ChooseYourLevelMenu
		for (std::map<Button*,Player*>::iterator it=m_buttonMap.begin(); it!=m_buttonMap.end(); ++it){
			if (it->first->isTargetedByMouse(mouseX, mouseY)){
				ChooseYourLevelMenu* chooseYourLevelMenu = new ChooseYourLevelMenu(it->second);
				MenuManager::getInstance()->setState(chooseYourLevelMenu);
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
void ManageGamesMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

/**
* @brief Handle key up event
* @see MenuManager
* @see State
* @see InputManager
* @see init()
*/
void ManageGamesMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}

}