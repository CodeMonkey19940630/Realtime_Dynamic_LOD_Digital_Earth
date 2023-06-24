#pragma once
#include <GL/GL.h>
#include <vector>
#ifndef GEOMATRIX_H
#include "GeoMath/GeoMatrix.h"
#endif

using namespace std;
//顶点
struct Vertex {
	// 位置
	gVector3f Position;
	// 法向量
	gVector3f Normal;
	// 纹理坐标
	gVector2f TexCoords;
	// u向量
	gVector3f Tangent;
	// v向量
	gVector3f Bitangent;
};

//纹理
struct Texture {
	unsigned int id;
	string type;
	string path;
	Texture()
	{
		id = 0;
	}
};

struct Material {
	//材质颜色光照
	gVector4f Ka;
	//漫反射
	gVector4f Kd;
	//镜反射
	gVector4f Ks;

	gVector4f Ke;

	float Ns;	//shininess
	Material()
	{
		Ns = 1;
	}
};

//Mesh类
class Mesh {
public:
	/*  Mesh 数据  */
	vector<unsigned int> indices;
    vector<Texture*> textures;

	Material mats;
	bool hasTexture;

	GLfloat* vertPos;
	GLfloat* vertNor;
	GLfloat* vertTex;
	GLfloat* vertTan;
	GLfloat* vertBitan; 

	/*  函数  */
	// 构造函数 参数：顶点 索引 纹理
	Mesh(const vector<Vertex>& _vertices, const vector<unsigned int>& _indices, const vector<Texture*>& _textures, Material _mat, bool _hasTex)
	{
		this->indices = _indices;
		this->textures = _textures;
		this->mats = _mat;
		this->hasTexture = _hasTex;

		unsigned int v_size = _vertices.size();

		vertPos = new GLfloat[v_size * 3];
		vertNor = new GLfloat[v_size * 3];
		vertTex = new GLfloat[v_size * 2];
		vertTan = new GLfloat[v_size * 3];
		vertBitan = new GLfloat[v_size * 3];

		for (int i=0;i<v_size;i++)
		{
			vertPos[i * 3] = _vertices[i].Position.x;
			vertPos[i * 3 + 1] = _vertices[i].Position.y;
			vertPos[i * 3 + 2] = _vertices[i].Position.z;

			vertNor[i * 3] = _vertices[i].Normal.x;
			vertNor[i * 3 + 1] = _vertices[i].Normal.y;
			vertNor[i * 3 + 2] = _vertices[i].Normal.z;

			vertTex[i * 2] = _vertices[i].TexCoords.x;
			vertTex[i * 2 + 1] = _vertices[i].TexCoords.y;

			vertTan[i * 3] = _vertices[i].Tangent.x;
			vertTan[i * 3 + 1] = _vertices[i].Tangent.y;
			vertTan[i * 3 + 2] = _vertices[i].Tangent.z;

			vertBitan[i * 3] = _vertices[i].Bitangent.x;
			vertBitan[i * 3 + 1] = _vertices[i].Bitangent.y;
			vertBitan[i * 3 + 2] = _vertices[i].Bitangent.z;

		}
	}
	~Mesh()
	{
		delete[] vertPos, vertPos = nullptr;
		delete[] vertNor, vertNor = nullptr;
		delete[] vertTex, vertTex = nullptr;
		delete[] vertTan, vertTan = nullptr;
		delete[] vertBitan, vertBitan = nullptr;
	}

public:


};
