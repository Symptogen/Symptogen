#ifndef _H_SYMPTOGEN_POWER_POWER_H_
#define _H_SYMPTOGEN_POWER_POWER_H_

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
	Power(){}

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
	* @brief Activate the current power
	* A power is activate or deactivate in the scope of one level. For one level, if m_bIsActivated is true, it means that this level can trigger this type of power.
	*/
	void activate() { m_bIsActivated = true; }

	/**
	* @brief Deactivate the current power
	* A power is activate or deactivate in the scope of one level. For one level, if m_bIsActivated is true, it means that this level can trigger this type of power.
	*/
	void deactivate() { m_bIsActivated = false; }

private:
	bool m_bIsActivated; /**< tIndicates if the power can be trigger in the current levelt */

};


}

#endif