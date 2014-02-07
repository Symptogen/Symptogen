#include "ManageGamesMenu.h"
#include <sstream>

#include "NewGameMenu.h"
#include "ChooseYourLevelMenu.h"

namespace Symp {
	
ManageGamesMenu::ManageGamesMenu(MenuManager* pMenuManager)
	:State(pMenuManager){
	
}

ManageGamesMenu::~ManageGamesMenu()
{
}

void ManageGamesMenu::init(){

	//Back button
	m_pBackButton = new Button("../assets/back.png");
	m_pMenuManager->addGuiComponent(m_pBackButton, 0);

	//Title
	m_pTitleImage = new Image("../assets/manage_game.png", 220, 10);
	m_pMenuManager->addGuiComponent(m_pTitleImage, 0);


	//Image Current Game Label
	m_pCurrentGameLabel = new Image("../assets/current_game.png", 200, 100);
	m_pMenuManager->addGuiComponent(m_pCurrentGameLabel, 2);

	// Last Player panel
	Color borderColor = Color(180, 100, 100);
	// m_pLastPlayerLayout = new Layout(200, 130, 400, 80, borderColor, 1);

	// 	//Button 
	// 	Color backgroundColor = Color(120, 120, 180, 50);
	// 	m_TestButton = new Button(backgroundColor, m_pLastPlayerLayout->getPosX(), m_pLastPlayerLayout->getPosY(),
	// 		m_pLastPlayerLayout->getWidth(), m_pLastPlayerLayout->getHeight());
	// 	m_pMenuManager->addGuiComponent(m_TestButton, 0);

	// 	//Avatar
	// 	std::ostringstream oss;
	// 	oss << m_pMenuManager->getLastPlayer()->getAvatarIndex();
	// 	std::string avatarIndex = oss.str();

	// 	Image* image = new Image(std::string("../assets/dino" + avatarIndex + ".png").c_str(), m_pLastPlayerLayout->getPosX(), 
	// 		m_pLastPlayerLayout->getPosY());
	// 	image->setWidth(m_pLastPlayerLayout->getWidth());
	// 	image->setHeight(m_pLastPlayerLayout->getHeight());
	// 	m_pLastPlayerLayout->addComponent(image, 0, 0, false);

	// 	std::string name = m_pMenuManager->getLastPlayer()->getName();
		

	// 	//Slider
	// 	int level = m_pMenuManager->getLastPlayer()->getCurrentLevel();
	// 	Slider* slider = new Slider(level * gTotalLevelNumber, m_pLastPlayerLayout->getPosX(), m_pLastPlayerLayout->getPosY());
	// 	m_pLastPlayerLayout->addComponent(slider, 1, 0, false);
	// 	m_pMenuManager->addGuiComponent(slider->getImage(), 2);

	// 	m_pMenuManager->addGuiLayout(m_pLastPlayerLayout, 1);

	createPlayerPanel(m_pMenuManager->getLastPlayer(), 200, 130, 400, 80, borderColor);

	// Load another label
	m_pLoadAnotherGameLabel = new Image("../assets/load_another_game.png", 200, 250);
	m_pMenuManager->addGuiComponent(m_pLoadAnotherGameLabel, 2);

	//Button
	m_pCreateNewGameButton = new Button(Symp::Color::GREY, 250, 480, 350, 80);
	m_pMenuManager->addGuiComponent(m_pCreateNewGameButton,0);

}

Layout* ManageGamesMenu::createPlayerPanel(Player* pPlayer, int iPosX, int iPosY, int iWidth, int iHeight, Color color){
	Layout* pLayout = new Layout(iPosX, iPosY, iWidth, iHeight, color, 1);

	int iVMargin = pLayout->getVerticalMargin();
	int iHMargin = pLayout->getHorizontalMargin();

	//Background + button for event
	Color backgroundColor = Color(120, 120, 180, 50);
	Button* button = new Button(backgroundColor, iPosX, iPosY,iWidth, iHeight);
	m_pMenuManager->addGuiComponent(button, 0);

	//Avatar
	std::ostringstream oss;
	oss << pPlayer->getAvatarIndex();
	std::string avatarIndex = oss.str();

	Image* image = new Image(std::string("../assets/dino" + avatarIndex + ".png").c_str(), iPosX + iHMargin, iPosY + iVMargin);
	image->setWidth(iWidth - 2*iHMargin);
	image->setHeight(iHeight - 2*iVMargin);
	pLayout->addComponent(image, 0, 0, false);
	image->update();

	//Name
	std::string name = pPlayer->getName();
	//TODO : write the name

	// Level slider
	int level = pPlayer->getCurrentLevel();
	int sliderHeight = 20;
	int sliderPosX = image->getIND_Entity2d()->getSurface()->getWidth()*image->getIND_Entity2d()->getScaleX() + image->getPosX() + 2 * iHMargin;
	int sliderPosY = iPosY + iHeight - (sliderHeight + iVMargin);
	int sliderWidth = iWidth - 4*iHMargin - image->getIND_Entity2d()->getSurface()->getWidth()*image->getIND_Entity2d()->getScaleY();
	std::cout << "level : " << level << " / " << (float) level/gTotalLevelNumber << std::endl;
	Slider* slider = new Slider((float)level/gTotalLevelNumber, sliderPosX, sliderPosY, sliderWidth, sliderHeight);
	pLayout->addComponent(slider, 1, 0, false);
	m_pMenuManager->addGuiComponent(slider->getImage(), 2);
	slider->update();
	m_pMenuManager->addGuiLayout(pLayout, 1);

	return pLayout;
}

void ManageGamesMenu::handleMouseClic(int mouseX, int mouseY){
	if (m_pCreateNewGameButton->isTargetedByMouse(mouseX, mouseY)){
		std::cout << "create new game clicked" << std::endl;
		NewGameMenu* newGameMenu = new NewGameMenu(m_pMenuManager);
		m_pMenuManager->setState(newGameMenu);
	}
	else if (m_TestButton->isTargetedByMouse(mouseX, mouseY)){
		ChooseYourLevelMenu* chooseYourLevelMenu = new ChooseYourLevelMenu(m_pMenuManager->getLastPlayer(), m_pMenuManager);
		m_pMenuManager->setState(chooseYourLevelMenu);
	}
	else if (m_pBackButton->isTargetedByMouse(mouseX, mouseY)){
		m_pMenuManager->goBack();
	}
}

void ManageGamesMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

void ManageGamesMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}

}