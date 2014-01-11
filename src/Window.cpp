#include "Window.h"

Window::Window(){
	m_pWindow = new IND_Window();
}

void Window::setWindow(IND_Window* pWindow){
	m_pWindow = pWindow;
}

void Window::setCursor(bool pEnable){
	m_pWindow->cursor(pEnable);
}