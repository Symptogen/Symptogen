#ifndef _H_SYMPTOGEN_INPUT_INPUT_MANAGER_H_
#define _H_SYMPTOGEN_INPUT_INPUT_MANAGER_H_

#include <Indie.h>
#include <IND_Input.h>

#include "Render.h"

/**
	Facade of IND_Input.
*/
class InputManager {
public:
	InputManager();
	void init(Render* pRender);
	void end();

	void update();
	bool quit();

	bool onKeyPress(IND_Key pKey);
	bool onKeyRelease(IND_Key pKey);
	bool isKeyPressed(IND_Key pKey);
	bool isKeyPressed(IND_Key pKey, unsigned long pTime);

	IND_Input* getIND_Input(){return m_pInput;}

private:
	IND_Input* m_pInput;
};

#endif //_H_SYMPTOGEN_INPUT_INPUT_MANAGER_H_