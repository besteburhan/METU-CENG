#version 410

out vec4 color;

uniform mat4 ModelViewProjection; 
uniform mat4 ModelView;
uniform vec4 cameraPosition;

uniform int widthTexture;
uniform int heightTexture;
uniform sampler2D sampler1;
uniform sampler2D sampler2;

in vec2 textureCoordinate;
in vec3 vertexNormal; 
in vec3 ToLightVector; 
in vec3 ToCameraVector; 

void main() {
    vec4 textureColor = texture(sampler1, textureCoordinate);
    vec4 ka = vec4(0.25,0.25,0.25,1.0); 
    vec4 Ia = vec4(0.3,0.3,0.3,1.0);
    vec4 Id = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 kd = vec4(1.0, 1.0, 1.0, 1.0);
    int phong = 100; 
    vec4 Is = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 ks = vec4(1.0, 1.0, 1.0, 1.0); 
    float cos_theta = max(dot(ToLightVector, vertexNormal), 0);
    vec3 h = (ToLightVector + ToCameraVector) / length(ToLightVector + ToCameraVector);
    float cos_alpha = pow(max(dot(vertexNormal, h), 0.0), phong);
    vec4 ambient = ka * Ia;
    vec4 diffuse = cos_theta * Id * kd;
    vec4 specular = ks * Is * cos_alpha;
    color = vec4(clamp( textureColor.xyz * vec3(ambient + diffuse + specular), 0.0, 1.0), 1.0);
}

