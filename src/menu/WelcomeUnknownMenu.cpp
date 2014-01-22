#include "WelcomeUnknownMenu.h"

namespace Symp {

void WelcomeUnknownMenu::init(){
	//Creation of a button
	button = new GuiComponent();
	m_guiComponentArray.push_back(button);
	m_pMenuManager->addGuiComponent(button, layer);
}

}