#ifndef _H_SYMPTOGEN_INPUT_INPUT_MANAGER_H_
#define _H_SYMPTOGEN_INPUT_INPUT_MANAGER_H_

#include <Indie.h>
#include <IND_Input.h>

#include "Render.h"

namespace Symp{

/**
	Facade of IND_Input.
*/
class InputManager {
public:
	InputManager(Render* pRender);
	~InputManager();

	void update();
	bool quit();

	bool onKeyPress(IND_Key pKey);
	bool onKeyRelease(IND_Key pKey);
	bool isKeyPressed(IND_Key pKey);
	bool isKeyPressed(IND_Key pKey, unsigned long pTime);

	int getMouseX();
	int getMouseY();

	bool isMouseMotion();
	bool onMouseButtonPress(IND_MouseButton pMouseButton);
	bool onMouseButtonRelease(IND_MouseButton pMouseButton);
	bool isMouseButtonPressed(IND_MouseButton pMouseButton);
	bool isMouseButtonPressed(IND_MouseButton pMouseButton, unsigned long pTime);

	IND_Input* getIND_Input(){return m_pInput;}

private:
	IND_Input* m_pInput;
};

}

#endif //_H_SYMPTOGEN_INPUT_INPUT_MANAGER_H_