#ifndef _H_SYMPTOGEN_POWER_SNEEZE_H_
#define _H_SYMPTOGEN_POWER_SNEEZE_H_

#include "Power.h"
#include <ctime>

namespace Symp {

class Sneeze : public Power {

public:

	/**
	* @brief Sneeze class constructor
	*/
	Sneeze() {
		m_uiLastExecution = time(NULL);
	}

	/**
	* @brief Sneeze class destructor
	*/
	~Sneeze(){}

	/**
	* @brief Implementation of the Power abstract class 
	* If the sneeze power is activated, this method will be called at each turn of the main loop. This method will make the character sneeze, or not.
	* When the time since the last sneeze exceeds #m_uiTimeToTriggerRandomSneeze, the random sneeze is activated and the more time elapses, the more the probability of sneezing increases.
	*/
	void execute();

	/**
	* @brief Force the execution of the sneeze, regardless of m_uiLastExecution.
	* This method is called when we want to force the execution of the sneeze, regardless of the randomm calculation. It is used to trigger the sneeze when the Dino encounters a flower.
	*/
	void forceExecution();

	/**
	* @brief Update the timer of the power
	* This method is called when the power animation begin, it is used to know when the animation begin and when it end.
	*/
	void updatePowerTimer();


	/**
	*	Getters
	*/
	inline unsigned int getLastExecution() const { return m_uiLastExecution; }
	inline unsigned int getRepulsionStrength() const { return m_uiRepulsionStrength; }
	inline unsigned int getTimeToTriggerRandomSneeze() const { return m_uiTimeToTriggerRandomSneeze; }

	/**
	*	Setters
	*/
	inline void setRepulsionStrength(unsigned int rs) { m_uiRepulsionStrength = rs; }
	inline void setTimeToTriggerRandomSneeze(unsigned int tttrs) { m_uiTimeToTriggerRandomSneeze = tttrs; }

private:


	unsigned int m_uiLastExecution; /**< tTime in milliseconds since the last random sneeze */
	unsigned int m_uiRepulsionStrength; /**< tValue of the repulsion strength (how much the character will be repulsed by the sneeze) */
	unsigned int m_uiTimeToTriggerRandomSneeze; /**< tThe time after which the random sneeze can occure. See #execute() */
};

}

#endif