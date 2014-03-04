#ifndef _H_SYMPTOGEN_POWER_POWER_H_
#define _H_SYMPTOGEN_POWER_POWER_H_

#include <Indie.h>

/** @namespace Symp */
namespace Symp {

/**
* @class Power class. Power is an abstract class with a pure virtual method named execute().
* There are  three powers inherited from this class : #Sneeze, #Fever and #Headache. Each one of them has to implement the virtual execute() method.
* @see Sneeze
* @see Fever
* @see Headache
*/
class Power {

public:

	/**
	* @brief Power class constructor
	*/
	Power(){
		m_bIsActivated = false;
		m_pTimer = new IND_Timer();
	}

	/**
	* @brief Power class destructor
	*/
	virtual ~Power() {}

	/**
	* @brief Virtual method 
	* This method has to be implemented by the inherited class #Sneeze, #Fever and #Headache
	*/
	virtual void execute() {}

	/**
	* @brief Get the state of the current power.
	* Warning : some powers have subtilities (example of the Sneeze, with the warning and the real sneeze, both when the power is activated).
	*/
	bool isActivated() const { return m_bIsActivated;}

protected:

	/**
	* @brief Activate the current power
	*/
	void activate() { 
		m_bIsActivated = true; 
	}

	/**
	* @brief Deactivate the current power
	*/
	void deactivate() { 
		m_bIsActivated = false;
	}

	IND_Timer* 	m_pTimer; /**< This timer is used to know if the animation is playing */

private:
	bool		m_bIsActivated; /**< Indicate if the power is activated now */
};


}

#endif