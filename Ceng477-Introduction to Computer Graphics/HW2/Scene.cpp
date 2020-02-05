#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <cmath>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"

using namespace tinyxml2;
using namespace std;
# define _USE_MATH_DEFINES

/*
	Transformations, clipping, culling, rasterization are done here.
	You can define helper functions inside Scene class implementation.
*/
void Scene::forwardRenderingPipeline(Camera *camera)
{
	vector<Matrix4*> transed_rotations;
	vector<Matrix4*> transed_translations;
	vector<Matrix4*> transed_scalings;
	for(int j=0; j<this->rotations.size(); j++) transed_rotations.push_back(nullptr);
	for(int j=0; j<this->translations.size(); j++) transed_translations.push_back(nullptr);
	for(int j=0; j<this->scalings.size(); j++) transed_scalings.push_back(nullptr);

	for(int i=0; i<this->models.size(); i++){
		Model *model = this->models[i];
		for(int j=0; j< model->triangles.size(); j++){
		    Triangle triangle = model->triangles[j];
			vector <Vec4> transformed_triangle;
			for(int v=0; v<3; v++){
				int vert_id = triangle.vertexIds[v];
				Vec3 *inp_vert = this->vertices[vert_id-1];
				Vec4 result_vert = Vec4(inp_vert->x, inp_vert->y, inp_vert->z, 1, inp_vert->colorId);
				result_vert = get_modeling_transformations(result_vert, model, transed_rotations, transed_translations, transed_scalings);
				Matrix4 cam_trans = get_camera_transformation(camera);
				result_vert = multiplyMatrixWithVec4(cam_trans, result_vert);
				Matrix4 orthog_transformation = get_orthographic(camera);
				if(this->projectionType == 1){ // perspective
					Matrix4 perspective_transformation = get_perspective_matrix(camera);
					result_vert = multiplyMatrixWithVec4(orthog_transformation,multiplyMatrixWithVec4(perspective_transformation, result_vert));
				}
				
				else // othograhic
					result_vert = multiplyMatrixWithVec4(orthog_transformation, result_vert);
				// perspective dividing;
				result_vert = Vec4(result_vert.x/result_vert.t, result_vert.y/result_vert.t, result_vert.z/result_vert.t, 1, result_vert.colorId);
				transformed_triangle.push_back(result_vert);
			}
			
			Vec3 normal_vector = get_normal_vector(transformed_triangle);
			if(!(this->cullingEnabled && !is_front(camera, transformed_triangle, normal_vector))){
				if(model->type == 0){ // wireframe -> clipping + edge interpolation
					//clipping(camera, transformed_triangle)
				    clipping_plotting(camera, transformed_triangle[0], transformed_triangle[1]);
				    clipping_plotting(camera, transformed_triangle[1], transformed_triangle[2]);
				    clipping_plotting(camera, transformed_triangle[2], transformed_triangle[0]);

				}
				else{ // solid -> triangle rasterization
					transformed_triangle = viewport_transformation(camera, transformed_triangle);
					drawing_triangle(camera, transformed_triangle);
				}
			}
		}
	}
}

Vec4 Scene::get_modeling_transformations(Vec4 result_vert, Model *model, vector<Matrix4*> transed_rotations, vector<Matrix4*> transed_translations,	vector<Matrix4*> transed_scalings){
	
	for(int type_index=0; type_index < model->transformationTypes.size(); type_index++){
		int trans_id = model->transformationIds[type_index];
		if(model->transformationTypes[type_index]=='r'){
			if(!transed_rotations[trans_id-1]){  // create if not
				transed_rotations[trans_id-1] = new Matrix4();
				*transed_rotations[trans_id-1] = *get_rotation(this->rotations[trans_id-1]);
			}
			result_vert = multiplyMatrixWithVec4(*(transed_rotations[trans_id-1]), result_vert);
		}
		else if(model->transformationTypes[type_index]=='t' ){
			if(!transed_translations[trans_id-1]){  // create if not
				transed_translations[trans_id-1] = new Matrix4();
				*transed_translations[trans_id-1] = *get_translation(this->translations[trans_id-1]);
			}
			result_vert = multiplyMatrixWithVec4(*transed_translations[trans_id-1], result_vert);
		}
		else if(model->transformationTypes[type_index]=='s'){
			if(!transed_scalings[trans_id-1]){  // create if not
				transed_scalings[trans_id-1] = new Matrix4();
				*transed_scalings[trans_id-1] = *get_scaling(this->scalings[trans_id-1]);
			}
			result_vert = multiplyMatrixWithVec4(*transed_scalings[trans_id-1], result_vert);
		}
	}
	return result_vert;
}

