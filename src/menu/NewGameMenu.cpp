#include "NewGameMenu.h"

namespace Symp {

NewGameMenu::NewGameMenu(MenuManager* pMenuManager) 
	: State(pMenuManager)
{

	//Temporary !
	Image* image1 = new Image("../assets/dino1.png", 150, 150);
	m_avatarVector.push_back(image1);

	Image* image2 = new Image("../assets/dino2.png", 150, 150);
	m_avatarVector.push_back(image2);

}

NewGameMenu::~NewGameMenu()
{
}

void NewGameMenu::init(){

	m_pBackButton = new Button("../assets/back.png");
	m_pMenuManager->addGuiComponent(m_pBackButton, 0);

	//Title
	m_pTitleImage = new Image("../assets/new_game.png", 200, 50);
	m_pMenuManager->addGuiComponent(m_pTitleImage, 0);

	//Avatars
	for (unsigned int i = 0; i < m_avatarVector.size(); ++i){
		m_avatarVector[i]->setWidth(100);
		m_avatarVector[i]->setHeight(100);
		m_avatarVector[i]->update();
		m_avatarVector[i]->hide();
		std::cout << m_avatarVector[i]->getAspectRatio() << std::endl;
		std::cout << m_avatarVector[i]->getWidth() << " / " << m_avatarVector[i]->getHeight() << std::endl;
		m_pMenuManager->addGuiComponent(m_avatarVector[i], 0);
	}
	m_pCurrentAvatar = m_avatarVector[1];
	m_pCurrentAvatar->show();

	//Arrows
	m_pArrowLayout = new Layout(160, 250, 80, 50, 2);

		m_pLeftArrow = new Button("../assets/left_arrow.png");
		m_pArrowLayout->addComponent(m_pLeftArrow, 0, 0);

		m_pRightArrow = new Button("../assets/right_arrow.png");
		m_pArrowLayout->addComponent(m_pRightArrow, 1, 0);

	m_pMenuManager->addGuiLayout(m_pArrowLayout, 1);

	m_pButtonLayout = new Layout(400, 300, 250, 40, 2);
		//Line edit

		//Launch button
		m_pLaunchButton = new Button("Create and Launch", Symp::Color::GREY);
		m_pButtonLayout->addComponent(m_pLaunchButton, 0, 0);

	//Settle the layout
	m_pMenuManager->addGuiLayout(m_pButtonLayout, 0);
}

void NewGameMenu::handleMouseClic(int mouseX, int mouseY){
	if (m_pLaunchButton->isTargetedByMouse(mouseX, mouseY)){
		m_pMenuManager->setLevelToLoad("../assets/map/map1.xml");
		m_pMenuManager->setLevelChoosen(true);

		//TODO : save data
		//m_sName
		//m_pCurrentAvatar
	}
	else if(m_pBackButton->isTargetedByMouse(mouseX, mouseY)){
		m_pMenuManager->goBack();
	}
	else if (m_pLeftArrow->isTargetedByMouse(mouseX, mouseY)){
		for (unsigned int i = 1; i < m_avatarVector.size(); ++i){
			if (m_avatarVector[i] == m_pCurrentAvatar){
				m_avatarVector[i]->hide();
				m_avatarVector[i-1]->show();
				m_pCurrentAvatar = m_avatarVector[i-1];
			}
		}
	}
	else if (m_pRightArrow->isTargetedByMouse(mouseX, mouseY)){
		for (unsigned int i = 0; i < m_avatarVector.size() -1 ; ++i){
			if (m_avatarVector[i] == m_pCurrentAvatar){
				m_avatarVector[i]->hide();
				m_avatarVector[i+1]->show();
				m_pCurrentAvatar = m_avatarVector[i+1];
			}
		}
	}
}

void NewGameMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

void NewGameMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}

}