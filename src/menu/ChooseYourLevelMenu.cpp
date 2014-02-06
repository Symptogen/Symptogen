#include "ChooseYourLevelMenu.h"
#include <sstream>

namespace Symp {


ChooseYourLevelMenu::ChooseYourLevelMenu(Player* pPlayer, MenuManager* pMenuManager) 
	: State(pMenuManager), m_pPlayer(pPlayer) {
}

void ChooseYourLevelMenu::init(){
	//Back button
	m_pBackButton = new Button("../assets/back.png");
	m_pMenuManager->addGuiComponent(m_pBackButton, 0);

	// Last Player panel
	Color borderColor = Color(180, 100, 100);
	m_pPlayerLayout = new Layout(200, 130, 400, 80, borderColor, 1);

		//Avatar
		std::ostringstream oss;
		oss << m_pPlayer->getAvatarIndex();
		std::string avatarIndex = oss.str();

		Image* image = new Image(std::string("../assets/dino" + avatarIndex + ".png").c_str());
		m_pPlayerLayout->addComponent(image, 0, 0);

		std::string name = m_pPlayer->getName();
		int level = m_pPlayer->getCurrentLevel();

	m_pMenuManager->addGuiLayout(m_pPlayerLayout, 1);

	//Image Current Game Label
	m_pChooseLabel = new Image("../assets/current_game.png", 200, 100);
	m_pMenuManager->addGuiComponent(m_pChooseLabel, 2);

	//Button
	m_pLevel1Button = new Button(Symp::Color::GREY, 250, 250, 80, 80);
	m_pMenuManager->addGuiComponent(m_pLevel1Button,0);
}

void ChooseYourLevelMenu::handleMouseClic(int mouseX, int mouseY){
	if (m_pBackButton->isTargetedByMouse(mouseX, mouseY)){
		m_pMenuManager->goBack();
	}
	else if (m_pLevel1Button->isTargetedByMouse(mouseX, mouseY)){
		m_pMenuManager->setLevelToLoad("../assets/map/map1.xml");
		m_pMenuManager->setLevelChoosen(true);
	}
}

void ChooseYourLevelMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

void ChooseYourLevelMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}

}