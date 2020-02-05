#version 410

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 coord;

uniform mat4 ModelViewProjection; 
uniform mat4 ModelView; 
uniform vec4 cameraPosition;
uniform float heightFactor;
uniform float texOffset;
uniform vec4 lightPosition;


uniform sampler2D sampler1;
uniform sampler2D sampler2;
uniform int widthTexture;
uniform int heightTexture;


out vec2 textureCoordinate; 
out vec3 vertexNormal; 
out vec3 ToLightVector;
out vec3 ToCameraVector; 


void main()
{
    textureCoordinate = vec2(coord.x+texOffset, coord.y);
    vec4 heightMap = texture(sampler2, textureCoordinate);
    vec3 v = vertex +  heightFactor * normal * heightMap.x;
    vertexNormal = normalize(normal);
    ToLightVector = normalize( lightPosition.xyz - v);
    ToCameraVector = normalize(cameraPosition.xyz - v);
    gl_Position = ModelViewProjection * ModelView * vec4(v.x, v.y, v.z, 1.0f);
}