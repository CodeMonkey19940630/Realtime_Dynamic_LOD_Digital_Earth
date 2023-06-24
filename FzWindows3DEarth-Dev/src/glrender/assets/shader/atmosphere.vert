#version 330 core
uniform vec3 v3CameraPos;		// The camera's current position
uniform vec3 v3LightPos;		// The direction vector to the light source
uniform vec3 v3InvWavelength;	// 1 / pow(wavelength, 4) for the red, green, and blue channels
uniform float fCameraHeight;	// The camera's current height
uniform float fCameraHeight2;	// fCameraHeight^2
uniform float fOuterRadius;		// The outer (atmosphere) radius
uniform float fOuterRadius2;	// fOuterRadius^2
uniform float fInnerRadius;		// The inner (planetary) radius
uniform float fInnerRadius2;	// fInnerRadius^2
uniform float fKrESun;			// Kr * ESun
uniform float fKmESun;			// Km * ESun
uniform float fKr4PI;			// Kr * 4 * PI
uniform float fKm4PI;			// Km * 4 * PI
uniform float fScale;			// 1 / (fOuterRadius - fInnerRadius)
uniform float fScaleDepth;		// The scale depth (i.e. the altitude at which the atmosphere's average density is found)
uniform float fScaleOverScaleDepth;	// fScale / fScaleDepth
uniform mat4 WorldViewProj;
uniform int nSamples;
uniform float fSamples;
                 
layout(location = 0) in vec3 aPos;

out vec4 pos;
out vec2 texCoord; 
out vec3 direction;
out vec3 color;
out vec3 secondaryColor;

float scale(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

void main()
{
	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	vec3 v3Pos = aPos;
	vec3 cameraPos = v3CameraPos;
	vec3 light = v3LightPos;
	
	vec3 v3Ray = v3Pos - cameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;


	
	vec3 v3Start;
	float fStartOffset;
	
	if(length(v3CameraPos) >= fOuterRadius)
	{
		// Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
		float B = 2.0 * dot(cameraPos, v3Ray);
		float C = fCameraHeight2 - fOuterRadius2;
		float fDet = max(0.0, B*B - 4.0 * C);
		float fNear = 0.5 * (-B - sqrt(fDet));
	
		// Calculate the ray's starting position, then calculate its scattering offset
		v3Start = cameraPos + v3Ray * fNear;
		fFar -= fNear;
		float fStartAngle = dot(v3Ray, v3Start) / fOuterRadius;
//		float fStartAngle = dot(normalize(v3Ray), normalize(v3Start));
		float fStartDepth = exp(-1.0 / fScaleDepth);
		fStartOffset = fStartDepth*scale(fStartAngle);
	
	}
	else
	{
		// Calculate the ray's starting position, then calculate its scattering offset
		v3Start = cameraPos;
		float fHeight = length(v3Start);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
		float fStartAngle = dot(v3Ray, v3Start) / fHeight;
//		float fStartAngle = dot(normalize(v3Ray), normalize(v3Start));
		fStartOffset = fDepth*scale(fStartAngle);	
	
	}



	// Initialize the scattering loop variables
	//gl_FrontColor = vec4(0.0, 0.0, 0.0, 0.0);
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	vec3 v3SampleRay = v3Ray * fSampleLength;
	vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

	// Now loop through the sample rays
	vec3 v3FrontColor = vec3(0.0, 0.0, 0.0);
	for(int i=0; i<nSamples; i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fLightAngle = dot(light, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
//		float fLightAngle = dot(light, normalize(v3SamplePoint));
//		float fCameraAngle = dot(normalize(v3Ray), normalize(v3SamplePoint));
		float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle)));
		vec3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}

	// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
	direction = cameraPos - v3Pos;
	secondaryColor = v3FrontColor * fKmESun;
	
	color = v3FrontColor * (v3InvWavelength * fKrESun);

	
	gl_Position = WorldViewProj*vec4(aPos,1.0);
}
