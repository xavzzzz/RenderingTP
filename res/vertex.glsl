#version 410

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 _view_projection;
uniform mat4 _transform;

void main()
{
    gl_Position = _view_projection * _transform * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}