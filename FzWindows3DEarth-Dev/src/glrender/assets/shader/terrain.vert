#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 tex;

uniform mat4 u_worldMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;


out vec2 TexCoord;
void main()
{
    TexCoord = tex;
    vec4 wPos =  vec4(aPos, 1.0)*u_worldMatrix;
    gl_Position = wPos*u_viewMatrix*u_projectionMatrix;
}