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
	* @brief Implementation of the Power abstract class 
	* If the fever power is activated, this method will be called at each turn of the main loop. This method will make the character's temperature change, or not.
	*/
	void execute();

	/**
	* @brief Force the execution of the fever.
	* This test function is used by the developers to easely trigger the power of hot fever (temperature > 0) or cold fever (temperature < 0).
	*/
	void forceExecution();

	/**
	*	Getters
	*/
	inline float 		getCurrentTemperature() const { return m_fCurrentTemperature;}
	inline const int 	getMaxTemperature() const { return m_iMaxTemperature;}
	inline const int 	getMinTemperature() const { return m_iMinTemperature;}
	inline int 			getHotRange() const { return m_uiHotRange;}
	inline int 			getColdRange() const { return m_uiColdRange;}
	inline bool			isInHotRange() const {return (m_fCurrentTemperature > m_uiHotRange) ? true : false;}
	inline bool			isInColdRange() const {return (m_fCurrentTemperature < m_uiColdRange) ? true : false;}
	inline float 		getTemperatureVariation() const { return m_fTemperatureVariation;}
	inline bool 		isInHotZone() const { return m_isInHotZone;}
	inline bool 		isInColdZone() const { return m_isInColdZone;}

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
	float m_fCurrentTemperature;

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