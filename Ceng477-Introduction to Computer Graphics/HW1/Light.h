#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "defs.h"

using namespace std;

// Class for point lights
class PointLight
{
public:	
    Vector3f position;	// Position of the point light

    PointLight(const Vector3f & position, const Vector3f & intensity);	// Constructor
    Vector3f computeLightContribution(const Vector3f& p); // Compute the contribution of light at point p

private:

    Vector3f intensity;	// Intensity of the point light
};


#endif
