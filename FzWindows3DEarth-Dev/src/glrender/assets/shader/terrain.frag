#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler * vec4(ourColor, 1.0)
uniform sampler2D shaderTexture;

void main()
{
	FragColor = texture(shaderTexture, TexCoord);
}