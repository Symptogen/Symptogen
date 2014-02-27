#ifndef _H_SYMPTOGEN_INPUT_INPUT_MANAGER_H_
#define _H_SYMPTOGEN_INPUT_INPUT_MANAGER_H_

#include <Indie.h>
#include <IND_Input.h>

#include "../util/Singleton.h"
#include "../render/Render.h"

namespace Symp{

/**
	Facade of IND_Input.
*/
class InputManager : public Singleton<InputManager> {

	// Friend to use private constructor/destructor
	friend class Singleton<InputManager>;

public:

	static void initRender(Render* pRender);

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
	static IND_Input* m_pInput;

	/** 
	*	Private constructor (because it is a singleton)
	*	@see InputManager()
	*	@see ~InputManager()
	*/
	InputManager();

	/**
	*	Private destructor (because it is a singleton)
	*	@see InputManager()
	*	@see ~InputManager()
	*/
	~InputManager();
};

}

#endif //_H_SYMPTOGEN_INPUT_INPUT_MANAGER_H_