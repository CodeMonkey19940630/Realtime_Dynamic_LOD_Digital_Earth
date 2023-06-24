#pragma once
#include <qopengl.h>
#define STB_IMAGE_IMPLEMENTATION        //原作者没写
#include <stb_image.h>
inline GLuint CreateTexture(const char* fileName)
{
	GLuint    textureId = 0;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// 设置纹理过滤参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 为当前绑定纹理对象设置
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// 加载并生成纹理
	int width, height, nrChannels;

	unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, STBI_rgb_alpha);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
	}
	else
	{
		glDeleteTextures(1, &textureId);
		textureId = 0;
	}

	return  textureId;
}

inline GLuint CreateTexture(const unsigned char *data, int width, int height, GLint internalformat = GL_RGBA)
{
	GLuint    textureId = 0;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// 设置纹理过滤参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 为当前绑定纹理对象设置
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, internalformat, GL_UNSIGNED_BYTE, data);
		//		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		glDeleteTextures(1, &textureId);
		textureId = 0;
	}

	return  textureId;
}