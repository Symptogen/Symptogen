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
	Color backgroundColor = Color(120, 120, 180, 50);
	createPlayerPanel(m_pMenuManager->getLastPlayer(), 200, 130, 400, 80, borderColor, backgroundColor);

	//Other players panel
	int posY = 280;
	for (unsigned int i = 0; i < m_pMenuManager->getPlayers().size(); ++i){
		createPlayerPanel(m_pMenuManager->getPlayers()[i], 200, posY + i*100, 400, 80, borderColor, backgroundColor);
	}

	// Load another label
	m_pLoadAnotherGameLabel = new Image("../assets/load_another_game.png", 200, 250);
	m_pMenuManager->addGuiComponent(m_pLoadAnotherGameLabel, 2);

	//Button
	m_pCreateNewGameButton = new Button(Symp::Color::GREY, 250, 500, 350, 80);
	m_pMenuManager->addGuiComponent(m_pCreateNewGameButton,0);

}

Layout* ManageGamesMenu::createPlayerPanel(Player* pPlayer, int iPosX, int iPosY, int iWidth, int iHeight, 
	Color borderColor, Color backgroundColor){
	Layout* pLayout = new Layout(iPosX, iPosY, iWidth, iHeight, borderColor, 1);

	int iVMargin = pLayout->getVerticalMargin();
	int iHMargin = pLayout->getHorizontalMargin();

	//Background + button for event
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
	Slider* slider = new Slider((float)level/gTotalLevelNumber, sliderPosX, sliderPosY, sliderWidth, sliderHeight);
	pLayout->addComponent(slider, 1, 0, false);
	m_pMenuManager->addGuiComponent(slider->getImage(), 2);
	slider->update();
	m_pMenuManager->addGuiLayout(pLayout, 1);

	m_buttonMap.insert(std::pair<Button*, Player*>(button, pPlayer));
	return pLayout;
}

void ManageGamesMenu::handleMouseClic(int mouseX, int mouseY){
	if (m_pCreateNewGameButton->isTargetedByMouse(mouseX, mouseY)){
		std::cout << "create new game clicked" << std::endl;
		NewGameMenu* newGameMenu = new NewGameMenu(m_pMenuManager);
		m_pMenuManager->setState(newGameMenu);
	}
	else if (m_pBackButton->isTargetedByMouse(mouseX, mouseY)){
		m_pMenuManager->goBack();
	}
	else{
		for (std::map<Button*,Player*>::iterator it=m_buttonMap.begin(); it!=m_buttonMap.end(); ++it){
			if (it->first->isTargetedByMouse(mouseX, mouseY)){
				ChooseYourLevelMenu* chooseYourLevelMenu = new ChooseYourLevelMenu(it->second, m_pMenuManager);
				m_pMenuManager->setState(chooseYourLevelMenu);
			}
		}
	}
}

void ManageGamesMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

void ManageGamesMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}

}