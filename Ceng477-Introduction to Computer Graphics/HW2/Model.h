#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "Triangle.h"
#include <iostream>

using namespace std;

class Model
{

public:
    int modelId;
    int type;
    int numberOfTransformations;
    vector<int> transformationIds;
    vector<char> transformationTypes;
    int numberOfTriangles;
    vector<Triangle> triangles;

    Model();
    Model(int modelId, int type, int numberOfTransformations,
          vector<int> transformationIds,
          vector<char> transformationTypes,
          int numberOfTriangles,
          vector<Triangle> triangles);

    friend ostream &operator<<(ostream &os, const Model &m);
};

#endif