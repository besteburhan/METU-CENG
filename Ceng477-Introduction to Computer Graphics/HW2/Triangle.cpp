#include "Triangle.h"

Triangle::Triangle() {
    this->vertexIds[0] = -1;
    this->vertexIds[1] = -1;
    this->vertexIds[2] = -1;
}

Triangle::Triangle(int vid1, int vid2, int vid3)
{
    this->vertexIds[0] = vid1;
    this->vertexIds[1] = vid2;
    this->vertexIds[2] = vid3;
}

Triangle::Triangle(const Triangle &other)
{
    this->vertexIds[0] = other.vertexIds[0];
    this->vertexIds[1] = other.vertexIds[1];
    this->vertexIds[2] = other.vertexIds[2];
}

// getters
int Triangle::getFirstVertexId()
{
    return this->vertexIds[0];
}

int Triangle::getSecondVertexId()
{
    return this->vertexIds[1];
}

int Triangle::getThirdVertexId()
{
    return this->vertexIds[2];
}

// setters
void Triangle::setFirstVertexId(int vid)
{
    this->vertexIds[0] = vid;
}

void Triangle::setSecondVertexId(int vid)
{
    this->vertexIds[1] = vid;
}

void Triangle::setThirdVertexId(int vid)
{
    this->vertexIds[2] = vid;
}