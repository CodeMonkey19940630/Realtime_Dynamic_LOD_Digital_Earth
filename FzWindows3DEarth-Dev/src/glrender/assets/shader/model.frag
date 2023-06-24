#version 330 core

uniform sampler2D texture_diffuse1;
uniform int hasTex;

uniform struct AmbientLight
{
        vec3 intensities;
} ambientLight;

uniform struct DirectionalLight
{
        vec3 direction;
        vec3 intensities;
} directionalLight;

uniform struct Material
{
    float Ns;
    vec3 ke;
    vec3 ka;
    vec3 kd;
    vec3 ks;
} material;

uniform vec3 eyePosition;

in vec3 fragVert;
in vec3 fragNormal;
in vec2 fragTexCoord;


layout(location = 0) out vec4 outColor;

//// ignore the kd and ka of material and lock them together

void main()
{

    float texU = fragTexCoord.x -floor(fragTexCoord.x);
    float texV = fragTexCoord.y - floor(fragTexCoord.y);

//    vec4 surfaceColor = texture(texture_diffuse1, vec2(fragTexCoord.x,1.0-fragTexCoord.y));
    vec4 surfaceColor = texture(texture_diffuse1, vec2(texU,texV));
    if(hasTex == 0) surfaceColor = vec4(1.0,1.0,1.0,1.0) ;

    // emissive
    vec3 emissive = material.ke;
    // ambient
//    vec3 ambient = material.ka * ambientLight.intensities;
    vec3 ambient = ambientLight.intensities;

    // diffuse
    vec3 N = normalize(fragNormal);
    vec3 L = normalize(directionalLight.direction);     // lightPosition - fragVert
    float brightness = max(dot(N,L), 0.0);
    vec3 diffuse = directionalLight.intensities * brightness;
    if (hasTex == 0) diffuse = material.kd * directionalLight.intensities * brightness;

    // specular
    vec3 V = normalize(eyePosition - fragVert);
    vec3 H = normalize(L + V);
    float specularLight = pow(max(dot(N,H), 0.0), material.Ns);
    specularLight = max(specularLight, 0.0);
    vec3 specular = material.ks * directionalLight.intensities * specularLight;
//    vec3 specular = directionalLight.intensities * specularLight;

    outColor.rgb = emissive + ambient + diffuse + specular;
    outColor.rgb = outColor.rgb * surfaceColor.rgb;
    outColor.a = 1.0;


    //// debug

//    outColor.rgb = fragNormal;
//	outColor.rgb = surfaceColor.rgb;
//	outColor.rgb = vec3(fragTexCoord,0.0);
}
