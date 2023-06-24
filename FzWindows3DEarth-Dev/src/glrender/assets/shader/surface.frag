#version 330 core

//precision mediump float;
//precision mediump int; 

uniform vec3 u_lightDirection;
uniform vec4 g_hColor0[12];
uniform vec4 g_wColor[6];
uniform int g_row;
uniform int g_col;
uniform int g_level;
uniform int tex_type; 
uniform int label_mark; 
uniform int flag_sun_light; 
uniform sampler2D shaderTexture;

in vec2 v_tex;

layout(location = 0) out vec4 outColor;

vec4 lerp(vec4 x, vec4 y, float s)
{
    return ( x + s*(y - x));
}
vec3 lerp3(vec3 x, vec3 y, float s)
{
    return ( x + s*(y - x));
}
float PI = 3.1415926535897932384626433832795;
float radius = 6378137.0;

vec4 pole_color[12];

vec3 SphericalToCartesian(
        float latitude,
        float longitude,
        float radius )
{
    float latRadians = latitude*PI/180.0;
    float lonRadians = longitude*PI/180.0;

    float radCosLat = radius * cos(latRadians);

    return vec3((radCosLat * cos(lonRadians)),
                radCosLat * sin(lonRadians),
                radius * sin(latRadians));

}


float tiley2lat(int y, int z)
{
    float n = PI - 2.0 * PI * float(y) / float(1 << z);
    return 180.0 / PI * atan(0.5 * (exp(n) - exp(-n)));
}

float tilex2long(int x, int z)
{
    return float(x) / float(1 << z) * 360.0 - 180.0;
}

float LatToY(float lat)
{
    float sinLat = sin(lat);
    return 0.5*log((1.0 + sinLat) / (1.0 - sinLat))*radius;

}

float YToLat(float y)
{
    return PI / 2.0 - 2.0 * atan(exp(-y / radius));
}

float LonToX(float lon)
{
    return lon * radius;
}

float XToLon(float x)
{
    return x / radius;
}

