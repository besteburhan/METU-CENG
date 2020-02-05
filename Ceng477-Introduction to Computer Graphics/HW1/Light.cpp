#include "Light.h"
#include <math.h>
using namespace std;

/* Constructor. Implemented for you. */
PointLight::PointLight(const Vector3f & position, const Vector3f & intensity)
    : position(position), intensity(intensity)
{
}

// Compute the contribution of light at point p using the
// inverse square law formula
Vector3f PointLight::computeLightContribution(const Vector3f& p)
{
    Vector3f intensity = this->intensity;
    float distance = (pow(this->position.x-p.x,2)+pow(this->position.y-p.y,2)+pow(this->position.z-p.z,2));
    return intensity/distance;
}
