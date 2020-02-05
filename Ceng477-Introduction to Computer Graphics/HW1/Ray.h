#ifndef _RAY_H_
#define _RAY_H_

#include "defs.h"

// Class for Ray object. You will implement the methods
class Ray
{
public:
	Vector3f origin;	// Origin of the ray
	Vector3f direction;	// Direction of the ray

	Ray();	// Constuctor
	Ray(const Vector3f& origin, const Vector3f& direction);	// Constuctor

	Vector3f getPoint(float t) const;       // Return the point along the ray at ray parameter t
	float gett(const Vector3f & p) const;   // Return the t parameter at point p along the ray

private:

	// Write any other stuff here. 
};

#endif

