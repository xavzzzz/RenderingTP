#version 410

layout(location = 0) in vec3 in_position;

uniform float _aspectRatio;
uniform float _time;
uniform mat4 _view_projection;

// À mettre avant le main
out vec3 vertex_position;
out vec3 uv;

void main()
{
    vertex_position = in_position;
    vec3 pos = in_position;
    pos.x = (pos.x/_aspectRatio)+cos(_time);
    pos.y = pos.y+sin(_time);
    gl_Position = _view_projection * vec4(pos, 1.);
}