#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Shape.h"
#include "tinyxml2.h"
#include <iostream>
#include <limits>
#include <math.h>
#include <typeinfo>
#include <string>
#include "Image.h"
#include <thread>

using namespace std;
using namespace tinyxml2;

/*
 * Must render the scene from each camera's viewpoint and create an image.
 * You can use the methods of the Image class to save the image as a PPM file.
 */
void Scene::renderScene(void)
{


    for(auto camera : this->cameras){
        Image image(camera->imgPlane.nx, camera->imgPlane.ny);
        int width = camera->imgPlane.nx;
        int height = camera->imgPlane.ny;// row sayısı
        int number_of_cores = thread::hardware_concurrency();
        if(number_of_cores==0 || height < number_of_cores){
            threadsFunction(camera, image, 0, height);
        }
        else{
            thread* threads = new thread[number_of_cores];
            int interval = height / number_of_cores;
            for(int i=0; i < number_of_cores; i++){
                int min_height = i * interval;
                int max_height;
                if(i==number_of_cores-1)
                    max_height = height;
                else
                    max_height = (i+1)*interval;
                threads[i] = thread(threadsFunction, camera, image, min_height, max_height);
            }
            for(int i = 0; i < number_of_cores; i++)
                threads[i].join();
            delete[] threads;
        }

        image.saveImage(camera->imageName);

    }
}
void Scene::threadsFunction(Camera* camera, Image image, int min_row, int max_row){
    for(int col=0; col < camera->imgPlane.nx ; col++){ // col -> point ->i
        for(int row=min_row; row < max_row ; row++){
            Vector3f pixelColor;
            Ray ray_camera_plane = camera->getPrimaryRay(row,col);
            pixelColor = pScene->calculatePixelColor(ray_camera_plane, pScene->maxRecursionDepth);
            if(int(pixelColor.x)>255)
                pixelColor.x = 255;
            if(int(pixelColor.y)>255)
                pixelColor.y = 255;
            if(int(pixelColor.z)>255)
                pixelColor.z = 255;
            Color color = {(unsigned char)(int(pixelColor.x)), (unsigned char)(int(pixelColor.y)), (unsigned char)(int(pixelColor.z))};
            image.setPixelValue(col,row,color);
        }
    }
}

Vector3f Scene::calculatePixelColor(const Ray& ray, int recursion_depth, const Shape* hit_object ){
    ReturnVal tempReturnVal;
    ReturnVal returnVal;
    Vector3f pixelColor;
    float t_min = numeric_limits<double>::max();
    Shape* object= nullptr;
    if(!hit_object){
        for(auto obj : this->objects){
            tempReturnVal = obj->intersect(ray);
            if(tempReturnVal.is_intersect && tempReturnVal.t < t_min){
                t_min = tempReturnVal.t;
                returnVal = tempReturnVal;
                object = obj;
            }
        }
    }
    if(object)
    {
        pixelColor = this->ambientLight * this->materials[object->matIndex-1]->ambientRef;
        Vector3f w_i;
        for(auto light : this->lights){
            w_i = (light->position - returnVal.intersection_point);
            float distance_w_i = sqrt(pow(w_i.x,2)+pow(w_i.y,2)+pow(w_i.z,2));
            w_i = w_i / distance_w_i;
            Ray shadow_ray(returnVal.intersection_point + w_i*this->shadowRayEps, w_i);
            bool is_shadowed=false;
            for(auto shadowed_obj: this->objects){
                ReturnVal tempReturnVal = shadowed_obj->intersect(shadow_ray);
                if(tempReturnVal.is_intersect){
                    Vector3f distance_shadowed_obj_vec = tempReturnVal.intersection_point - returnVal.intersection_point;
                    float distance_shadowed_obj =
                            sqrt(pow(distance_shadowed_obj_vec.x,2)+pow(distance_shadowed_obj_vec.y,2)+pow(distance_shadowed_obj_vec.z,2));
                    if( distance_shadowed_obj < distance_w_i){
                        is_shadowed = true;
                        break;
                    }
                }
            }

            if (!is_shadowed){
                Vector3f irradience = light->computeLightContribution(returnVal.intersection_point);
                Vector3f l_d = this->materials[object->matIndex-1]->diffuseRef * irradience;
                float cos_teta = max(w_i.dot_product(returnVal.surface_normal),float(0));
                l_d = l_d*cos_teta;
                Vector3f w_o = ray.direction*(-1);
                Vector3f h = (w_i + w_o);
                h = h.normalize();
                float cos_alpha = max(float(0), returnVal.surface_normal.dot_product(h));
                Vector3f l_s = this->materials[object->matIndex-1]->specularRef * irradience;
                l_s = l_s * pow(cos_alpha,this->materials[object->matIndex-1]->phongExp);
                pixelColor = (pixelColor + l_d) + l_s;
            }
        }
    }
    else{
        pixelColor = this->backgroundColor;
    }

    if(object && recursion_depth!=0 && this->materials[object->matIndex-1]->mirrorRef.x && this->materials[object->matIndex-1]->mirrorRef.y && this->materials[object->matIndex-1]->mirrorRef.z ){
        Vector3f w_r =
                ray.direction + (returnVal.surface_normal*2)*(returnVal.surface_normal.dot_product(ray.direction*(-1)));
        w_r = w_r.normalize();
        Ray reflection_ray(returnVal.intersection_point + w_r * this->shadowRayEps, w_r);
        Vector3f tempPixelColor = calculatePixelColor(reflection_ray,recursion_depth-1);
        pixelColor = pixelColor + this->materials[object->matIndex-1]->mirrorRef * tempPixelColor;
    }
    return pixelColor;
}