void main()
{
    //
    vec3 vec_light_dir = u_lightDirection;

    vec4 texNor,colorPic,texW;
    float h;
    vec3 vNormal_0;
    if(tex_type==3 || (tex_type == 1 && (g_row==-1 || g_row==-2)))
    {
        texNor = texture(shaderTexture, v_tex);

        h = texNor.z;

        vNormal_0=vec3(texNor.x,texNor.y,1.0f);

        vNormal_0=normalize(vNormal_0);

    }
    else if(tex_type == 1)
    {
        colorPic = texture(shaderTexture, v_tex);
    }





    //
    float tileSize = 2.0 * PI * radius / pow(2.0, float(g_level));//单位：米

    float north,south,west,nY,sY,wX,mLat,mLon,lat,lon;

    if(g_row == -1)//north pole
    {
        north = 90.0;

        west = tilex2long(g_col, g_level);

        wX = LonToX(west*PI / 180.0);

        south = tiley2lat(0,g_level);


        float span_lat = north - south;

        lat = south + (1.0 - v_tex.y)*span_lat;


        mLon = wX + v_tex.x*tileSize;
        lon = XToLon(mLon)*180.0 / PI;


    }
    else if (g_row == -2) //south pole
    {
        north = -1.0*tiley2lat(0, g_level);//单位：度
        west = tilex2long(g_col, g_level);

        south = -90.0;

        wX = LonToX(west*PI / 180.0);

        float span_lat = north - south;

        lat = south + (1.0 - v_tex.y)*span_lat;


        mLon = wX + v_tex.x*tileSize;
        lon = XToLon(mLon)*180.0 / PI;

    }
    else
    {
        north = tiley2lat(g_row, g_level);//单位：度
        west = tilex2long(g_col, g_level);

        nY = LatToY(north*PI / 180.0);//单位：米
        sY = nY - tileSize;	//单位：米
        south = YToLat(sY)*180.0/PI;

        wX = LonToX(west*PI / 180.0);

        mLat = sY + (1.0 - v_tex.y)*tileSize;//nY  - v_tex.y*tileSize;	//单位：米
        lat = YToLat(mLat)*180.0 / PI;


        mLon = wX + v_tex.x*tileSize;
        lon = XToLon(mLon)*180.0 / PI;

    }

    //
    vec2 w_tex;

    if(lon<0.0f)
    {
        lon = lon + 360.0;
    }

    lon = clamp(lon,0.0,360.0);

    w_tex.x = lon/360.0;

    lat = lat + 90.0;

    lat = clamp(lat,0.0,180.0);

    w_tex.y = (180.0 - lat)/180.0;




    //
    vec3 vNormal = SphericalToCartesian(lat-90.0,lon,radius);
    vNormal=normalize(vNormal);

    //

    vec4 color;
    vec4 colorSea;

    int indexSea0=3;
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    //卫星影像
    if(tex_type == 1 && (g_row==-1 || g_row==-2))
    {
        pole_color[0]=vec4(0.117647,0.196078,0.396078,-8000.0);
        pole_color[1]=vec4(0.219608,0.282353,0.47451,-1000.0);
        pole_color[2]=vec4(0.290196,0.372549,0.592157,-100.0);
        pole_color[3]=vec4(0.305882,0.392157,0.596078,0.0);
        pole_color[4]=vec4(0.6451,0.6569,0.7157,0.0);
        pole_color[5]=vec4(0.7451,0.7569,0.8157,400.0);
        pole_color[6]=vec4(0.7451,0.7569,0.8157,1000.0);
        pole_color[7]=vec4(0.7451,0.7569,0.8157,2000.0);
        pole_color[8]=vec4(0.7451,0.7569,0.8157,5000.0);
        pole_color[9]=vec4(0.7451,0.7569,0.8157,7000.0);
        pole_color[10]=vec4(0.7451,0.7569,0.8157,8000.0);
        pole_color[11]=vec4(0.7451,0.7569,0.8157,8500.0);

        for(int i=0;i<11;i++)
        {
            if(h>pole_color[i].w && h<=pole_color[i+1].w)
            {
                color=lerp(pole_color[i],pole_color[i+1],(h-pole_color[i].w)/(pole_color[i+1].w-pole_color[i].w));
                color.w=1.f;
                colorSea=lerp(pole_color[i],pole_color[i+1],(h-pole_color[i].w)/(pole_color[i+1].w-pole_color[i].w));
                colorSea.w=1.f;
                break;
            }
        }

        if(h<pole_color[0].w)
        {
            color=pole_color[0];		color.w=0.f;
            colorSea=pole_color[0];		colorSea.w=0.f;
        }
        if(h>pole_color[11].w)
        {
            color=pole_color[11];		color.w=0.f;
            colorSea=pole_color[11];		colorSea.w=0.f;
        }

        if(h>0.f)
        {
            colorSea=vec4(pole_color[indexSea0].xyz,1.f);
        }
        if(h<=0.f)
        {
            color=vec4(pole_color[indexSea0+1].xyz,1.f);
        }

    }
    else
    {
        for(int i=0;i<11;i++)
        {
            if(h>g_hColor0[i].w && h<=g_hColor0[i+1].w)
            {
                color=lerp(g_hColor0[i],g_hColor0[i+1],(h-g_hColor0[i].w)/(g_hColor0[i+1].w-g_hColor0[i].w));
                color.w=1.f;
                colorSea=lerp(g_hColor0[i],g_hColor0[i+1],(h-g_hColor0[i].w)/(g_hColor0[i+1].w-g_hColor0[i].w));
                colorSea.w=1.f;
                break;
            }
        }

        if(h<g_hColor0[0].w)
        {
            color=g_hColor0[0];		color.w=0.f;
            colorSea=g_hColor0[0];		colorSea.w=0.f;
        }
        if(h>g_hColor0[11].w)
        {
            color=g_hColor0[11];		color.w=0.f;
            colorSea=g_hColor0[11];		colorSea.w=0.f;
        }

        if(h>0.f)
        {
            colorSea=vec4(g_hColor0[indexSea0].xyz,1.f);
        }
        if(h<=0.f)
        {
            color=vec4(g_hColor0[indexSea0+1].xyz,1.f);
        }

    }



    //////////////////////////////////////////////////////////////////////////////////////////
    //
    vec3 vDiffuse;
    vec3 vDiffuseSea;
    //考虑光照
    if(flag_sun_light==0)
        vDiffuseSea = colorSea.rgb;
    else
        vDiffuseSea = colorSea.rgb * clamp(dot(vNormal, normalize(vec_light_dir.rgb))*1.5, 0.0, 3.0)*1.2;


    //卫星影像
    if(tex_type==1)
    {
        if(g_row==-1)
            vDiffuse = colorSea.rgb * clamp(dot(vNormal_0, vec3(0.1971,-0.6052,0.2895))*3.0 , 0.15, 3.0)*1.2;
        else if(g_row == -2)
            vDiffuse = color.rgb * clamp(dot(vNormal_0, vec3(0.1971,-0.6052,0.2895))*3.0 , 0.0, 3.0)*1.2;
        else
            vDiffuse = colorPic.rgb;

        if(flag_sun_light==1)
        {
            vDiffuse = vDiffuse * clamp(dot(vNormal, normalize(vec_light_dir.rgb))*1.5, 0.25, 2.0)*1.2;
        }

    }

    //标准
    if(tex_type==2)
        //	vDiffuse = vec3(248.0/255.0,245.0/255.0,237.0/255.0);
    {
        if(g_row==-1)
            vDiffuse = colorSea.rgb * clamp(dot(vNormal_0, vec3(0.1971,-0.6052,0.2895))*3.0 , 0.15, 3.0)*1.2;
        else if(g_row == -2)
            vDiffuse = color.rgb * clamp(dot(vNormal_0, vec3(0.1971,-0.6052,0.2895))*3.0 , 0.0, 3.0)*1.2;
        else
            vDiffuse = colorPic.rgb;

        if(flag_sun_light==1)
        {
            vDiffuse = vDiffuse * clamp(dot(vNormal, normalize(vec_light_dir.rgb))*1.5, 0.25, 2.0)*1.2;
        }
    }
    //地势图
    if(tex_type==3)
    {
        if(flag_sun_light==0)
        {
            vDiffuse = color.rgb;
        }
        else
        {
            //考虑光照
            vDiffuse = color.rgb * clamp(dot(vNormal_0, normalize(vec_light_dir.rgb))*1.5, 0.15, 3.0)*1.2;
        }

    }

    vec3 outputColor;

    if(h<=0.f && tex_type!=1)
        outputColor=vDiffuseSea;
    else
        outputColor=vDiffuse;

    if(g_level>3)
    {
        //		if(label_mark == 1)
        //			outputColor = outputColor.rgb*(1.0 - colorR.a) + colorR.rgb*colorR.a;

    }

    //
    outColor = vec4(outputColor,1.0f);

}