double Scene::f(int x, int y, int x_first, int y_first, int x_second, int y_second){
	return x*(y_first-y_second) + y*(x_second-x_first) + x_first*y_second - y_first*x_second;
}
void Scene::drawing_triangle(Camera * camera, vector <Vec4> transformed_triangle){

	double x_min = min(min(transformed_triangle[0].x, transformed_triangle[1].x), transformed_triangle[2].x);
	double x_max = max(max(transformed_triangle[0].x, transformed_triangle[1].x), transformed_triangle[2].x);
	double y_min = min(min(transformed_triangle[0].y, transformed_triangle[1].y), transformed_triangle[2].y);
	double y_max = max(max(transformed_triangle[0].y, transformed_triangle[1].y), transformed_triangle[2].y);
	int x_0 = round(transformed_triangle[0].x);
	int y_0 = round(transformed_triangle[0].y);
	int x_1 = round(transformed_triangle[1].x);
	int y_1 = round(transformed_triangle[1].y);
	int x_2 = round(transformed_triangle[2].x);
	int y_2 = round(transformed_triangle[2].y);
	cout << x_min << y_min << endl;
	for(double y = max(0.0, round(y_min)); y < camera->verRes and y <= round(y_max); y++){
		for(double x = max(0.0, round(x_min)); x< camera->horRes and x <= round(x_max) ; x++){

			double alpha = f(x, y, x_1, y_1, x_2, y_2);
			alpha = alpha / f(x_0, y_0, x_1, y_1, x_2, y_2);
			double beta = f(x, y, x_2, y_2, x_0, y_0);
			beta = beta / f(x_1, y_1, x_2, y_2, x_0, y_0);
			double gama = f(x, y, x_0, y_0, x_1, y_1);
			gama = gama / f(x_2, y_2, x_0, y_0, x_1, y_1);
			if( alpha >= 0 and beta >= 0 and gama >= 0){
				Color *v_0 = new Color();
				*v_0 = *this->colorsOfVertices[transformed_triangle[0].colorId-1];
				Color *v_1 = new Color();
				*v_1 = *this->colorsOfVertices[transformed_triangle[1].colorId-1];
				Color *v_2 = new Color();
				*v_2 = *this->colorsOfVertices[transformed_triangle[2].colorId-1];
				Color color_0 = Color(alpha*v_0->r, alpha*v_0->g, alpha*v_0->b);
				Color color_1 = Color(beta*v_1->r, beta*v_1->g, beta*v_1->b);
				Color color_2 = Color(gama*v_2->r, gama*v_2->g, gama*v_2->b);
				this->image[x][y] = Color(color_0.r + color_1.r +color_2.r,
										  color_0.g + color_1.g + color_2.g,
										  color_0.b + color_1.b + color_2.b);
			}
		}
	}

}

vector <Vec4> Scene::viewport_transformation(Camera* camera, vector <Vec4> transformed_triangle ){
	Matrix4 viewport_transf = getIdentityMatrix();
	viewport_transf.val[0][0] = double(camera->horRes)/2; viewport_transf.val[0][3] = double(camera->horRes-1)/2;
	viewport_transf.val[1][1] = double(camera->verRes)/2; viewport_transf.val[1][3] = double(camera->verRes-1)/2;
	viewport_transf.val[2][2] = 0.5; viewport_transf.val[2][3] = 0.5;
	transformed_triangle[0] = multiplyMatrixWithVec4(viewport_transf, transformed_triangle[0]);
	transformed_triangle[1] = multiplyMatrixWithVec4(viewport_transf, transformed_triangle[1]);
	transformed_triangle[2] = multiplyMatrixWithVec4(viewport_transf, transformed_triangle[2]);
	return transformed_triangle;

}


