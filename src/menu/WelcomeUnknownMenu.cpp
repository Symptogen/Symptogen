#include "WelcomeUnknownMenu.h"
#include "Button.h"

namespace Symp {

WelcomeUnknownMenu::WelcomeUnknownMenu(MenuManager* pMenuManager) 
	: State(pMenuManager)
{
	//Temporary !
	//Creation of a button
	Button* button = new Button("fuckingButton", "../assets/dino.png");
	button->setPosition(500.f, 0, 10.f);
	button->setScale(0.2, 0.2);
	m_guiComponentArray.push_back(button);
	m_pMenuManager->addGuiComponent(button, 0);

}

WelcomeUnknownMenu::~WelcomeUnknownMenu()
{
}

void WelcomeUnknownMenu::handleMouseClic(int mouseX, int mouseY){
	std::cout << "Clic on " << mouseX << " and " << mouseY <<std::endl;
	for (unsigned int i = 0; i < m_guiComponentArray.size(); ++i ){
		if (m_guiComponentArray[i]->isTargetedByMouse(mouseX, mouseY)){
			
			if(m_guiComponentArray[i]->getName() == "fuckingButton"){
				std::cout << "found ! " << std::endl;
			}

		}
	}
}

void WelcomeUnknownMenu::keyDownPressed(){
	std::cout << "Key down pressed" <<std::endl;
}

void WelcomeUnknownMenu::keyUpPressed(){
	std::cout << "Key up pressed" <<std::endl;
}

}