#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <chrono>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include <jpeglib.h>
#include <sstream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
GLFWwindow * win = NULL;
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idHeightTexture;
int widthTexture;
int  heightTexture;
int widthImagePlane = 1000;
int heightImagePlane = 1000;
float heightFactor = 0.0f;
float cameraSpeed = 0.0;
GLint lightPosition;
glm::vec4 lp;

GLint sampler1;
GLint sampler2;
glm::vec3 cameraPos;
glm::vec3 cameraGaze;
glm::vec3 cameraUp;
glm::vec3 cameraU;
glm::mat4x4 viewMatrix;
glm::mat4 projectionMatrix;
float texOffset=0.0;


#pragma pack(push, 1)
struct VNC {
    float x;
    float y;
    float z;
    float nx;
    float ny;
    float nz;
    float xTexPos;
    float yTexPos;
};
#pragma pack(pop)
class Vec3i
    {
    public:
        int x, y, z;

    Vec3i()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    Vec3i(int x, int y, int z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vec3i(const Vec3i &other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
    }

    float getElementAt(int index)
    {
        switch (index)
        {
        case 0:
            return this->x;

        case 1:
            return this->y;

        case 2:
            return this->z;

        default:
            return this->z;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec3i& v) {
        
        os << std::fixed << "[" << v.x << ", " << v.y << ", " << v.z << "]";

        return os;
    }
};

using namespace std;
static void errorCallback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}
bool readDataFromFile(const string& fileName, string &data) {
    fstream myfile;

    myfile.open(fileName.c_str(), std::ios::in);

    if (myfile.is_open()) {
        string curLine;

        while (getline(myfile, curLine)){
            data += curLine;
            if (!myfile.eof())
                data += "\n";
        }

        myfile.close();
    }
    else
        return false;

    return true;
}
void initShaders() {

    idProgramShader = glCreateProgram();
    string shaderVertSource;
    string shaderFragSource;

    // initialize fragment
    readDataFromFile("sphere_shader.frag", shaderFragSource);
    GLint length = shaderFragSource.length();
    const GLchar* shader1 = (const GLchar*) shaderFragSource.c_str();

    idFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(idFragmentShader, 1, &shader1, &length);
    glCompileShader(idFragmentShader);

    char output1[1024] = {0};
    glGetShaderInfoLog(idFragmentShader, 1024, &length, output1);
    printf("FS compile log: %s\n", output1);


    // initialize vertex
    readDataFromFile("sphere_shader.vert", shaderVertSource);
    length = shaderVertSource.length();
    const GLchar* shader2 = (const GLchar*) shaderVertSource.c_str();

    idVertexShader= glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(idVertexShader, 1, &shader2, &length);
    glCompileShader(idVertexShader);

    char output2[1024] = {0};
    glGetShaderInfoLog(idVertexShader, 1024, &length, output2);
    printf("VS compile log: %s\n", output2);


    glAttachShader(idProgramShader, idVertexShader);
    glAttachShader(idProgramShader, idFragmentShader);

    glLinkProgram(idProgramShader);

}
void getHeightMap(char *filename,int *w, int *h){
{
    int width, height;

    unsigned char *raw_image = NULL;
    int bytes_per_pixel = 3;   /* or 1 for GRACYSCALE images */
    int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1];

    FILE *infile = fopen( filename, "rb" );
    unsigned long location = 0;
    int i = 0, j = 0;

    if ( !infile )
    {
        printf("Error opening jpeg file %s\n!", filename );
        return;
    }
    printf("Texture filename = %s\n",filename);

    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error( &jerr );
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress( &cinfo );
    /* this makes the library read from infile */
    jpeg_stdio_src( &cinfo, infile );
    /* reading the image header which contains image information */
    jpeg_read_header( &cinfo, TRUE );
    /* Start decompression jpeg here */
    jpeg_start_decompress( &cinfo );

    /* allocate memory to hold the uncompressed image */
    raw_image = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );
    /* read one scan line at a time */
    while( cinfo.output_scanline < cinfo.image_height )
    {
        jpeg_read_scanlines( &cinfo, row_pointer, 1 );
        for( i=0; i<cinfo.image_width*cinfo.num_components;i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;

    glGenTextures(1,&idHeightTexture);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, idHeightTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);


    glGenerateMipmap(GL_TEXTURE_2D);
    /* wrap up decompression, destroy objects, free pointers and close open files */
    jpeg_finish_decompress( &cinfo );
    jpeg_destroy_decompress( &cinfo );
    free( row_pointer[0] );
    free( raw_image );
    fclose( infile );

    }
}
void initTexture(char *filename,int *w, int *h)
{
    int width, height;

    unsigned char *raw_image = NULL;
    int bytes_per_pixel = 3;   /* or 1 for GRACYSCALE images */
    int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1];

    FILE *infile = fopen( filename, "rb" );
    unsigned long location = 0;
    int i = 0, j = 0;

    if ( !infile )
    {
        printf("Error opening jpeg file %s\n!", filename );
        return;
    }
    printf("Texture filename = %s\n",filename);

    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error( &jerr );
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress( &cinfo );
    /* this makes the library read from infile */
    jpeg_stdio_src( &cinfo, infile );
    /* reading the image header which contains image information */
    jpeg_read_header( &cinfo, TRUE );
    /* Start decompression jpeg here */
    jpeg_start_decompress( &cinfo );

    /* allocate memory to hold the uncompressed image */
    raw_image = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );
    /* read one scan line at a time */
    while( cinfo.output_scanline < cinfo.image_height )
    {
        jpeg_read_scanlines( &cinfo, row_pointer, 1 );
        for( i=0; i<cinfo.image_width*cinfo.num_components;i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;

    glGenTextures(1,&idJpegTexture);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, idJpegTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);

    *w = width;
    *h = height;

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   

    jpeg_finish_decompress( &cinfo );
    jpeg_destroy_decompress( &cinfo );
    free( row_pointer[0] );
    free( raw_image );
    fclose( infile );

}
bool is_change = false;
void keyboard_activites(GLFWwindow *window, int key, int scancode, int action, int mods){
    if (action==GLFW_PRESS || action==GLFW_REPEAT){
        switch(key){
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            case GLFW_KEY_W:
                cameraGaze = glm::rotate(cameraGaze, glm::radians(0.05f), cameraU);
			    cameraUp = glm::rotate(cameraUp, glm::radians(0.05f), cameraU);
                break;
            case GLFW_KEY_S:
                cameraGaze = glm::rotate(cameraGaze, glm::radians(-0.05f), cameraU);
			    cameraUp = glm::rotate(cameraUp, glm::radians(-0.05f), cameraU);
                break;
            case GLFW_KEY_A:
                cameraGaze = glm::rotate(cameraGaze, glm::radians(0.05f), cameraUp);
			    cameraU = glm::rotate(cameraU, glm::radians(0.05f), cameraUp);
                break;
            case GLFW_KEY_D:
                cameraGaze = glm::rotate(cameraGaze, glm::radians(-0.05f), cameraUp);
			    cameraU = glm::rotate(cameraU, glm::radians(-0.05f), cameraUp);
                break;
            case GLFW_KEY_R:
                heightFactor = heightFactor + 0.5f;
                glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), heightFactor);
                break;
            case GLFW_KEY_F:
                heightFactor = heightFactor - 0.5f;
                glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), heightFactor);
                break;
            case GLFW_KEY_Q: 
                texOffset = texOffset + 1.0/250;
                glUniform1f(glGetUniformLocation(idProgramShader, "texOffset"), texOffset);
                break;
            case GLFW_KEY_E: 
                texOffset = texOffset - 1.0/250;
                glUniform1f(glGetUniformLocation(idProgramShader, "texOffset"), texOffset);
                break;
            case GLFW_KEY_T:
                lp.y = lp.y - 5;
                glUniform4fv(glGetUniformLocation(idProgramShader, "lightPosition"), 1, &lp.x);
                break;
            case GLFW_KEY_G:
                lp.y = lp.y + 5;
                glUniform4fv(glGetUniformLocation(idProgramShader, "lightPosition"), 1, &lp.x);
                break;
            case GLFW_KEY_LEFT:
                lp.x = lp.x + 5;
                glUniform4fv(glGetUniformLocation(idProgramShader, "lightPosition"), 1, &lp.x);
                break;
            case GLFW_KEY_RIGHT:
                lp.x = lp.x - 5;
                glUniform4fv(glGetUniformLocation(idProgramShader, "lightPosition"), 1, &lp.x);
                break;
            case GLFW_KEY_UP:
                lp.z = lp.z + 5;
                glUniform4fv(glGetUniformLocation(idProgramShader, "lightPosition"), 1, &lp.x);
                break;
            case GLFW_KEY_DOWN:
                lp.z = lp.z - 5;
                glUniform4fv(glGetUniformLocation(idProgramShader, "lightPosition"), 1, &lp.x);
                break;
            case GLFW_KEY_Y:
                cameraSpeed = cameraSpeed + 0.01;
                break;
            case GLFW_KEY_H:
                cameraSpeed = cameraSpeed - 0.01;
                break;
            case GLFW_KEY_X:
                cameraSpeed = 0.0f;
                break;
            case GLFW_KEY_I:
                cameraPos = glm::vec3(0, 600, 0);
                cameraGaze = glm::vec3(0.0f, -1.0f, 0.0f);
                cameraUp = glm::vec3(0.0f,0.0f, 1.0f);
                cameraU = glm::vec3(1.0f,0.0f, 0.0f);
                lightPosition = glGetUniformLocation(idProgramShader, "lightPosition");
                lp = glm::vec4((float) 0, 1600.0f, (float) 0,1);
                glUniform4fv(glGetUniformLocation(idProgramShader, "lightPosition"), 1, &lp.x);
                cameraSpeed = 0.0f;
                texOffset = 0.0;
                glUniform1f(glGetUniformLocation(idProgramShader, "texOffset"), texOffset);
                heightFactor = 0.0;
                glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), heightFactor);
                
                break;
            case GLFW_KEY_P:
                if (!is_change) {
                    glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(),0,0,widthImagePlane,heightImagePlane,GLFW_DONT_CARE);
                    
                } else {
                    glfwSetWindowMonitor(window, NULL,1000, 1000,widthImagePlane,heightImagePlane,GLFW_DONT_CARE);
                }
                is_change = !is_change;
            default:
                return;
            

        }
    }

}
void window_size_callback(GLFWwindow* window, int width, int height)
{
    widthImagePlane = width;
    heightImagePlane = height;
    glViewport(0, 0, width, height);
}
int main(int argc, char * argv[]) {
    if (argc != 3) {
        printf("One height map, one texture map are expected\n");
        exit(-1);
    }
    glfwSetErrorCallback(errorCallback);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    win = glfwCreateWindow(widthImagePlane, heightImagePlane, "Ceng477-HW3", nullptr, nullptr);
    glfwMakeContextCurrent(win);
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    initShaders();
    initTexture(argv[2], & widthTexture, & heightTexture);
    lp = glm::vec4((float) 0.0, 1600.0f, (float) 0, 1);
    getHeightMap(argv[1], & widthTexture, & heightTexture);
    int radius = 350;
    int horizCount = 250;
    int vertCount = 125;
    float horizStep = 2 * M_PI / horizCount;
    float vertStep =  M_PI / vertCount;
    float alpha_horiz, beta_vert;
  
    std::vector <VNC> vertices( (horizCount + 1) * (vertCount + 1));
    std::vector <Vec3i> indices(2 * (horizCount + 1) * (vertCount + 1));
    float x,y,z,r_cos;
    float s,t;
    float nx, ny, nz;
    for (int i = 0; i <= vertCount; ++i) {
        beta_vert =M_PI / 2 - i * vertStep;
        r_cos = radius * cosf(beta_vert);
        z = radius * sinf(beta_vert);
        for (int  j = 0; j <= horizCount; ++j) {
            alpha_horiz = j * horizStep;
            x = r_cos * cosf(alpha_horiz);             
            y = r_cos * sinf(alpha_horiz);            
            vertices[i*horizCount + j].x = (float) x;
            vertices[i*horizCount + j].y = (float) y;
            vertices[i*horizCount + j].z = (float) z;
            
            vertices[i*horizCount + j].nx = (float) x * 1.0f / radius;
            vertices[i*horizCount + j].ny = (float) y * 1.0f / radius;
            vertices[i*horizCount + j].nz = (float) z * 1.0f / radius;

            vertices[i*horizCount + j].xTexPos = (float) j  / horizCount;
            vertices[i*horizCount + j].yTexPos = (float) i / vertCount;
        }
    }

    int k1,k2;
    int k=0;
    for (int i = 0; i < vertCount +1 ; ++i) {
        k1 = i * (horizCount + 1);     
        k2 = k1 + horizCount + 1;   
        for (int j = 0; j < horizCount +1 ; ++j, ++k1, ++k2) {
            if(i != 0)
            {
                indices[k].x = k1;
                indices[k].y = k2;
                indices[k].z = k1 + 1;
                k++;
            }
            if(i != vertCount)
            {
                indices[k].x =k1 + 1;
                indices[k].y =k2;
                indices[k].z =k2 + 1;
                k++;
            }
        }
    }

    ///////////////////////////////////////////////////////////////////
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Vec3i), indices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VNC), vertices.data(), GL_DYNAMIC_DRAW);
    
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
     glEnableVertexAttribArray(0);
     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*3));
     glEnableVertexAttribArray(1);
     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*6));
     glEnableVertexAttribArray(2);

    
    glUseProgram(idProgramShader);

    sampler1 = glGetUniformLocation(idProgramShader, "sampler1");
    sampler2 = glGetUniformLocation(idProgramShader, "sampler2");
    glUniform1i(sampler1, 0);
    glUniform1i(sampler2, 1);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, idHeightTexture);
    
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, idJpegTexture);
        

    cameraPos = glm::vec3(0.0f , 600.0f ,0.0f);
    cameraGaze = glm::vec3(0.0f, -1.0f, 0.0f);
    cameraUp = glm::vec3(0.0f,0.0f, 1.0f);
    cameraU = glm::vec3(1.0f,0.0f, 0.0f);
    viewMatrix = glm::lookAt(cameraPos, glm::vec3(cameraPos.x + cameraGaze.x * 0.1, cameraPos.y + cameraGaze.y * 0.1, cameraPos.z + cameraGaze.z * 0.1),cameraUp);
    lightPosition = glGetUniformLocation(idProgramShader, "lightPosition");
    glUniform4fv(lightPosition, 1, &lp.x);

    unsigned int mv_loc = glGetUniformLocation(idProgramShader, "ModelView");
    glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    projectionMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 1000.0f);
    unsigned int mvp_loc = glGetUniformLocation(idProgramShader, "ModelViewProjection");
    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), heightFactor);
    glUniform1f(glGetUniformLocation(idProgramShader, "texOffset"), texOffset);
    glUniform1i(glGetUniformLocation(idProgramShader, "widthTexture"), widthTexture);
    glUniform1i(glGetUniformLocation(idProgramShader, "heightTexture"), heightTexture);

    glfwSetWindowSizeCallback(win, window_size_callback);
    glfwSetKeyCallback(win, keyboard_activites);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 
    while (!glfwWindowShouldClose(win)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraGaze, cameraUp);
        viewMatrix = glm::lookAt(cameraPos, glm::vec3(cameraPos.x + cameraGaze.x * 0.1, cameraPos.y + cameraGaze.y * 0.1, cameraPos.z + cameraGaze.z * 0.1),cameraUp);

        unsigned int mv_loc = glGetUniformLocation(idProgramShader, "ModelView");
        glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        unsigned int CamPosLoc = glGetUniformLocation(idProgramShader, "cameraPosition");

        glUniform4f(CamPosLoc, cameraPos.x, cameraPos.y, cameraPos.z, 1.0);
        glDrawElements(GL_TRIANGLES, 3 * 2 * (heightTexture + 1) * (widthTexture + 1), GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(win);
        glfwPollEvents();
        cameraPos = cameraPos + cameraSpeed * cameraGaze;
        

    }
    glfwDestroyWindow(win);
    glfwTerminate();
    
    return 0;

}
