#version 330 core
//precision mediump float;

uniform sampler2D u_stippleTexture;
uniform float lineType;

in vec4 pos;
in vec4 color;
in float v_uTex;

layout(location = 0) out vec4 outColor;


void main()
{
        if(lineType>0.5)
        {
                float stipple = texture(u_stippleTexture, vec2(v_uTex,0.0)).r;
                if (stipple < 0.5)
                discard;
        }

        outColor = color;

}
