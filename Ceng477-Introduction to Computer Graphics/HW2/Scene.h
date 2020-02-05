#ifndef _SCENE_H_
#define _SCENE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix4.h"

using namespace std;

class Scene
{
public:
	Color backgroundColor;
	bool cullingEnabled;
	int projectionType;

	vector< vector<Color> > image;
	vector< Camera* > cameras;
	vector< Vec3* > vertices;
	vector< Color* > colorsOfVertices;
	vector< Scaling* > scalings;
	vector< Rotation* > rotations;
	vector< Translation* > translations;
	vector< Model* > models;

	Scene(const char *xmlPath);

	void initializeImage(Camera* camera);
	void forwardRenderingPipeline(Camera* camera);
	int makeBetweenZeroAnd255(double value);
	void writeImageToPPMFile(Camera* camera);
	void convertPPMToPNG(string ppmFileName, int osType);

	void drawing_triangle(Camera *camera, vector <Vec4> transformed_triangle);
	Matrix4 *get_scaling(Scaling *scale);
	Matrix4 get_translation(Vec3 trans);
	Matrix4 *get_translation(Translation *trans);
	Matrix4 *get_rotation(Rotation *rot);
	Matrix4 get_camera_transformation(Camera *camera);
	Matrix4 get_orthographic(Camera *camera);
	Matrix4 get_perspective_matrix(Camera *camera);
	Vec3 get_normal_vector(vector<Vec4> triangle);
	Vec3 transform_normal(Vec3 old_normal, Matrix4 trans_matrix);
	bool is_front(Camera *camera, vector<Vec4> transformed_triangle, Vec3 normal);
	bool is_visible(double den, double num, double *t_e, double *t_l);
	Vec4* clipping_plotting(Camera* camera, Vec4 first_vertex, Vec4 second_vertex);
	vector <Vec4> viewport_transformation(Camera* camera, vector <Vec4> transformed_triangle );
	void line_rasterization(Camera* camera, Vec4 first_vertex, Vec4 second_vertex);
	Vec4 get_modeling_transformations(Vec4 result_vert, Model *model, vector<Matrix4*> transed_rotations, vector<Matrix4*> transed_translations,	vector<Matrix4*> transed_scalings);
	double f(int x, int y, int x_first, int y_first, int x_second, int y_second);
};

#endif
