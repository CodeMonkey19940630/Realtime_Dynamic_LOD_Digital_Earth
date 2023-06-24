#version 330 core
uniform vec3 v3CameraPos;		// The camera's current position
uniform vec4 v4Color;		// line color
uniform mat4 WorldViewProj;
uniform int p_type;
uniform float cameraAlt;
uniform float lineAlt;
uniform int offset_type;

layout(location = 0) in vec2 aPos;
layout(location = 1) in float uTex;

out vec4 pos;
out vec4 color;
out float v_uTex;

#define PI 3.1415926535897932384626433832795f
#define EARTH_MEANRADIUS	6378137.0
#define RADIAN_TO_METER	(6378137.0)
#define METER_TO_RADIAN	(1.0f/(6378137.0))

vec3 SphericalToCartesian(
        float latitude,
        float longitude,
        float radius )
{
    float latRadians = latitude;
    float lonRadians = longitude;

    float radCosLat = radius * cos(latRadians);

    return vec3((radCosLat * cos(lonRadians)),
                radCosLat * sin(lonRadians),
                radius * sin(latRadians));

}

vec3 SphericalToMercator(
        float latitude,
        float longitude,
        float radius )
{
    float sinLat = sin(latitude);
    float z = 0.5*log((1.0 + sinLat)/(1.0 - sinLat));

    float y = longitude;
    vec3 position = vec3( 0.0, y, z ) * RADIAN_TO_METER;
    return vec3( radius,
                 position.y,
                 position.z );
}

void main()
{
    vec3 v3Pos;
    if(p_type==0)
    {
        if(offset_type==0)
            v3Pos = SphericalToMercator(aPos.x,aPos.y,EARTH_MEANRADIUS + lineAlt);
        else if(offset_type==1)
            v3Pos = SphericalToMercator(aPos.x,aPos.y+2.0*PI,EARTH_MEANRADIUS + lineAlt);
        else
            v3Pos = SphericalToMercator(aPos.x,aPos.y-2.0*PI,EARTH_MEANRADIUS + lineAlt);

    }
    if(p_type==1)
        v3Pos = SphericalToCartesian(aPos.x,aPos.y,EARTH_MEANRADIUS + lineAlt);
    vec3 cameraPos = v3CameraPos;

    color = v4Color;
    v_uTex = uTex;

    gl_Position = WorldViewProj*vec4(v3Pos,1.0);
}
