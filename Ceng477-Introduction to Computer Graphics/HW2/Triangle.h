#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__


class Triangle
{
public:
    int vertexIds[3];

    Triangle();
    Triangle(int vid1, int vid2, int vid3);
    Triangle(const Triangle &other);

    int getFirstVertexId();
    int getSecondVertexId();
    int getThirdVertexId();

    void setFirstVertexId(int vid);
    void setSecondVertexId(int vid);
    void setThirdVertexId(int vid);
};


#endif