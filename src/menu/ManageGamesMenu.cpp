#include "ManageGamesMenu.h"
#include <sstream>

#include "NewGameMenu.h"
#include "ChooseYourLevelMenu.h"

/** @namespace Symp */
namespace Symp {

extern int g_WindowHeight;
extern int g_WindowWidth;

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

	m_background = new Image("../assets/menu/manage-games-background.png");
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


	//Image that display the "Current Game" Label
	m_pCurrentGameLabel = new Image("../assets/menu/current_game.png", g_WindowWidth*0.25, g_WindowHeight*0.26, 0.7);
	MenuManager::getInstance()->addGuiComponent(m_pCurrentGameLabel, 2);

	// Last Player panel
	createPlayerPanel(MenuManager::getInstance()->getLastPlayer(), g_WindowWidth*0.25,  g_WindowHeight*0.3, g_WindowWidth*0.6, g_WindowHeight*0.1, Color::YELLOWDINO, Color::BLUEDINO);

	// If there is more than one player
	if(MenuManager::getInstance()->getPlayers().size() > 1 ) {
		//Image that display the "Load another game" Label
		m_pLoadAnotherGameLabel = new Image("../assets/menu/load_another_game.png", g_WindowWidth*0.25, g_WindowHeight*0.46, 0.7);
		MenuManager::getInstance()->addGuiComponent(m_pLoadAnotherGameLabel, 2);

		//Other players panel
		int posY = g_WindowHeight*0.5;
		for (unsigned int i = 0; i < MenuManager::getInstance()->getPlayers().size(); ++i){
			if(MenuManager::getInstance()->getPlayers()[i] != MenuManager::getInstance()->getLastPlayer()) {
				createPlayerPanel(MenuManager::getInstance()->getPlayers()[i], g_WindowWidth*0.25, posY + i*g_WindowHeight*0.12, g_WindowWidth*0.6, g_WindowHeight*0.1, Color::YELLOWDINO, Color::BLUEDINO);
			}		
		}

	}
	
	//Create a new Player Button
	m_pCreateNewGameButton = new Image("../assets/menu/create-new-game.png", g_WindowWidth*0.42, g_WindowHeight*0.8);
	m_pCreateNewGameButton->setHeight(g_WindowHeight*0.1);
	m_pCreateNewGameButton->setColor(Color::YELLOWDINO);
	m_pCreateNewGameButton->enable();
	m_pCreateNewGameButton->setAspectRatio(AspectRatio::KEEP_ASPECT_RATIO);
	if(MenuManager::getInstance()->getPlayers().size() >= m_LIMIT_NB_LEVEL)
		m_pCreateNewGameButton->hide();
	MenuManager::getInstance()->addGuiComponent(m_pCreateNewGameButton, 0);

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
	//Layout* pLayout = new Layout(iPosX, iPosY, iWidth, iHeight, borderColor, LayoutFillAttribute::BORDER);

	int iCrossWidth = g_WindowWidth*0.1;

	Layout* persoLayout = new Layout(iPosX, iPosY, iWidth - (iCrossWidth + 10), iHeight, borderColor, LayoutFillAttribute::BORDER);
	int iVMargin = persoLayout->getVerticalMargin();
	int iHMargin = persoLayout->getHorizontalMargin();

	//Creation of the Button
	Button* button = new Button(backgroundColor, iPosX, iPosY, iWidth - (iCrossWidth + 10), iHeight);
	MenuManager::getInstance()->addGuiComponent(button, 0);

	//Retrieve the avatar index
	std::ostringstream oss;
	oss << pPlayer->getAvatarIndex();
	std::string avatarIndex = oss.str();

	// Display the avatar 
	Image* image = new Image(std::string("../assets/menu/avatar" + avatarIndex + ".png").c_str(), iPosX + iHMargin, iPosY + iVMargin);
	image->setWidth(iWidth - 2*iHMargin);
	image->setHeight(iHeight - 2*iVMargin);
	persoLayout->addComponent(image, 0, 0, false);
	image->update();

	// Display the Player progression bar
	int level = pPlayer->getCurrentLevel();
	int sliderHeight = g_WindowHeight*0.03;
	int sliderPosX = image->getIND_Entity2d()->getSurface()->getWidth()*image->getIND_Entity2d()->getScaleX() + image->getPosX() + 2 * iHMargin;
	int sliderPosY = iPosY + iHeight - (sliderHeight + iVMargin);
	int sliderWidth = iWidth - iCrossWidth - 6*iHMargin - image->getIND_Entity2d()->getSurface()->getWidth()*image->getIND_Entity2d()->getScaleY();
	Slider* slider = new Slider((float)level/gTotalLevelNumber, sliderPosX, sliderPosY, sliderWidth, sliderHeight);
	persoLayout->addComponent(slider, 1, 0, false);
	MenuManager::getInstance()->addGuiComponent(slider->getImage(), 2);
	slider->update();
	MenuManager::getInstance()->addGuiLayout(persoLayout, 1);

	// Retrieve the last level unlocked
	Text* lastLevel = new Text("Last level unlocked : " + getLevelName(pPlayer->getCurrentLevel()), Color(20, 20, 20), sliderPosX + sliderWidth - 10, iPosY + sliderHeight, true);
	lastLevel->getIND_Entity2d()->setAlign(IND_RIGHT);
	MenuManager::getInstance()->addGuiComponent(lastLevel, 1);

	//Retrieve the player's name
	Text* name = new Text(pPlayer->getName(), Color::BLUEDINO, sliderPosX, iPosY);
	name->getIND_Entity2d()->setAlign(IND_LEFT);
	MenuManager::getInstance()->addGuiComponent(name, 1);

	//Cross Image
	Image* cross = new Image("../assets/menu/cross.png", iPosX + iWidth - (iCrossWidth + 2*iHMargin), iPosY + 2*iVMargin);
	cross->setColor(Color::RED);
	cross->enable();
	MenuManager::getInstance()->addGuiComponent(cross, 0);

	//Associate the button with the player
	m_buttonMap.insert(std::pair<Button*, Player*>(button, pPlayer));
	m_crossMap.insert(std::pair<Image*, Player*>(cross, pPlayer));

	//Return the main Layout
	return persoLayout;
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
	if(MenuManager::getInstance()->getPlayers().size() < m_LIMIT_NB_LEVEL){
		m_pCreateNewGameButton->show();
	}
	else{
		m_pCreateNewGameButton->hide();
	}

	if (m_pCreateNewGameButton->isTargetedByMouse(mouseX, mouseY) && MenuManager::getInstance()->getPlayers().size() < m_LIMIT_NB_LEVEL){
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
		for (std::map<Image*,Player*>::iterator it=m_crossMap.begin(); it!=m_crossMap.end(); ++it){
			if (it->first->isTargetedByMouse(mouseX, mouseY)){
				MenuManager::getInstance()->erasePlayerData(it->second);
			}
		}
	}
}

std::string ManageGamesMenu::getLevelName(int level){
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