Vec4* Scene::clipping_plotting(Camera* camera, Vec4 first_vertex, Vec4 second_vertex){
	double *t_e = new double();
	double *t_l = new double();
	*t_e = 0;
	*t_l = 1;
	double d_x = second_vertex.x-first_vertex.x;
	double d_y = second_vertex.y-first_vertex.y;
	double d_z = second_vertex.z-first_vertex.z;

	if (is_visible(d_x, -1-first_vertex.x, t_e, t_l) && is_visible(-d_x, first_vertex.x - 1, t_e, t_l) && is_visible(d_y, -1-first_vertex.y, t_e, t_l)
			&& is_visible(-d_y, first_vertex.y - 1, t_e, t_l) && is_visible(d_z, -1-first_vertex.z, t_e, t_l) && is_visible(-d_z, first_vertex.z - 1, t_e, t_l)){
		if (*t_l < 1){
            second_vertex.x = first_vertex.x+d_x*(*t_l);
            second_vertex.y = first_vertex.y+d_y*(*t_l);
            second_vertex.z = first_vertex.z+d_z*(*t_l);
		}
		if (*t_e > 0){
            first_vertex.x = first_vertex.x+d_x*(*t_e);
            first_vertex.y = first_vertex.y+d_y*(*t_e);
            first_vertex.z = first_vertex.z+d_z*(*t_e);
		}

		Matrix4 viewport_transf = getIdentityMatrix();
		viewport_transf.val[0][0] = double(camera->horRes)/2; viewport_transf.val[0][3] = double(camera->horRes-1)/2;
		viewport_transf.val[1][1] = double(camera->verRes)/2; viewport_transf.val[1][3] = double(camera->verRes-1)/2;
        viewport_transf.val[2][2] = 0.5; viewport_transf.val[2][3] = 0.5;
        first_vertex = multiplyMatrixWithVec4(viewport_transf, first_vertex);
        second_vertex = multiplyMatrixWithVec4(viewport_transf, second_vertex);

		line_rasterization(camera, first_vertex, second_vertex);

	}	
		
}
void Scene::line_rasterization(Camera* camera, Vec4 first_vertex, Vec4 second_vertex){
	double slope = (second_vertex.y-first_vertex.y)/(second_vertex.x-first_vertex.x);
	double d_x = second_vertex.x-first_vertex.x;
	double d_y = second_vertex.y-first_vertex.y;
	if ((slope > 0) && (slope < 1)){
		if( second_vertex.x < first_vertex.x ){ 
			Vec4 temp = first_vertex;
			first_vertex = second_vertex;
			second_vertex = temp;
			d_x = second_vertex.x-first_vertex.x;
			d_y = second_vertex.y-first_vertex.y;
		}
		double d = -2*d_y+(d_x);
		int y = max(0.0, round(first_vertex.y));
		Color c = *this->colorsOfVertices[first_vertex.colorId-1];
		Color dc = *this->colorsOfVertices[second_vertex.colorId-1];
		dc.r = (dc.r-c.r)/(d_x);
		dc.g = (dc.g-c.g)/(d_x);
		dc.b = (dc.b-c.b)/(d_x);
		for (int x=max(0.0, round(first_vertex.x)); y < camera->verRes && x < camera->horRes && x < round(second_vertex.x); x++){
			this->image[x][y] = c;
			if (d < 0){
				y++;
				d = d + 2*(-d_y + d_x);
			}
			else {
				d = d - 2*d_y;
			}
			c.r += dc.r;
			c.g += dc.g;
			c.b += dc.b;
		}

	}
	else if (slope > 1){
		if( second_vertex.x < first_vertex.x ){ 
			Vec4 temp = first_vertex;
			first_vertex = second_vertex;
			second_vertex = temp;
			d_x = second_vertex.x-first_vertex.x;
			d_y = second_vertex.y-first_vertex.y;
		}
		double d = 2*d_x-(d_y);
		int x=max(0.0, round(first_vertex.x));
		Color c = *this->colorsOfVertices[first_vertex.colorId-1];
		Color dc = *this->colorsOfVertices[second_vertex.colorId-1];
		dc.r = (dc.r-c.r)/(d_y);
		dc.g = (dc.g-c.g)/(d_y);
		dc.b = (dc.b-c.b)/(d_y);

		for (int y=max(0.0, round(first_vertex.y)); x < camera->horRes && y<camera->verRes && y<round(second_vertex.y); y++){
			this->image[x][y] = c;
			if (d>0){
				x++;
				d = d + -2*d_y + 2*d_x;
			}
			else {
				d = d + 2*d_x;
			}
			c.r += dc.r;
			c.g += dc.g;
			c.b += dc.b;
		}

	}
	else if(slope<0 && slope>-1){
		if(second_vertex.x > first_vertex.x){
			Vec4 temp = first_vertex;
			first_vertex = second_vertex;
			second_vertex = temp;
			d_x = second_vertex.x-first_vertex.x;
			d_y = second_vertex.y-first_vertex.y;
		}
		double d = 2*d_y+(d_x);
		int y=max(0.0, round(first_vertex.y));
		Color c = *this->colorsOfVertices[first_vertex.colorId-1];
		Color dc = *this->colorsOfVertices[second_vertex.colorId-1];
		dc.r = (dc.r-c.r)/(-d_x); 
		dc.g = (dc.g-c.g)/(-d_x);
		dc.b = (dc.b-c.b)/(-d_x);
		int x;

		for ( x = min(double(camera->horRes-1), round(first_vertex.x)); y < camera->verRes && x >= 0  && x >= round(second_vertex.x); x--){
			this->image[x][y] = c;
			if (d > 0){
				y++;
				d = d + 2*d_y + 2*d_x;
			}
			else {
				d = d + 2*d_y;
			}
			c.r += dc.r;
			c.g += dc.g;
			c.b += dc.b;
		}
	}
	else if(slope < -1){
		if( second_vertex.x > first_vertex.x ){ 
			Vec4 temp = first_vertex;
			first_vertex = second_vertex;
			second_vertex = temp;
			d_x = second_vertex.x-first_vertex.x;
			d_y = second_vertex.y-first_vertex.y;
		}
		double d = d_y + 2*d_x;
		int x=round(first_vertex.x);
		Color c = *this->colorsOfVertices[first_vertex.colorId-1];
		Color dc = *this->colorsOfVertices[second_vertex.colorId-1];
		dc.r = (dc.r-c.r)/(d_y);
		dc.g = (dc.g-c.g)/(d_y);
		dc.b = (dc.b-c.b)/(d_y);

		for (int y=max(0.0, round(first_vertex.y)); y < camera->verRes && y<=round(second_vertex.y) && x>=0; y++){
			this->image[x][y] = c;
			if (d<0){
				x--;
				d = d + 2*d_y + 2*d_x ;
			}
			else {
				d = d + 2*d_x;
			}
			c.r += dc.r;
			c.g += dc.g;
			c.b += dc.b;
		}
	}
}	


