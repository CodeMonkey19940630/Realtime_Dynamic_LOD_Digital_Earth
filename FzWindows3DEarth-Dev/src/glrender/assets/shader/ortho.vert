#version 330 core

uniform mat4 u_worldMatrix;                            
uniform mat4 u_viewMatrix;                            
uniform mat4 u_projectionMatrix;            

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 tex;

out vec2 v_tex;
void main()
{        
    v_tex = tex;
    gl_Position = u_projectionMatrix * u_viewMatrix * u_worldMatrix * vec4(aPos, 1.0);
}
