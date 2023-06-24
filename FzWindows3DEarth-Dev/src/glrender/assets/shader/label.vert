#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 tex;

uniform mat4 g_mvpMatrix;
uniform vec4 Frustum;
uniform vec4 Viewport;
uniform vec3 viewPointVec;
uniform vec3 cameraPos;
uniform float nearDis;
uniform float farDis;
uniform vec4 fSize;

uniform float CB_LAT;
uniform float CB_LON;
uniform float CB_HEIGHT;
uniform float CB_YAW;
uniform float CB_PITCH;
uniform float CB_ROLL;
uniform int CB_INDEX;

uniform int p_type;
uniform int type;

out vec2 v_tex;
out float typeLab;

#define PI 3.1415926535897932384626433832795f
#define EARTH_MEANRADIUS	6378137.0
#define RADIAN_TO_METER	(6378137.0)
#define METER_TO_RADIAN	(1.0f/(6378137.0))

//
vec4 QuadPosTexData[4] = vec4[4](
    vec4( -1.0, -1.0, 0.0, 0.0 ),
    vec4( -1.0, 1.0, 1.0, 0.0 ),
    vec4( 1.0, -1.0, 0.0, 1.0 ),
    vec4( 1.0, 1.0, 1.0, 1.0 ));
vec4 LabelPosTexData[4] = vec4[4](

    vec4( -1.0, 0.0, 0.0, 1.0 ),
    vec4( -1.0, 2.0, 0.0, 0.0 ),
    vec4( 1.0, 0.0, 1.0, 1.0 ),
    vec4( 1.0, 2.0, 1.0, 0.0 ));

vec4 LabelTopCPosTexData[4] = vec4[4](

    vec4( 0.0, 0.0, 0.0, 1.0 ),
    vec4( 0.0, 2.0, 0.0, 0.0 ),
    vec4( 2.0, 0.0, 1.0, 1.0 ),
    vec4( 2.0, 2.0, 1.0, 0.0 ));

// Standard Transform Functions

vec4 viewToProj2( vec4 pos )
{
    return vec4(  pos.x * 2.0 * Frustum.z / Frustum.x,
                    pos.y * 2.0 * Frustum.z / Frustum.y,
                    ( Frustum.w / ( Frustum.w - Frustum.z ) ) * ( pos.z - pos.w * Frustum.z ),
                    pos.z );
}

// From window pixel pos to projection frame at the specified z (view frame).
vec2 projToWindow( vec4 pos )
{
    return vec2(  Viewport.x * 0.5 * ( ( pos.x / pos.w ) + 1.0 ) + Viewport.z,
                    Viewport.y * 0.5 * ( 1.0 - ( pos.y / pos.w ) ) + Viewport.w );
}

// From window pixel pos to projection frame at the specified z (view frame).
vec4 windowToProj( vec2 pos, float depth )
{
    return vec4(  ( ( ( pos.x - Viewport.z ) * 2.0 / Viewport.x ) - 1.0 ) * depth,
                    ( ( ( pos.y - Viewport.w ) * 2.0 / Viewport.y ) - 1.0 ) * ( -1.0*depth ),
                    ( depth - Frustum.z ) * Frustum.w / ( Frustum.w - Frustum.z ),
                    depth );
}


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
        //float z = log(tan(PI*0.25+latitude/2.0));

        float sinLat = sin(latitude);
        float z = 0.5*log((1.0 + sinLat)/(1.0 - sinLat));

        float y = longitude;
        vec3 position = vec3( 0.0, y, z ) * RADIAN_TO_METER;
        return vec3( radius,
                                         position.y,
                                         position.z );
}

//
mat4 MatrixFromRotation( float x, float y, float z )
{
    //roll mat4
    mat4 xRot;
    float yaw = x;
    float c = cos( yaw );
    float s = sin( yaw );
        xRot = mat4(1.0,0.0,0.0,0.0,0.0,c,s,0.0,0.0,-1.0*s,c,0.0,0.0,0.0,0.0,1.0);

    //pitch mat4
    mat4 yRot;
    float pitch = y;
    c = cos( pitch );
    s = sin( pitch );

        yRot = mat4(c,0.0,-1.0*s,0.0,0.0,1.0,0.0,0.0,s,0.0,c,0.0,0.0,0.0,0.0,1.0);

    //yaw mat4
    mat4 zRot;
    float roll = z;
    c = cos( roll );
    s = sin( roll );

        zRot = mat4(c,s,0.0,0.0,-1.0*s,c,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0);

    mat4 outMatrix;
    //outMatrix = mul( zRot, yRot );
    //outMatrix = mul( outMatrix, xRot );
    outMatrix = xRot*yRot*zRot;
    return outMatrix;
}

mat4 MatrixFromScale( vec3 sxyz )
{
    mat4 scaleTrans;

        scaleTrans = mat4(sxyz.x,0.0,0.0,0.0,0.0,sxyz.y,0.0,0.0,0.0,0.0,sxyz.z,0.0,0.0,0.0,0.0,1.0);

    return scaleTrans;
}

mat4 MatrixTranslation( vec3 trans )
{
    mat4 scaleTrans;

        scaleTrans = mat4(1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,trans.x,trans.y,trans.z,1.0);

    return scaleTrans;
}

float lerp1f(float x, float y, float s)
{
        return ( x + s*(y - x));
}


void main()
{
    //
        vec3 spherePos;
        if(p_type==0)
                spherePos= SphericalToMercator(CB_LAT,CB_LON,EARTH_MEANRADIUS + CB_HEIGHT);
        if(p_type==1)
                spherePos= SphericalToCartesian(CB_LAT,CB_LON,EARTH_MEANRADIUS + CB_HEIGHT);

        vec4 center = g_mvpMatrix*vec4(spherePos,1.0);

        float alpha = 90.0;

        float  alphaR = radians( alpha );
    vec4 cs = vec4( sin( alphaR ), cos( alphaR ), -cos( alphaR ), sin( alphaR ) );

        vec2 p[5];
    p[0] = projToWindow( center );
    p[1] = vec2( fSize[0], fSize[1] );
    p[2] = p[1] + vec2( fSize[2], 0.0 );
    p[3] = p[2] + vec2( 0.0, fSize[3] );
    p[4] = p[1] + vec2( 0.0, fSize[3] );

        vec2 pt[4];
        pt[0] = p[1];
        pt[1] = p[2];
        pt[2] = p[4];
        pt[3] = p[3];

        vec2 pa = vec2( pt[gl_VertexID].x * cs[0] + pt[gl_VertexID].y * cs[1], -pt[gl_VertexID].x * cs[1] + pt[gl_VertexID].y * cs[0] );


        gl_Position = windowToProj( p[0] + pa, center.w );

//	v_tex = QuadPosTexData[gl_VertexID].zw;
        if(type == 2)
    {
                v_tex = tex;
        typeLab = 2.0;
    }
        if(type == 1)
    {
                v_tex = QuadPosTexData[gl_VertexID].zw;
        typeLab = 1.0;
    }
    if(type == 0)
    {
        v_tex = QuadPosTexData[gl_VertexID].zw;
        typeLab = 0.0;
    }
    if(type == 3)
    {
        v_tex = QuadPosTexData[gl_VertexID].zw;
        typeLab = 3.0;
    }

}
