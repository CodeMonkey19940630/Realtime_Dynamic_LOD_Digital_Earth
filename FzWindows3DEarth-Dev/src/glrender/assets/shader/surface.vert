#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 tex;

uniform mat4 u_worldMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform int p_type;
uniform int offset_type;

out vec2 v_tex;

#define PI 3.1415926535897932384626433832795f
#define RADIAN_TO_METER	(6378137.0)
#define METER_TO_RADIAN	(1.0f/(6378137.0))

void main()
{
    v_tex = tex;
    vec4 wPos = u_worldMatrix * vec4(aPos, 1.0);

    if(p_type==0)
    {
        if(offset_type==1)
            wPos.y += 2.0*PI*RADIAN_TO_METER;
        else if(offset_type==-1)
            wPos.y -= 2.0*PI*RADIAN_TO_METER;

    }

    gl_Position = u_projectionMatrix * u_viewMatrix * wPos;
}
