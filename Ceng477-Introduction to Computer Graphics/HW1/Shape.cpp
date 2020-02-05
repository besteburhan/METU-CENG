#include "Shape.h"
#include "Scene.h"
#include <cstdio>
#include <math.h>
#include <limits>

Shape::Shape(void)
{
}

Shape::Shape(int id, int matIndex)
    : id(id), matIndex(matIndex)
{
}

Sphere::Sphere(void)
{}


Sphere::Sphere(int id, int matIndex, int cIndex, float R)
    : Shape(id, matIndex), cIndex(cIndex), R(R)
{

}


ReturnVal Sphere::intersect(const Ray & ray) const
{

    ReturnVal returnVal;
	Vector3f center_camera = ray.origin;
	Vector3f ray_direction = ray.direction;
	Vector3f center_sphere = pScene->vertices[this->cIndex-1];
	float radius_sphere = this->R;
	float discriminant;
	float t=-1;
    Vector3f distance_centers = center_camera-center_sphere;
        discriminant = pow((ray_direction.dot_product(distance_centers)),2)
                - (ray_direction.dot_product(ray_direction)) * ( distance_centers.dot_product(distance_centers) - pow(radius_sphere,2) );

    if(discriminant<0 ){
        returnVal.is_intersect = false;
    }
    else{
        discriminant = sqrt(discriminant);
        float dd = ray_direction.dot_product(ray_direction);
        float d_o = ray_direction.dot_product(distance_centers);
        float t_1 =  (- d_o + discriminant)/ dd;
        float t_2 =  (- d_o - discriminant)/ dd;
        float testEps = pScene->intTestEps;
        if(pScene->intTestEps==0)
            testEps = 1e-6;
        if(t_1 < testEps && t_2 < testEps) {
            returnVal.is_intersect = false;
            return returnVal;
        }
        else if(t_1 < testEps || t_2 < testEps)
            t = max(t_1,t_2);
        else
            t = min(t_1,t_2);

        returnVal.intersection_point = center_camera + ray_direction*t;
        returnVal.surface_normal = ( returnVal.intersection_point - center_sphere )/radius_sphere;
        returnVal.t = t;

    }
    return returnVal;


}

Triangle::Triangle(void)
{}


Triangle::Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index)
    : Shape(id, matIndex), p1Index(p1Index),p2Index(p2Index),p3Index(p3Index)
{

}
float Triangle::determinant(Vector3f col1, Vector3f col2, Vector3f col3) const{
    return col1.x * ( (col2.y * col3.z) - (col3.y * col2.z) ) + col1.y * ((col3.x * col2.z) - (col2.x *col3.z)) + col1.z * ((col2.x * col3.y) - (col2.y * col3.x));
}


ReturnVal Triangle::intersect(const Ray & ray) const
{

    ReturnVal returnVal;
    Vector3f center_camera = ray.origin;
    Vector3f ray_direction = ray.direction;
    Vector3f p1 = pScene->vertices[this->p1Index-1];
    Vector3f p2 = pScene->vertices[this->p2Index-1];
    Vector3f p3 = pScene->vertices[this->p3Index-1];
    Vector3f p1_p2 = p1-p2;
    Vector3f p1_p3 = p1-p3;
    Vector3f p1_center_camera = p1 - center_camera;

    float determinant_A =  this->determinant(p1_p2, p1_p3, ray_direction);
    float beta = (this->determinant(p1_center_camera, p1_p3, ray_direction))/determinant_A;
    float gama = (this->determinant(p1_p2, p1_center_camera, ray_direction))/determinant_A;
    float t = (this->determinant(p1_p2, p1_p3, p1_center_camera))/determinant_A;
    float testEps = pScene->intTestEps;
        if(pScene->intTestEps==0)
            testEps = 1e-6;
    if(  t >= testEps && beta+gama <= 1 && beta >= 0 &&  gama >= 0){
        Vector3f cross_edges = (p1_p2 * (-1)).cross_product(p1_p3 * (-1));
        returnVal.intersection_point = center_camera + ray_direction*t;
        returnVal.surface_normal = cross_edges.normalize();
        returnVal.t = t;
    }
    else{
        returnVal.is_intersect = false;
    }
    return returnVal;
}

Mesh::Mesh()
{}


Mesh::Mesh(int id, int matIndex, const vector<Triangle>& faces)
    : Shape(id, matIndex), matIndex(matIndex), faces(faces)
{

}


ReturnVal Mesh::intersect(const Ray & ray) const
{
	ReturnVal returnVal;
    Vector3f center_camera = ray.origin;
    Vector3f ray_direction = ray.direction;
    returnVal.is_intersect = false;
    returnVal.t = numeric_limits<double>::max();
    for (int i = 0; i < this->faces.size(); ++i){
        Triangle triangle = this->faces[i];
        ReturnVal tempReturnValue = triangle.intersect(ray);
        if(tempReturnValue.is_intersect && tempReturnValue.t < returnVal.t)
            returnVal = tempReturnValue;
	}
    return returnVal;
}

