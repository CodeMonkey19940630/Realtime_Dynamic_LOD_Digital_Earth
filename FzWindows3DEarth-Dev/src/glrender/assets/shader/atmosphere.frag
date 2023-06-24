#version 330 core
//precision mediump float;
//precision mediump int;

uniform vec3 v3LightPosMed;		// The direction vector to the light source
uniform float g;
uniform float g2;    
 
in vec4 pos;
in vec2 texCoord; 
in vec3 direction;
in vec3 color;
in vec3 secondaryColor; 
                      
layout(location = 0) out vec4 outColor;


void main()                            
{                                      
	vec3 light = normalize(v3LightPosMed);
	vec3 dir = normalize(direction);
//	float fCos = dot(v3LightPosMed, direction) / length(direction);
	float fCos = dot(light, dir);
	float fRayleighPhase = 0.75 * (1.0 + fCos*fCos);
	float fMiePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos*fCos) / pow(abs(1.0 + g2 - 2.0*g*fCos), 1.5);
    vec3 f = fRayleighPhase * color + fMiePhase * secondaryColor;
	
	//outColor = vec4(f, 1.0);		
	//outColor = vec4(color, 1.0);
	//outColor = vec4(secondaryColor, 1.0);
	outColor = vec4(color + secondaryColor, 1.0);

}
