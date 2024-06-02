#version 410

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D myTexture;

void main()
{
    
    FragColor = texture(myTexture, TexCoord); // Display UV coordinates as colors
}