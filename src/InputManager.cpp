#include "InputManager.h"

namespace Symp{

InputManager::InputManager(Render* pRender){
	m_pInput = new IND_Input();
	m_pInput->init(pRender->getIND_Render());
}

InputManager::~InputManager(){
	m_pInput->end();
	DISPOSE(m_pInput);
}

void InputManager::update(){
	m_pInput->update();
}

bool InputManager::quit(){
	return m_pInput->quit();
}

bool InputManager::onKeyPress(IND_Key pKey){
	return m_pInput->onKeyPress(pKey);
}

bool InputManager::onKeyRelease(IND_Key pKey){
	return m_pInput->onKeyRelease(pKey);
}

bool InputManager::isKeyPressed(IND_Key pKey){
	return m_pInput->isKeyPressed(pKey);
}

bool InputManager::isKeyPressed(IND_Key pKey, unsigned long pTime){
	return m_pInput->isKeyPressed(pKey, pTime);
}

bool InputManager::onMouseButtonPress(IND_MouseButton pMouseButton){
	return m_pInput->onMouseButtonPress(pMouseButton);
}

bool InputManager::onMouseButtonRelease(IND_MouseButton pMouseButton){
	return m_pInput->onMouseButtonRelease(pMouseButton);
}

bool InputManager::isMouseButtonPressed(IND_MouseButton pMouseButton){
	return m_pInput->isMouseButtonPressed(pMouseButton);
}

bool InputManager::isMouseButtonPressed(IND_MouseButton pMouseButton, unsigned long pTime){
	return m_pInput->isMouseButtonPressed(pMouseButton, pTime);
}

int InputManager::getMouseX(){
	return m_pInput->getMouseX();
}

int InputManager::getMouseY(){
	return m_pInput->getMouseY();
}

}