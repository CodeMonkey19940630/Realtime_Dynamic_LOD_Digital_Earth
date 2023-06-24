#version 330 core
//precision highp float;

uniform sampler2D lableTexture;
uniform sampler2D fontTexture;
uniform vec3 fontColor;
uniform vec3 fontShColor;

in vec2 v_tex;
in float typeLab;

layout(location = 0) out vec4 outColor;

void main()
{
    if(abs(3.0-typeLab)<0.1)
    {
        vec4 color = texture(lableTexture, v_tex);
        outColor = vec4(fontColor.rgb,color.a);
    }
    else if(abs(2.0-typeLab)<0.1)
    {
        vec4 normal = texture(fontTexture, v_tex);
        outColor = normal;
    }
    else
    {
        vec4 color = texture(lableTexture, v_tex);
        outColor = color;
    }


}
