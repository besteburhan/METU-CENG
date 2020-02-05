#version 410

layout(location = 0) in vec3 vertex;

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
    // initial
    textureCoordinate = vec2( - ((vertex.x+texOffset) / widthTexture),  - (vertex.z / heightTexture));
    vec4 texColor = texture(sampler1, textureCoordinate);
    vec4 heightMap = texture(sampler2, textureCoordinate);
    float height = heightFactor * heightMap.x;

    // left
    vec2 coordLeft = vec2(-(vertex.x+texOffset-1) / widthTexture, -vertex.z / heightTexture);
    heightMap = texture(sampler2, coordLeft);
    float hLeft = heightFactor * heightMap.x;
    vec3 left_vert = vec3(-1, hLeft-vertex.y, 0);
    // right
    vec2 coordRight = vec2(-(vertex.x+texOffset+1) / widthTexture, -vertex.z / heightTexture);
    heightMap = texture(sampler2, coordRight);
    float hRight = heightFactor * heightMap.x;
    vec3 right_vert = vec3(1, hRight-vertex.y, 0);
    // bottom
    vec2 coordBottom = vec2(-(vertex.x+texOffset) / widthTexture, -(vertex.z-1) / heightTexture);
    heightMap = texture(sampler2, coordBottom);
    float hBottom = heightFactor *heightMap.x;
    vec3 bottom_vert = vec3(0, hBottom-vertex.y, -1);
    // top
    vec2 coordTop = vec2(-(vertex.x+texOffset) / widthTexture, -(vertex.z+1) / heightTexture);
    heightMap = texture(sampler2, coordTop);
    float hTop = heightFactor * heightMap.x;
    vec3 top_vert = vec3(0, hTop-vertex.y, 1);
    // right
    vec2 coordTopRight = vec2(-(vertex.x+texOffset+1) / widthTexture, -(vertex.z+1) / heightTexture);
    heightMap = texture(sampler2, coordTopRight);
    float hTopRight = heightFactor * heightMap.x;
    vec3 top_right_vert = vec3(1, hTopRight-vertex.y, 1);
    // bottom left
    vec2 coordBottomLeft = vec2(-(vertex.x+texOffset-1) / widthTexture, -(vertex.z -1)/ heightTexture);
    heightMap = texture(sampler2, coordBottomLeft);
    float hBottomLeft = heightFactor * heightMap.x;
    vec3 bottom_left_vert = vec3(-1, hBottomLeft-vertex.y, -1);

    vec3 n1 = cross(left_vert,top_vert);
    vec3 n2 = cross( top_vert,top_right_vert);
    vec3 n3 = cross(top_right_vert,right_vert);
    vec3 n4 = cross( right_vert,bottom_vert);
    vec3 n5 = cross( bottom_vert,bottom_left_vert);
    vec3 n6 = cross( bottom_left_vert,left_vert);

    vertexNormal = n1 + n2 + n3 + n4 + n5 + n6;
    vertexNormal = normalize(vec3(vertexNormal.x / 6, vertexNormal.y / 6, vertexNormal.z / 6));
        

    ToLightVector = normalize( lightPosition.xyz - vec3(vertex.x, height, vertex.z));
    ToCameraVector = normalize(cameraPosition.xyz - vec3(vertex.x, height, vertex.z));
    
    gl_Position = ModelViewProjection * ModelView * vec4(vertex.x, height, vertex.z, 1.0f);


    
}