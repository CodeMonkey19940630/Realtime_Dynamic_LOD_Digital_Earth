#pragma once

#include <assimp/Importer.hpp>        //assimp库头文件
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <iostream>
#include "Mesh.h"

//在其他线程创建对应纹理名称
extern void CreateTextureOtherThread(const char* fileName, unsigned int* _id);
class Modeling
{
public:
	//Assimp::Importer * importer;
    //存储到目前为止加载的所有纹理，优化以确保纹理不会被加载多次。
    vector<Texture*> textures_loaded;
    vector<Mesh*> meshes;
    string directory;
    string modeling_name_;
    bool gammaCorrection;
protected:
    vector<gMatrix4f> motion_matrixs_;
	bool			  is_loading_;

public:
    /*  函数  */
    Modeling()
    {
		is_loading_ = true;
    }
    // 构造函数,需要一个3D模型的文件路径
    Modeling(string const &path, bool gamma = false) : gammaCorrection(gamma)
	{
        loadModel(path);
	}
	// 构造函数,需要一个3D模型的文件内容
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
    /*  函数   */
    // 从文件加载支持ASSIMP扩展的模型，并将生成的网格存储在网格矢量中。
    void loadModel(string const &path)
	{

		is_loading_ = true;
        // 通过ASSIMP读文件
        Assimp::Importer * importer = new Assimp::Importer;
        const aiScene* scene = importer->ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // 检查错误
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // 如果不是0
        {
            cout << "错误::ASSIMP:: " << importer->GetErrorString() << endl;
            return;
        }

        // 检索文件路径的目录路径
        directory = path.substr(0, path.find_last_of('/'));

        // 以递归方式处理ASSIMP的根节点

		processNode(scene->mRootNode, scene);
		is_loading_ = false;

		delete importer;
		importer = nullptr;

    }
	// 从内存加载支持ASSIMP扩展的模型，并将生成的网格存储在网格矢量中。
	void loadModel(const void* pBuffer,size_t pLength)
	{
		is_loading_ = true;
		// 通过ASSIMP读文件
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFileFromMemory(pBuffer, pLength, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// 检查错误
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // 如果不是0
		{
			cout << "错误::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// 内存加载方式暂时不考虑纹理;
		directory = "";

		// 以递归方式处理ASSIMP的根节点
		processNode(scene->mRootNode, scene);
		is_loading_ = false;

	}

    // 以递归方式处理节点。 处理位于节点处的每个单独网格，并在其子节点（如果有）上重复此过程。
    void processNode(aiNode *node, const aiScene *scene)
    {
        // 处理位于当前节点的每个网格
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // 节点对象仅包含索引用来索引场景中的实际对象。
            // 场景包含所有数据，节点只是为了有组织的保存东西（如节点之间的关系）。
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // 在我们处理完所有网格（如果有的话）后，我们会递归处理每个子节点
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh* processMesh(aiMesh *mesh, const aiScene *scene)
    {
        // 要填写的数据
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture*> textures;

        // 遍历每个网格的顶点
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
           // 我们声明一个占位符向量，因为assimp使用它自己的向量类，它不直接转换为glm的vec3类，所以我们首先将数据传递给这个占位符glm :: vec3。
            gVector3f vector;
            // 位置
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // 法线
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            // 纹理坐标
            if (mesh->mTextureCoords[0]) // 网格是否包含纹理坐标？
            {
                gVector2f vec;
                // 顶点最多可包含8个不同的纹理坐标。 因此，我们假设我们不会使用顶点可以具有多个纹理坐标的模型，因此我们总是采用第一个集合（0）。
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = gVector2f(0.0f, 0.0f);
            // u向量
			if (mesh->mTangents){
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;
            }
			else
				vertex.Tangent = gVector3f(1,0,0);
            // v向量
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
        //现在遍历每个网格面（一个面是一个三角形的网格）并检索相应的顶点索引。
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // 检索面的所有索引并将它们存储在索引向量中
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // 加工材料
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        Material mat;
        mat.Ns = 10;
        mat.Ka = gVector4f(0.2);
        mat.Kd = gVector4f(0.8);
        mat.Ks = gVector4f(0);
        mat.Ke = gVector4f(0);
        bool hasTex = false;

        aiColor3D color;
        //读取mtl文件顶点数据
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
        // 我们假设着色器中的采样器名称约定。 每个漫反射纹理应命名为'texture_diffuseN'，其中N是从1到MAX_SAMPLER_NUMBER的序列号。
        //同样适用于其他纹理，如下列总结：
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. 漫反射贴图
        vector<Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. 高光贴图
		vector<Texture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3.法线贴图
		std::vector<Texture*> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. 高度贴图
		std::vector<Texture*> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        if (textures.size()>0)
        {
            hasTex = true;
        }

        // 返回从提取的网格数据创建的网格对象指针
        return new Mesh(vertices, indices, textures, mat, hasTex);
    }

    // 检查给定类型的所有材质纹理，如果尚未加载纹理，则加载纹理。
    // 所需信息作为Texture结构返回。
    vector<Texture*> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
    {
        vector<Texture*> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // 检查之前是否加载了纹理，如果是，则继续下一次迭代：跳过加载新纹理
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j]->path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;// 已加载具有相同文件路径的纹理，继续下一个（优化）。
                }
            }
            if (!skip)
            {   // 如果尚未加载纹理，请加载它

				Texture* texture = new Texture();
				string file_name_ = (this->directory + "/") + str.C_Str();
				//纹理加载完时应当返回是否有效;
				CreateTextureOtherThread(file_name_.c_str(), &(texture->id));
                texture->type = typeName;
				texture->path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  //将其存储为整个模型加载的纹理，以确保我们不会加载重复纹理。
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
