/*****************************************************************************************
 * File: IND_Vector2.h
 * Desc: 2D Vector implementation
 *****************************************************************************************/

/*********************************** The zlib License ************************************
 *
 * Copyright (c) 2013 Indielib-crossplatform Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 *
 *****************************************************************************************/

#ifndef _IND_VECTOR2_
#define _IND_VECTOR2_

#include "IndiePlatforms.h"

//Libraries dependencies
#include <math.h>
#include <assert.h>

//Classes dependencies

/**
 @defgroup IND_Vector2 IND_Vector2
 @ingroup Math
 Encapsulates a 2d vector and it's operations
 */

/**@{*/

/**
 Encapsulates a 2d vector and it's operations. Makes easier to work with vector types and encapsultes common operations
 like sum, multiplication, dot product...
 */
class LIB_EXP IND_Vector2 {
public:    
	//----- CONSTRUCTORS/DESTRUCTORS -----
	
    /**
     Default constructor, initializes elements to 0
     */
	IND_Vector2(): _x(0), _y(0) {
	}
	/**
     Designated constructor
     @param newx,newy Values of components
     */
	explicit IND_Vector2(float newx, float newy): _x(newx), _y(newy)
	{}
	~IND_Vector2()
	{}
	
    /** @name Operators overloading */
    /**@{*/
	
    /**
     Copies element by element all components to the vector
     @param newvector Another vector
     @return Ourselves, modified
     */
	IND_Vector2 &operator = (const IND_Vector2 &newvector) {
		_x = newvector._x;
		_y = newvector._y;

		return *this;
	}
    
	/**
     Performs element by element comparison
     @param newvector Another vector
     @return true if all components equal, false otherwise
     */
	inline bool operator == (const IND_Vector2 &newvector) const {
		return (_x == newvector._x && _y == newvector._y);
	}
    
    /**
     Performs element by element comparison
     @param newvector Another vector
     @return true if all components equal, false otherwise
     */
	inline bool operator != (const IND_Vector2 &newvector) const {
		return (_x != newvector._x || _y != newvector._y);
	}
    
	/**
     Performs sum of element by element.
     Means x+x, y+y etc
     @param newvector Another vector
     @return Ourselves, modified
     */
	inline IND_Vector2 operator + (const IND_Vector2 &newvector) const {
		return IND_Vector2(
		          _x + newvector._x,
		           _y + newvector._y);
	}
    
	/**
     Performs sum to ourselves. Doesn't do anything.
     @return Ourselves, modified
     */
	inline const IND_Vector2 &operator + () const {
		return *this;
	}
    
	/**
     Performs sum of element by element.
     Means x+x, y+y etc
     @param tosum Another vector
     @return Ourselves, modified
     */
	inline IND_Vector2 &operator += (const IND_Vector2 &tosum) {
		_x += tosum._x;
		_y += tosum._y;

		return *this;
	}
    
	/**
     Sums the same ammount to every element.
     @param offset The value to add to every coordinate
     @return Ourselves, modified
     */
	inline IND_Vector2 &operator += (const float offset) {
		_x += offset;
		_y += offset;

		return *this;
	}
    
	/**
     Performs a substraction of element by element.
     Means x+x, y+y etc
     @param newvector Another vector
     @return Ourselves, modified
     */
	inline IND_Vector2 operator - (const IND_Vector2 &newvector) const {
		return IND_Vector2(
		           _x - newvector._x,
		           _y - newvector._y);
	}
    
	/**
     Performs a substraction of element by element.
     Means x+x, y+y etc
     @param torest Another vector
     @return Ourselves, modified
     */
	inline IND_Vector2 &operator -= (const IND_Vector2 &torest) {
		_x -= torest._x;
		_y -= torest._y;

		return *this;
	}
    
	/**
     Subtracts the same ammount to every element.
     @param offset The value to substract to every coordinate
     @return Ourselves, modified
     */
	inline IND_Vector2 &operator -= (const float offset) {
		_x -= offset;
		_y -= offset;

		return *this;
	}
    
	/**
     Negates the vector, element by element.
     @return Ourselves, modified
     */
	inline IND_Vector2 operator - () const {
		return IND_Vector2(-_x, -_y);
	}
    
	/**
     Multiply by a scalar every element.
     @param scalar The value to multiply to every coordinate
     @return Ourselves, modified
     */
	inline IND_Vector2 operator * (const float scalar) const {
		return IND_Vector2(
		           _x * scalar,
		           _y * scalar);
	}
    
