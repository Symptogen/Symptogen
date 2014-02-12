#ifndef _H_SYMPTOGEN_MENU_STATE_H_
#define _H_SYMPTOGEN_MENU_STATE_H_

#include "GuiComponent.h"

/** @namespace Symp */
namespace Symp {
	
class MenuManager;
/**
* @class Interface State. This class is part of the State Machine pattern that implements the menus management. Each menu 
* inherits State class to be managed by the MenuManager.
* @see MenuManager
*/
class State {
public:
	/**
	* @brief State class constructor 
	* Responsible for the initialization of the only class private attribute.
	* @param pMenuManager is the reference to the #MenuManager class
	* @see State::init()
	* @see MenuManager
	*/
	State(MenuManager* pMenuManager){m_pMenuManager = pMenuManager;}
	/**
	* @brief virtual destructor
	* @see State
	* @see MenuManager
	*/
	virtual ~State(){}
	/**
	* @brief virtual function that initialize the graphical components of a menu 
	* @see State
	* @see MenuManager
	*/
	virtual void init() = 0;
	/**
	* @brief virtual function that forward the mouse clic event to the menu logic
	* The events are retrieved by the #GameManager and transmitted to the game or to the menu following the current
	* state of the application. 
	* @param mouseX the x coordinate of the mouse clic position
	* @param mouseY the y coordinate of the mouse clic position
	* @see State
	* @see MenuManager
	* @see GameManager::updateMenu()
	*/
	virtual void handleMouseClic(int mouseX, int mouseY) = 0;
	/**
	* @brief virtual function that forward the key down event to the menu logic
	* The events are retrieved by the #GameManager and transmitted to the game or to the menu following the current
	* state of the application. 
	* @see State
	* @see MenuManager
	* @see GameManager::updateMenu()
	*/
	virtual void keyDownPressed() = 0;
	/**
	* @brief virtual function that forward the key up event to the menu logic
	* The events are retrieved by the #GameManager and transmitted to the game or to the menu following the current
	* state of the application. 
	* @see State
	* @see MenuManager
	* @see GameManager::updateMenu()
	*/
	virtual void keyUpPressed() = 0;

protected:
	MenuManager* m_pMenuManager; /**< the reference to the #MenuManager */

};

}

#endif //_H_SYMPTOGEN_MENU_STATE_H_