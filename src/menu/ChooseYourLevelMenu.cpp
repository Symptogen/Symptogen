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
	//Color borderColor = Color(180, 100, 100);
	m_pPlayerLayout = new Layout(210, 40, 380, 120);

		//Avatar
		std::ostringstream oss;
		oss << m_pPlayer->getAvatarIndex();
		std::string avatarIndex = oss.str();

		Image* image = new Image(std::string("../assets/dino" + avatarIndex + ".png").c_str());
		m_pPlayerLayout->addComponent(image, 0, 0);

		std::string name = m_pPlayer->getName();

	m_pMenuManager->addGuiLayout(m_pPlayerLayout, 1);
		
	// Level slider
	m_pSliderLayout = new Layout(200, 180, 400, 60);

		int level = m_pPlayer->getCurrentLevel();
		int iVMargin = m_pSliderLayout->getVerticalMargin();
		int iHMargin = m_pSliderLayout->getHorizontalMargin();
		int sliderHeight = 20;
		Slider* slider = new Slider((float)level/gTotalLevelNumber, m_pSliderLayout->getPosX() + iHMargin, m_pSliderLayout->getPosY() + iVMargin,
			m_pSliderLayout->getWidth() - 2*iHMargin, sliderHeight);
		m_pSliderLayout->addComponent(slider, 1, 0, false);
		m_pMenuManager->addGuiComponent(slider->getImage(), 2);
		slider->update();

	m_pMenuManager->addGuiLayout(m_pSliderLayout, 1);

	// choose your level label
	m_pChooseLabel = new Image("../assets/load_a_game.png", 200, 250);
	m_pMenuManager->addGuiComponent(m_pChooseLabel, 2);

	//Buttons
	int nbColumns = 3;
	int row, column;
	m_pButtonLayout = new Layout(230, 280, 250, 250);

	for (unsigned int i = 1; i < gTotalLevelNumber; ++i){

 		std::ostringstream oss;
		oss << i;
		std::string levelIndex = oss.str();

		Button* button = new Button(levelIndex, Symp::Color::GREY);

		if (i > m_pPlayer->getCurrentLevel()) button->disable();

		row = (int)(i - 1)/nbColumns;
		column = (int)(i%nbColumns)-1;
		if (column == -1) column = nbColumns-1;
		m_pButtonLayout->addComponent(button, row, column);
	}

	m_pMenuManager->addGuiLayout(m_pButtonLayout, 2);
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