#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vec3.h"
#include <string>

using namespace std;

class Camera
{

public:
    int cameraId;
    Vec3 pos;
    Vec3 gaze;
    Vec3 u;
    Vec3 v;
    Vec3 w;
    double left, right, bottom, top;
    double near;
    double far;
    int horRes;
    int verRes;
    string outputFileName;

    Camera();

    Camera(int cameraId,
           Vec3 pos, Vec3 gaze,
           Vec3 u, Vec3 v, Vec3 w,
           double left, double right, double bottom, double top,
           double near, double far,
           int horRes, int verRes,
           string outputFileName);
    

    Camera(const Camera &other);

    friend std::ostream& operator<<(std::ostream& os, const Camera& c);
};

#endif