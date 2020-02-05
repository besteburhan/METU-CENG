#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Ray.h"
#include "defs.h"

// Structure for holding variables related to the image plane
typedef struct ImagePlane
{
	float left;     // "u" coordinate of the left edge
	float right;    // "u" coordinate of the right edge
	float bottom;   // "v" coordinate of the bottom edge
	float top;      // "v" coordinate of the top edge
    float distance; // distance to the camera (always positive)
    int nx;         // number of pixel columns
    int ny;         // number of pixel rows
} ImagePlane;

class Camera
{
public:
  char imageName[32];
  int id;
  ImagePlane imgPlane;     // Image plane

	Camera(int id,                      // Id of the camera
           const char* imageName,       // Name of the output PPM file 
           const Vector3f& pos,         // Camera position
           const Vector3f& gaze,        // Camera gaze direction
           const Vector3f& up,          // Camera up direction
           const ImagePlane& imgPlane); // Image plane parameters

    // Computes the primary ray through pixel (row, col)
	Ray getPrimaryRay(int row, int col) const;

private:
    Vector3f pos;
    Vector3f gaze;
    Vector3f up;
    Vector3f vxw;
    Vector3f m;
    Vector3f q;


    //
	// You can add member functions and variables here
    //
};

#endif
