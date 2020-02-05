#ifndef _DEFS_H_
#define _DEFS_H_

#include <iostream>
#include <math.h>
using namespace std;
class Scene;



/* 3 dimensional vector holding floating point numbers.
Used for both coordinates and color. 
Use x, y, z for coordinate computations, and use r, g, b for color computations. 
Note that you do not have to use this one if you use any vector computation library like Eigen. */
typedef struct Vector3f
{
	union 
	{
		float x;
		float r;
	};
	union
	{
		float y;
		float g;
	};
	union
	{
		float z;
		float b;
	};
    Vector3f& operator =(const Vector3f& a)
    {
        this->x = a.x;
        this->y = a.y;
        this->z = a.z;
        return *this;
    }
    Vector3f operator +(const Vector3f& a) const
    {
        Vector3f temp;
        temp.x = this->x + a.x;
        temp.y = this->y + a.y;
        temp.z = this->z + a.z;
        return temp;
    }
    Vector3f operator -(const Vector3f& a) const
    {
        Vector3f temp;
        temp.x = this->x - a.x;
        temp.y = this->y - a.y;
        temp.z = this->z - a.z;
        return temp;
    }
    Vector3f operator *(float a) const
    {
        Vector3f temp;
        temp.x = this->x * a;
        temp.y = this->y * a;
        temp.z = this->z * a;
        return temp;
    }
    Vector3f operator *(Vector3f a) const
    {
        Vector3f temp;
        temp.x = this->x * a.x;
        temp.y = this->y * a.y;
        temp.z = this->z * a.z;
        return temp;
    }
    Vector3f operator /(float a) const
    {
        Vector3f temp;
        temp.x = this->x / a;
        temp.y = this->y / a;
        temp.z = this->z / a;
        return temp;
    }

    float dot_product(const Vector3f& a)
    {
        return this->x*a.x + this->y*a.y + this->z*a.z;
    }
    Vector3f cross_product(const Vector3f& a)
    {
        Vector3f cross;
        cross.x = this->y * a.z - this->z * a.y;
        cross.y = this->z * a.x - this->x * a.z;
        cross.z = this->x * a.y - this->y * a.x;
        return cross;
    }
    Vector3f normalize(){
        Vector3f temp = *this;
        return temp / sqrt(pow(temp.x,2)+pow(temp.y,2)+pow(temp.z,2));
    }

} Vector3f;



/* Structure to hold return value from ray intersection routine.
This should hold information related to the intersection point,
for example, coordinate of the intersection point, surface normal at the intersection point etc.
Think about the variables you will need for this purpose and declare them here inside of this structure. */
typedef struct ReturnVal
{
    Vector3f intersection_point;
    Vector3f surface_normal;
    float t;
    bool is_intersect=true;
} ReturnVal;

//
// The global variable through which you can access the scene data
//
extern Scene* pScene;

#endif
