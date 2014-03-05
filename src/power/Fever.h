#ifndef _H_SYMPTOGEN_POWER_FEVER_H_
#define _H_SYMPTOGEN_POWER_FEVER_H_

#include "Power.h"
#include <ctime>

namespace Symp {

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
	inline bool 		isInHotZone() const { return m_isInHotZone; }
	inline bool 		isInColdZone() const { return m_isInColdZone; }

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
	inline void isInHotZone(bool yes) {
		m_isInHotZone = yes;
		if(yes) m_isInColdZone = false;
	}
	inline void isInColdZone(bool yes) {
		m_isInColdZone = yes;
		if(yes) m_isInHotZone = false;
	}

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
	*	Variation’s step of the temperature. Can be positive or negative.
	*/
	float m_fTemperatureVariation;

	/**
	*	If the dino is in a hot zone.
	*/
	bool m_isInHotZone;

	/**
	*	If the dino is in a cold zone.
	*/
	bool m_isInColdZone;

	/**
	*	The factor affected to the temperature variation when we are in a particular zone.
	*/
	int m_iZoneVariationFactor;

};

}

#endif // _H_SYMPTOGEN_POWER_FEVER_H_