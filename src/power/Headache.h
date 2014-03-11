#ifndef _H_SYMPTOGEN_POWER_HEADACHE_H_
#define _H_SYMPTOGEN_POWER_HEADACHE_H_

#include "Power.h"
#include <ctime>

namespace Symp {

/**
*
*/
class Headache : public Power {

public:

	/**
	*	@brief Headache class constructor
	*/
	Headache(){
		m_uiLastExecution = time(NULL);
		m_iRotationAngle = 90;
		m_iInterpolateAngle = 0;
		m_uiStep = 0;
	}

	/**
	*	@brief Headache class destructor
	*/
	~Headache(){}

	/**
	* @brief Implementation of the Power abstract class 
	* If the Headache power is activated, this method will be called at each turn of the main loop. This method will make the character's temperature change, or not.
	*/
	void execute();

	/**
	* @brief Force the execution of the Headache.
	* This test function is used by the developers to easely trigger the power of hot Headache (temperature > 0) or cold fever (temperature < 0).
	*/
	void forceExecution();

	/**
	*	Getters
	*/
	inline unsigned int getLastExecution() const {return m_uiLastExecution;}
	inline int getRotationAngle() const {return m_iRotationAngle;}
	inline int getMaxRotationAgnle() const {return m_iMaxRotationAngle;}
	inline int getInterpolateAngle() const {return m_iInterpolateAngle;}
	inline unsigned int getTimeToTriggerRandomHeadache() const {return m_uiTimeToTriggerRandomHeadache;}

	/**
	*	Setters
	*/
	inline void setRotationAngle(int rotationAngle){m_iRotationAngle = rotationAngle;}
	inline void setMaxRotationAngle(int maxRotationAngle){m_iMaxRotationAngle = maxRotationAngle;}
	inline void setTimeToTriggerRandomHeadache(unsigned int timeToTriggerRandomHeadache){m_uiTimeToTriggerRandomHeadache = timeToTriggerRandomHeadache;}

private:
	unsigned int 	m_uiLastExecution; /**< tTime in milliseconds since the last rotation */
	int 			m_iRotationAngle;  /**< The rotation angle */
	int 			m_iMaxRotationAngle;
	int 			m_iInterpolateAngle;
	unsigned int 	m_uiStep;
	unsigned int 	m_uiTimeToTriggerRandomHeadache; /**< tThe time after which the random rotation can occure. See #execute() */

};

}

#endif // _H_SYMPTOGEN_POWER_HEADACHE_H_