	/**
     Multiplies corresponding elements of vectors.
     @param rhs Another vector
     @return Ourselves, modified
     */
	inline IND_Vector2 operator * (const IND_Vector2 &rhs) const {
		return IND_Vector2(
		           _x * rhs._x,
		           _y * rhs._y);
	}
    
	/**
     Multiply by a scalar every element.
     @param scalar The value to multiply to every coordinate
     @return Ourselves, modified
     */
	inline IND_Vector2 &operator *= (const float scalar) {
		_x *= scalar;
		_y *= scalar;
		return *this;
	}
    
	/**
     Multiplies corresponding elements of vectors.
     @param tomultiply Another vector
     @return Ourselves, modified
     */
	inline  IND_Vector2 &operator *= (const IND_Vector2 &tomultiply) {
		_x *= tomultiply._x;
		_y *= tomultiply._y;

		return *this;
	}
    
	/**
     Divices by a scalar every element.
     If value is 0, will break!
     @param scalar The value to divide to every coordinate
     @return Ourselves, modified
     */
	inline IND_Vector2 operator / (const float scalar) const {
		assert(scalar != 0.0f);
		float inv = 1.0f / scalar;
		return IND_Vector2(
		           _x * inv,
		           _y * inv);
	}
    
	/**
     Divides corresponding elements of vectors.
     @param divideto Another vector
     @return Ourselves, modified
     */
	inline IND_Vector2 operator / (const IND_Vector2 &divideto) const {
		assert(divideto._x != 0 && divideto._y != 0);
		return IND_Vector2(
		           _x / divideto._x,
		           _y / divideto._y);
	}
    
	/**
     Divices by a scalar every element.
     If value is 0, will break!
     @param scalar The value to divide to every coordinate
     @return Ourselves, modified
     */
	inline IND_Vector2 &operator /= (const float scalar) {
		assert(scalar != 0.0f);

		float inv = 1.0f / scalar;
		_x *= inv;
		_y *= inv;
		return *this;
	}
    
	/**
     Divides corresponding elements of vectors.
     @param todivide Another vector
     @return Ourselves, modified
     */
	inline IND_Vector2 &operator /= (const IND_Vector2 &todivide) {
       assert(todivide._x != 0 && todivide._y != 0);
		_x /= todivide._x;
		_y /= todivide._y;
		return *this;
	}

	/**@}*/

    /** @name Vector utilities */
    
	/**
     Computes the length of the vector.
     
     Calculation is sqrt((_x * _x) + (_y * _y))
     
     @return Length of vector value
     */
	inline float length() {
		return(sqrt((_x * _x) + (_y * _y)));
	}

    /**
     Computes dot product of two vectors.
     @return Value of dot product
     */
	inline float dotProduct(const IND_Vector2 &vec) const {
		return ((_x * vec._x) + (_y * vec._y));
	}

    /**
     Computes cross product of two vectors.
     @param vec The vector to make cross product with
     @return Vector result of cross product
     */
	inline float crossProduct(const IND_Vector2 &vec) const {
		return ((_x * vec._y) - (_y * vec._x));
	}

	/**
     Normalizes the vector.
     
     Operation computes length of vector, and divides every vector component by it.
     @return Length of vector (before being normalized)
     */
	inline float normalise() {
		float l = length();

		// Protection on floating point calcs
		if (l > 1e-08) {
			float invlength = 1.0f / l;
			_x *= invlength;
			_y *= invlength;
		}

		return l;
	}
    
	/**
     Computes the distance to another vector
     
     Calculates difference of components, (x2-x1,y2-y1) and gets the length of it.
     
     @param to The vector to where distance is to be computed (that is x2) from (x2-x1,y2-y1)
     @return Length of vector differenct between this and the other one
     */
	inline float distance(const IND_Vector2 &to) const {
		return (*this - to).length();
	}

	/**
     Computes the angle between two vectors.
     Angle between vectors:
     alpha = arcos((vec1*vec2)/|vec1|*|vec2|) in a general way
     alpha = arcos(vec1*vec2) in unit vectors
     
     @param dest Vector  in
     @return value of angle. This will be signed angle
     */
	inline float angleBetweenUnitVecs(const IND_Vector2 &dest) {
		float dotp = dotProduct(dest);

		return (acos(dotp));
	}
    
    /** @name Public Values */
    /**@{*/
	float _x, _y;          ///< Values of vector
    /**@}*/
    
    /**@}*/
};

/**@}*/

#endif   //_IND_VECTOR2_