// Parses XML file. 
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLError eResult;
	XMLElement *pElement;

	maxRecursionDepth = 1;
	shadowRayEps = 0.001;

	eResult = xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	pElement = pRoot->FirstChildElement("MaxRecursionDepth");
	if(pElement != nullptr)
		pElement->QueryIntText(&maxRecursionDepth);

	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%f %f %f", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	pElement = pRoot->FirstChildElement("ShadowRayEpsilon");
	if(pElement != nullptr)
		pElement->QueryFloatText(&shadowRayEps);

	pElement = pRoot->FirstChildElement("IntersectionTestEpsilon");
	if(pElement != nullptr)
		eResult = pElement->QueryFloatText(&intTestEps);

	// Parse cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while(pCamera != nullptr)
	{
        int id;
        char imageName[64];
        Vector3f pos, gaze, up;
        ImagePlane imgPlane;

		eResult = pCamera->QueryIntAttribute("id", &id);
		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &pos.x, &pos.y, &pos.z);
		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &gaze.x, &gaze.y, &gaze.z);
		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &up.x, &up.y, &up.z);
		camElement = pCamera->FirstChildElement("NearPlane");
		str = camElement->GetText();
		sscanf(str, "%f %f %f %f", &imgPlane.left, &imgPlane.right, &imgPlane.bottom, &imgPlane.top);
		camElement = pCamera->FirstChildElement("NearDistance");
		eResult = camElement->QueryFloatText(&imgPlane.distance);
		camElement = pCamera->FirstChildElement("ImageResolution");	
		str = camElement->GetText();
		sscanf(str, "%d %d", &imgPlane.nx, &imgPlane.ny);
		camElement = pCamera->FirstChildElement("ImageName");
		str = camElement->GetText();
		strcpy(imageName, str);

		cameras.push_back(new Camera(id, imageName, pos, gaze, up, imgPlane));

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// Parse materals
	pElement = pRoot->FirstChildElement("Materials");
	XMLElement *pMaterial = pElement->FirstChildElement("Material");
	XMLElement *materialElement;
	while(pMaterial != nullptr)
	{
		materials.push_back(new Material());

		int curr = materials.size() - 1;
	
		eResult = pMaterial->QueryIntAttribute("id", &materials[curr]->id);
		materialElement = pMaterial->FirstChildElement("AmbientReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->ambientRef.r, &materials[curr]->ambientRef.g, &materials[curr]->ambientRef.b);
		materialElement = pMaterial->FirstChildElement("DiffuseReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->diffuseRef.r, &materials[curr]->diffuseRef.g, &materials[curr]->diffuseRef.b);
		materialElement = pMaterial->FirstChildElement("SpecularReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->specularRef.r, &materials[curr]->specularRef.g, &materials[curr]->specularRef.b);
		materialElement = pMaterial->FirstChildElement("MirrorReflectance");
		if(materialElement != nullptr)
		{
			str = materialElement->GetText();
			sscanf(str, "%f %f %f", &materials[curr]->mirrorRef.r, &materials[curr]->mirrorRef.g, &materials[curr]->mirrorRef.b);
		}
				else
		{
			materials[curr]->mirrorRef.r = 0.0;
			materials[curr]->mirrorRef.g = 0.0;
			materials[curr]->mirrorRef.b = 0.0;
		}
		materialElement = pMaterial->FirstChildElement("PhongExponent");
		if(materialElement != nullptr)
			materialElement->QueryIntText(&materials[curr]->phongExp);

		pMaterial = pMaterial->NextSiblingElement("Material");
	}

	// Parse vertex data
	pElement = pRoot->FirstChildElement("VertexData");
	int cursor = 0;
	Vector3f tmpPoint;
	str = pElement->GetText();
	while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
		cursor++;
	while(str[cursor] != '\0')
	{
		for(int cnt = 0 ; cnt < 3 ; cnt++)
		{
			if(cnt == 0)
				tmpPoint.x = atof(str + cursor);
			else if(cnt == 1)
				tmpPoint.y = atof(str + cursor);
			else
				tmpPoint.z = atof(str + cursor);
			while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
				cursor++; 
			while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
				cursor++;
		}
		vertices.push_back(tmpPoint);
	}

	// Parse objects
	pElement = pRoot->FirstChildElement("Objects");
	
	// Parse spheres
	XMLElement *pObject = pElement->FirstChildElement("Sphere");
	XMLElement *objElement;
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int cIndex;
		float R;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Center");
		eResult = objElement->QueryIntText(&cIndex);
		objElement = pObject->FirstChildElement("Radius");
		eResult = objElement->QueryFloatText(&R);

		objects.push_back(new Sphere(id, matIndex, cIndex, R));

		pObject = pObject->NextSiblingElement("Sphere");
	}

	// Parse triangles
	pObject = pElement->FirstChildElement("Triangle");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Indices");
		str = objElement->GetText();
		sscanf(str, "%d %d %d", &p1Index, &p2Index, &p3Index);

		objects.push_back(new Triangle(id, matIndex, p1Index, p2Index, p3Index));

		pObject = pObject->NextSiblingElement("Triangle");
	}

	// Parse meshes
	pObject = pElement->FirstChildElement("Mesh");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;
		int cursor = 0;
		int vertexOffset = 0;
		vector<Triangle> faces;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Faces");
		objElement->QueryIntAttribute("vertexOffset", &vertexOffset);
		str = objElement->GetText();
		while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
			cursor++;
		while(str[cursor] != '\0')
		{
			for(int cnt = 0 ; cnt < 3 ; cnt++)
			{
				if(cnt == 0)
					p1Index = atoi(str + cursor) + vertexOffset;
				else if(cnt == 1)
					p2Index = atoi(str + cursor) + vertexOffset;
				else
					p3Index = atoi(str + cursor) + vertexOffset;
				while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
					cursor++; 
				while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
					cursor++;
			}
			faces.push_back(*(new Triangle(-1, matIndex, p1Index, p2Index, p3Index)));
		}

		objects.push_back(new Mesh(id, matIndex, faces));

		pObject = pObject->NextSiblingElement("Mesh");
	}

	// Parse lights
	int id;
	Vector3f position;
	Vector3f intensity;
	pElement = pRoot->FirstChildElement("Lights");

	XMLElement *pLight = pElement->FirstChildElement("AmbientLight");
	XMLElement *lightElement;
	str = pLight->GetText();
	sscanf(str, "%f %f %f", &ambientLight.r, &ambientLight.g, &ambientLight.b);

	pLight = pElement->FirstChildElement("PointLight");
	while(pLight != nullptr)
	{
		eResult = pLight->QueryIntAttribute("id", &id);
		lightElement = pLight->FirstChildElement("Position");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &position.x, &position.y, &position.z);
		lightElement = pLight->FirstChildElement("Intensity");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &intensity.r, &intensity.g, &intensity.b);

		lights.push_back(new PointLight(position, intensity));

		pLight = pLight->NextSiblingElement("PointLight");
	}
}