bool Scene::is_visible(double den, double num, double *t_e, double *t_l){
	double t = num/den;
	if(den>0){
		if(t > *t_l)
			return false;
		if(t > *t_e)
			*(t_e) = t;
	}
	else if(den < 0){
		if(t < *t_e)
			return false;
		if(t < *t_l)
			*(t_l) = t;
	}
	else if(num > 0)
		return false;
	return true;
}

bool Scene::is_front(Camera *camera, vector<Vec4> transformed_triangle, Vec3 normal){
	Vec3 cam_to_triangle = Vec3(transformed_triangle[0].x-camera->v.x, transformed_triangle[0].y-camera->v.y, transformed_triangle[0].z-camera->v.z,1);
	double dot_prod = dotProductVec3(normal, cam_to_triangle);

	return dot_prod<0 ? true : false;
}
Vec3 Scene::get_normal_vector(vector<Vec4> triangle){
	Vec4 v_0 = triangle[0];
	Vec4 v_1 = triangle[1];
	Vec4 v_2 = triangle[2];
	Vec3 b_a = Vec3(v_1.x - v_0.x, v_1.y - v_0.y, v_1.z - v_0.z, 1);
	Vec3 c_a = Vec3(v_2.x - v_0.x, v_2.y - v_0.y, v_2.z - v_0.z, 1);
	Vec3 normal;
	normal = crossProductVec3(c_a, b_a);
	normal = normalizeVec3(normal);
	return normal;


}

