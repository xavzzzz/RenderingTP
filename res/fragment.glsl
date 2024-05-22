#version 410

out vec4 out_color;

uniform vec2 _uniforme; // Vous pouvez mettre le type que vous voulez, et le nom que vous voulez
uniform float _trans;


// À mettre avant le main

uniform sampler2D my_texture;

void main()
{   
    out_color = vec4(uv.x, uv.y, .0, 1.);
}