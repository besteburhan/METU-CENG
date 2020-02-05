#include "Camera.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>
using namespace std;
Camera::Camera(int id,                      // Id of the camera
               const char* imageName,       // Name of the output PPM file 
               const Vector3f& pos,         // Camera position
               const Vector3f& gaze,        // Camera gaze direction
               const Vector3f& up,          // Camera up direction
               const ImagePlane& imgPlane)  // Image plane parameters
               :id(id), pos(pos), imgPlane(imgPlane)
{
    Vector3f u = up;
    Vector3f g = gaze;
    this->up = u.normalize();
    this->gaze = g.normalize();
    memcpy(this->imageName, imageName, strlen(imageName)+1);
    Vector3f vxw = this->up.cross_product((this->gaze)*(-1));
    this->vxw = vxw.normalize();

    this->m = this->pos + ( (this->gaze) * (this->imgPlane.distance));
    this->q = m + this->vxw * this->imgPlane.left +  this->up * this->imgPlane.top ;


}

/* Takes coordinate of an image pixel as row and col, and
 * returns the ray going through that pixel. 
 */
Ray Camera::getPrimaryRay(int row, int col) const
{
    float s_u = ((this->imgPlane.right - this->imgPlane.left)/this->imgPlane.nx)*(col+0.5);
    float s_v = ((this->imgPlane.top - this->imgPlane.bottom)/this->imgPlane.ny)*(row+0.5);
    Vector3f direction = this->q + (this->vxw * s_u  ) - ( this->up * s_v  ) - this->pos;
    direction = direction.normalize();
	Ray ray(this->pos, direction);
    return  ray;
}