Matrix4 Scene::get_perspective_matrix(Camera *camera){
	Matrix4 pers = getIdentityMatrix();
	pers.val[0][0] = camera->near; pers.val[1][1] = camera->near; pers.val[2][2] = camera->far + camera->near; 
	pers.val[2][3] = camera->far*camera->near; pers.val[3][2] = -1; pers.val[3][3] = 0;
	return pers;
}
Matrix4 Scene::get_orthographic(Camera *camera){
	Matrix4 orth = getIdentityMatrix();
	orth.val[0][0] = 2/(camera->right - camera->left); 
	orth.val[1][1] = 2/(camera->top - camera->bottom);
	orth.val[2][2] = -2/(camera->far - camera->near);  
	orth.val[0][3] = -(camera->right + camera->left)/(camera->right - camera->left); 
	orth.val[1][3] = -(camera->top + camera->bottom)/(camera->top - camera->bottom);
	orth.val[2][3] = -(camera->far + camera->near)/(camera->far - camera->near);
	return orth;
}
Matrix4 Scene::get_camera_transformation(Camera *camera){
	Matrix4 translated_cam = get_translation(camera->pos);
	Matrix4 rotated_cam = getIdentityMatrix();
	rotated_cam.val[0][0] = camera->u.x; rotated_cam.val[0][1] = camera->u.y; rotated_cam.val[0][2] = camera->u.z;
	rotated_cam.val[1][0] = camera->v.x; rotated_cam.val[1][1] = camera->v.y; rotated_cam.val[1][2] = camera->v.z;
	rotated_cam.val[2][0] = camera->w.x; rotated_cam.val[2][1] = camera->w.y; rotated_cam.val[2][2] = camera->w.z;
	Matrix4 res = multiplyMatrixWithMatrix(rotated_cam, translated_cam);
	return res;


}
Matrix4* Scene::get_rotation(Rotation *rot){
	Matrix4 M = getIdentityMatrix();
	Matrix4 M_invers = getIdentityMatrix();
	Matrix4 *result = new Matrix4();
	Vec3 u = Vec3(rot->ux, rot->uy, rot->uz, 1);
	u = normalizeVec3(u);
	Vec3 v = Vec3();
	double min_value = min(min(u.x, u.y), u.z);
	if(u.x == min_value){
		v.x = 0; v.y = -u.z; v.z = u.y;
	}
	else if(u.y == min_value){
		v.y = 0; v.x = -u.z; v.z = u.x;
	}
	else if(u.z == min_value){
		v.z = 0; v.y = -u.x; v.x = u.y;
	}
	
	v = normalizeVec3(v);
	Vec3 w = crossProductVec3(u, v);
	w = normalizeVec3(w);
	M_invers.val[0][0] = u.x ; M_invers.val[1][0] = u.y ; M_invers.val[2][0] = u.z ; 
	M_invers.val[0][1] = v.x ; M_invers.val[1][1] = v.y ; M_invers.val[2][1] = v.z ; 
	M_invers.val[0][2] = w.x ; M_invers.val[1][2] = w.y ; M_invers.val[2][2] = w.z ; 
	
	M.val[0][0] = u.x ; M.val[1][0] = v.x ; M.val[2][0] = w.x ; 
	M.val[0][1] = u.y ; M.val[1][1] = v.y ; M.val[2][1] = w.y ; 
	M.val[0][2] = u.z ; M.val[1][2] = v.z ; M.val[2][2] = w.z ; 
	double sin_theta = sin(rot->angle*M_PI/180.0);
	double cos_theta = cos(rot->angle*M_PI/180.0);
	Matrix4 Rx = getIdentityMatrix();
	Rx.val[1][1] = cos_theta;	Rx.val[1][2] = -sin_theta;
	Rx.val[2][1] = sin_theta;	Rx.val[2][2] = cos_theta;

	*result = multiplyMatrixWithMatrix(M_invers, multiplyMatrixWithMatrix(Rx, M));
	
	return result;

}
Matrix4* Scene::get_translation(Translation *trans){

	Matrix4 *result = new Matrix4();
	*result = getIdentityMatrix();
	result->val[0][3] = trans->tx;
	result->val[1][3] = trans->ty;
	result->val[2][3] = trans->tz;
	return result;

}
Matrix4 Scene::get_translation(Vec3 trans){

	Matrix4 result = getIdentityMatrix();
	result.val[0][3] = -trans.x;
	result.val[1][3] = -trans.y;
	result.val[2][3] = -trans.z;
	return result;

}
Matrix4* Scene::get_scaling(Scaling *scale){

	Matrix4 *result = new Matrix4();
	*result = getIdentityMatrix();
	result->val[0][0] = scale->sx;
	result->val[1][1] = scale->sy;
	result->val[2][2] = scale->sz;
	return result;
}
/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *pElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	// read background color
	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	pElement = pRoot->FirstChildElement("Culling");
	if (pElement != NULL)
		pElement->QueryBoolText(&cullingEnabled);

	// read projection type
	pElement = pRoot->FirstChildElement("ProjectionType");
	if (pElement != NULL)
		pElement->QueryIntText(&projectionType);

	// read cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while (pCamera != NULL)
	{
		Camera *cam = new Camera();

		pCamera->QueryIntAttribute("id", &cam->cameraId);

		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

		cam->gaze = normalizeVec3(cam->gaze);
		cam->u = crossProductVec3(cam->gaze, cam->v);
		cam->u = normalizeVec3(cam->u);

		cam->w = inverseVec3(cam->gaze);
		cam->v = crossProductVec3(cam->u, cam->gaze);
		cam->v = normalizeVec3(cam->v);

		camElement = pCamera->FirstChildElement("ImagePlane");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &cam->left, &cam->right, &cam->bottom, &cam->top,
			   &cam->near, &cam->far, &cam->horRes, &cam->verRes);

		camElement = pCamera->FirstChildElement("OutputName");
		str = camElement->GetText();
		cam->outputFileName = string(str);

		cameras.push_back(cam);

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// read vertices
	pElement = pRoot->FirstChildElement("Vertices");
	XMLElement *pVertex = pElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (pVertex != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = pVertex->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = pVertex->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		vertices.push_back(vertex);
		colorsOfVertices.push_back(color);

		pVertex = pVertex->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	pElement = pRoot->FirstChildElement("Translations");
	XMLElement *pTranslation = pElement->FirstChildElement("Translation");
	while (pTranslation != NULL)
	{
		Translation *translation = new Translation();

		pTranslation->QueryIntAttribute("id", &translation->translationId);

		str = pTranslation->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		translations.push_back(translation);

		pTranslation = pTranslation->NextSiblingElement("Translation");
	}

	// read scalings
	pElement = pRoot->FirstChildElement("Scalings");
	XMLElement *pScaling = pElement->FirstChildElement("Scaling");
	while (pScaling != NULL)
	{
		Scaling *scaling = new Scaling();

		pScaling->QueryIntAttribute("id", &scaling->scalingId);
		str = pScaling->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		scalings.push_back(scaling);

		pScaling = pScaling->NextSiblingElement("Scaling");
	}

	// read rotations
	pElement = pRoot->FirstChildElement("Rotations");
	XMLElement *pRotation = pElement->FirstChildElement("Rotation");
	while (pRotation != NULL)
	{
		Rotation *rotation = new Rotation();

		pRotation->QueryIntAttribute("id", &rotation->rotationId);
		str = pRotation->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		rotations.push_back(rotation);

		pRotation = pRotation->NextSiblingElement("Rotation");
	}

	// read models
	pElement = pRoot->FirstChildElement("Models");

	XMLElement *pModel = pElement->FirstChildElement("Model");
	XMLElement *modelElement;
	while (pModel != NULL)
	{
		Model *model = new Model();

		pModel->QueryIntAttribute("id", &model->modelId);
		pModel->QueryIntAttribute("type", &model->type);

		// read model transformations
		XMLElement *pTransformations = pModel->FirstChildElement("Transformations");
		XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

		pTransformations->QueryIntAttribute("count", &model->numberOfTransformations);

		while (pTransformation != NULL)
		{
			char transformationType;
			int transformationId;

			str = pTransformation->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			model->transformationTypes.push_back(transformationType);
			model->transformationIds.push_back(transformationId);

			pTransformation = pTransformation->NextSiblingElement("Transformation");
		}

		// read model triangles
		XMLElement *pTriangles = pModel->FirstChildElement("Triangles");
		XMLElement *pTriangle = pTriangles->FirstChildElement("Triangle");

		pTriangles->QueryIntAttribute("count", &model->numberOfTriangles);

		while (pTriangle != NULL)
		{
			int v1, v2, v3;

			str = pTriangle->GetText();
			sscanf(str, "%d %d %d", &v1, &v2, &v3);

			model->triangles.push_back(Triangle(v1, v2, v3));

			pTriangle = pTriangle->NextSiblingElement("Triangle");
		}

		models.push_back(model);

		pModel = pModel->NextSiblingElement("Model");
	}
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
			}

			this->image.push_back(rowOfColors);
		}
	}
	// if image is filled before, just change color rgb values with the background color
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				this->image[i][j].r = this->backgroundColor.r;
				this->image[i][j].g = this->backgroundColor.g;
				this->image[i][j].b = this->backgroundColor.b;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFileName.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFileName << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
	string command;

	// call command on Ubuntu
	if (osType == 1)
	{
		command = "convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// call command on Windows
	else if (osType == 2)
	{
		command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// default action - don't do conversion
	else
	{
	}
}