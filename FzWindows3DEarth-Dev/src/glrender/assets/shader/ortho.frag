#version 330 core

//precision mediump float;
//precision mediump int;                       

uniform sampler2D shaderTexture;

in vec2 v_tex;

layout(location = 0) out vec4 outColor;


void main()                            
{

    outColor = texture(shaderTexture, v_tex);

}
