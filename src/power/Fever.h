#ifndef _H_SYMPTOGEN_POWER_FEVER_H_
#define _H_SYMPTOGEN_POWER_FEVER_H_

#include "Power.h"
#include <ctime>

namespace Symp {

/**
* This enum is used to define the index of the thermometer (depend on the fever power).
*/
enum ThermometerStep {
	Step1,
	Step2,
	Step3,
	Step4,
	Step5,
	Step6,
	Step7,
	Step8
};

/**
*
*/
class Fever : public Power {

public:

	/**
	*	@brief Fever class constructor
	*/
	Fever();

	/**
	*	@brief Fever class destructor
	*/
	~Fever();

	/**
	*	@brief Implementation of the Power abstract class 
	* If the fever power is activated, this method will be called at each turn of the main loop. This method will make the character's temperature change, or not.
	*/
	void execute();

	/**
	*	Getters
	*/
	inline int 			getCurrentTemperature() const { return m_iCurrentTemperature;}
	inline const int 	getMaxTemperature() const { return m_iMaxTemperature;}
	inline const int 	getMinTemperature() const { return m_iMinTemperature;}
	inline int 			getHotRange() const { return m_uiHotRange;}
	inline int 			getColdRange() const { return m_uiColdRange;}
	inline float 		getTemperatureVariation() const { return m_fTemperatureVariation;}

	/**
	*	Returns value : [0, 7]
	* 	0 => die by cold
	*	1 => activate cold power
	*	[2, 5] => neutral zone
	*	6 => activate hot power
	*	7 => die by hot
	*/
	size_t 				getThermometerStep() const;

	/**
	*	Setters
	*/
	inline void setTemperatureVariation(float temperatureVariation) { m_fTemperatureVariation = temperatureVariation;}

private:
	/**
	*	Current temperature of the character.
	*/
	int m_iCurrentTemperature;

	/**
	*	Temperature above which the character dies because of the heat.
	*/
	const int m_iMaxTemperature;

	/**
	*	Temperature below which the character dies because of the cold
	*/
	const int m_iMinTemperature;

	/**
	*	Threshold from which the heat power is activated
	*/
	int m_uiHotRange;

	/**
	*	Threshold from which the cold power is activated.
	*/
	int m_uiColdRange;

	/**
	*	Variation’s step of the temperature.
	*/
	float m_fTemperatureVariation;

};

}

#endif // _H_SYMPTOGEN_POWER_FEVER_H_