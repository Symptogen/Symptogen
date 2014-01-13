#ifndef _H_SYMPTOGEN_RENDER_WINDOW_H_
#define _H_SYMPTOGEN_RENDER_WINDOW_H_

#include <Indie.h>
#include <IND_Window.h>

namespace Symp{

/**
	Facade of IND_Window.
*/
class Window {
public:
	Window();

	void setWindow(IND_Window* pWindow);
	void setCursor(bool pEnable);

	IND_Window* getIND_Window(){return m_pWindow;}

private:
	IND_Window* m_pWindow;
};

}

#endif //_H_SYMPTOGEN_RENDER_WINDOW_H_