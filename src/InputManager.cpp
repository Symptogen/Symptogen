#include "InputManager.h"

InputManager::InputManager(){
	m_pInput = new IND_Input();
}

void InputManager::init(Render* pRender){
	m_pInput->init(pRender->getIND_Render());
}

void InputManager::end(){
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