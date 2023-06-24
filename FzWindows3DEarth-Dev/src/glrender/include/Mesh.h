#pragma once
#include <GL/GL.h>
#include <vector>
#ifndef GEOMATRIX_H
#include "GeoMath/GeoMatrix.h"
#endif

using namespace std;
//����
struct Vertex {
	// λ��
	gVector3f Position;
	// ������
	gVector3f Normal;
	// ��������
	gVector2f TexCoords;
	// u����
	gVector3f Tangent;
	// v����
	gVector3f Bitangent;
};

//����
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
	//������ɫ����
	gVector4f Ka;
	//������
	gVector4f Kd;
	//������
	gVector4f Ks;

	gVector4f Ke;

	float Ns;	//shininess
	Material()
	{
		Ns = 1;
	}
};

//Mesh��
class Mesh {
public:
	/*  Mesh ����  */
	vector<unsigned int> indices;
    vector<Texture*> textures;

	Material mats;
	bool hasTexture;

	GLfloat* vertPos;
	GLfloat* vertNor;
	GLfloat* vertTex;
	GLfloat* vertTan;
	GLfloat* vertBitan; 

	/*  ����  */
	// ���캯�� ���������� ���� ����
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
