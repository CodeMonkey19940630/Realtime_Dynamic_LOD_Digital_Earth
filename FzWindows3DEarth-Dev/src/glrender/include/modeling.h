#pragma once

#include <assimp/Importer.hpp>        //assimp��ͷ�ļ�
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <iostream>
#include "Mesh.h"

//�������̴߳�����Ӧ��������
extern void CreateTextureOtherThread(const char* fileName, unsigned int* _id);
class Modeling
{
public:
	//Assimp::Importer * importer;
    //�洢��ĿǰΪֹ���ص����������Ż���ȷ�������ᱻ���ض�Ρ�
    vector<Texture*> textures_loaded;
    vector<Mesh*> meshes;
    string directory;
    string modeling_name_;
    bool gammaCorrection;
protected:
    vector<gMatrix4f> motion_matrixs_;
	bool			  is_loading_;

public:
    /*  ����  */
    Modeling()
    {
		is_loading_ = true;
    }
    // ���캯��,��Ҫһ��3Dģ�͵��ļ�·��
    Modeling(string const &path, bool gamma = false) : gammaCorrection(gamma)
	{
        loadModel(path);
	}
	// ���캯��,��Ҫһ��3Dģ�͵��ļ�����
	Modeling(const void* buffer, size_t length, bool gamma = false) : gammaCorrection(gamma)
	{
		loadModel(buffer, length);
	}
    ~Modeling()
    {
		for (size_t i = 0; i < meshes.size(); i++)
		{
			delete meshes[i], meshes[i] = nullptr;
		}
		meshes.clear();
		for (size_t i = 0; i < textures_loaded.size(); i++)
		{
			if (!textures_loaded[i])
			{
				continue;
			}
			if (textures_loaded[i]->id != 0){
				glDeleteTextures(1, &textures_loaded[i]->id);
				textures_loaded[i]->id = 0;
			}
			delete textures_loaded[i], textures_loaded[i] = nullptr;
		}
		textures_loaded.clear();
    }
public:
    /*  ����   */
    // ���ļ�����֧��ASSIMP��չ��ģ�ͣ��������ɵ�����洢������ʸ���С�
    void loadModel(string const &path)
	{

		is_loading_ = true;
        // ͨ��ASSIMP���ļ�
        Assimp::Importer * importer = new Assimp::Importer;
        const aiScene* scene = importer->ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // ������
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // �������0
        {
            cout << "����::ASSIMP:: " << importer->GetErrorString() << endl;
            return;
        }

        // �����ļ�·����Ŀ¼·��
        directory = path.substr(0, path.find_last_of('/'));

        // �Եݹ鷽ʽ����ASSIMP�ĸ��ڵ�

		processNode(scene->mRootNode, scene);
		is_loading_ = false;

		delete importer;
		importer = nullptr;

    }
	// ���ڴ����֧��ASSIMP��չ��ģ�ͣ��������ɵ�����洢������ʸ���С�
	void loadModel(const void* pBuffer,size_t pLength)
	{
		is_loading_ = true;
		// ͨ��ASSIMP���ļ�
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFileFromMemory(pBuffer, pLength, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// ������
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // �������0
		{
			cout << "����::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// �ڴ���ط�ʽ��ʱ����������;
		directory = "";

		// �Եݹ鷽ʽ����ASSIMP�ĸ��ڵ�
		processNode(scene->mRootNode, scene);
		is_loading_ = false;

	}

    // �Եݹ鷽ʽ����ڵ㡣 ����λ�ڽڵ㴦��ÿ���������񣬲������ӽڵ㣨����У����ظ��˹��̡�
    void processNode(aiNode *node, const aiScene *scene)
    {
        // ����λ�ڵ�ǰ�ڵ��ÿ������
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // �ڵ��������������������������е�ʵ�ʶ���
            // ���������������ݣ��ڵ�ֻ��Ϊ������֯�ı��涫������ڵ�֮��Ĺ�ϵ����
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // �����Ǵ�����������������еĻ��������ǻ�ݹ鴦��ÿ���ӽڵ�
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh* processMesh(aiMesh *mesh, const aiScene *scene)
    {
        // Ҫ��д������
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture*> textures;

        // ����ÿ������Ķ���
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
           // ��������һ��ռλ����������Ϊassimpʹ�����Լ��������࣬����ֱ��ת��Ϊglm��vec3�࣬�����������Ƚ����ݴ��ݸ����ռλ��glm :: vec3��
            gVector3f vector;
            // λ��
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // ����
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            // ��������
            if (mesh->mTextureCoords[0]) // �����Ƿ�����������ꣿ
            {
                gVector2f vec;
                // �������ɰ���8����ͬ���������ꡣ ��ˣ����Ǽ������ǲ���ʹ�ö�����Ծ��ж�����������ģ�ͣ�����������ǲ��õ�һ�����ϣ�0����
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = gVector2f(0.0f, 0.0f);
            // u����
			if (mesh->mTangents){
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;
            }
			else
				vertex.Tangent = gVector3f(1,0,0);
            // v����
			if (mesh->mBitangents)
			{
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vector;
            }
			else
				vertex.Bitangent = gVector3f(1, 0, 0);
            vertices.push_back(vertex);
        }
        //���ڱ���ÿ�������棨һ������һ�������ε����񣩲�������Ӧ�Ķ���������
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // ����������������������Ǵ洢������������
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // �ӹ�����
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        Material mat;
        mat.Ns = 10;
        mat.Ka = gVector4f(0.2);
        mat.Kd = gVector4f(0.8);
        mat.Ks = gVector4f(0);
        mat.Ke = gVector4f(0);
        bool hasTex = false;

        aiColor3D color;
        //��ȡmtl�ļ���������
        material->Get(AI_MATKEY_COLOR_AMBIENT, color);
        mat.Ka = gVector4f(color.r, color.g, color.b, 1.0);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        mat.Kd = gVector4f(color.r, color.g, color.b, 1.0);
        material->Get(AI_MATKEY_COLOR_SPECULAR, color);
        mat.Ks = gVector4f(color.r, color.g, color.b, 1.0);
        material->Get(AI_MATKEY_COLOR_EMISSIVE, color);
        mat.Ke = gVector4f(color.r, color.g, color.b, 1.0);
        material->Get(AI_MATKEY_SHININESS, color);
        mat.Ns = color.r;
        // ���Ǽ�����ɫ���еĲ���������Լ���� ÿ������������Ӧ����Ϊ'texture_diffuseN'������N�Ǵ�1��MAX_SAMPLER_NUMBER�����кš�
        //ͬ�����������������������ܽ᣺
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. ��������ͼ
        vector<Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. �߹���ͼ
		vector<Texture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3.������ͼ
		std::vector<Texture*> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. �߶���ͼ
		std::vector<Texture*> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        if (textures.size()>0)
        {
            hasTex = true;
        }

        // ���ش���ȡ���������ݴ������������ָ��
        return new Mesh(vertices, indices, textures, mat, hasTex);
    }

    // ���������͵����в������������δ�����������������
    // ������Ϣ��ΪTexture�ṹ���ء�
    vector<Texture*> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
    {
        vector<Texture*> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // ���֮ǰ�Ƿ��������������ǣ��������һ�ε�������������������
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j]->path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;// �Ѽ��ؾ�����ͬ�ļ�·��������������һ�����Ż�����
                }
            }
            if (!skip)
            {   // �����δ���������������

				Texture* texture = new Texture();
				string file_name_ = (this->directory + "/") + str.C_Str();
				//���������ʱӦ�������Ƿ���Ч;
				CreateTextureOtherThread(file_name_.c_str(), &(texture->id));
                texture->type = typeName;
				texture->path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  //����洢Ϊ����ģ�ͼ��ص�������ȷ�����ǲ�������ظ�����
            }
        }
        return textures;
	}

	bool Usable() const
	{
		return !is_loading_;
	}

    void AddMotionMatrix(const gMatrix4f& _matrix)
    {
        motion_matrixs_.push_back(_matrix);
    }

    vector<gMatrix4f>& GetMotionMatrixs()
    {
        return motion_matrixs_;
    }

};
