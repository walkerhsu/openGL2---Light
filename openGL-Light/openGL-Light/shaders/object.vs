#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 normal;
out vec3 FragPos;
out vec3 LightPos;
out vec2 TexCoords;

uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 inverseModel;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(view * model * vec4(aPos , 1.0));
    normal = mat3(transpose(inverseModel)) * aNormal;
    LightPos = vec3(view * vec4(lightPos , 1.0));
    TexCoords = aTexCoords;
    
}
