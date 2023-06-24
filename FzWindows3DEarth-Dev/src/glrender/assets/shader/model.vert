#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec4 instanceVecRow1;
layout(location = 4) in vec4 instanceVecRow2;
layout(location = 5) in vec4 instanceVecRow3;
layout(location = 6) in vec4 instanceVecRow4;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragVert;
out vec3 fragNormal;
out vec2 fragTexCoord;


void main()
{
//    mat4 instanceMatrix=mat4(vec4(1,0,0,0),vec4(0,1,0,0),vec4(0,0,1,0),vec4(0,0,0,1));
    mat4 instanceMatrix=mat4(instanceVecRow1,instanceVecRow2,instanceVecRow3,instanceVecRow4);
    fragVert = vec3( instanceMatrix * vec4(aPos, 1.0));
    fragNormal = mat3(transpose(inverse(instanceMatrix))) * aNormal;
    fragTexCoord = aTexCoords;
    gl_Position = projection * view * vec4(fragVert, 1.0);
}
