#version 410

out vec4 FragColor;
uniform vec3 light_direction;
in vec3 normals;

uniform sampler2D myTexture;

void main()
{
    
    FragColor = vec4(normals, 1);
}