#include "ModelRenderer.h"
#include "../../../include/glRender/FzNode/FzModelNode.h"
#include <QFile>
#include "ModelingManager.h"
#include "modeling.h"
extern ModelingManager* model_manager_;

ModelRenderer::ModelRenderer()
{
	widget_ = nullptr;
	f_ = nullptr;
	if (nullptr == model_manager_)
	{
		model_manager_ = new ModelingManager(widget_);
	}
}

ModelRenderer::~ModelRenderer()
{
	modelProgram.removeAllShaders();
	delete[] vert_memery_;
	delete[] frag_memery_;
	widget_ = nullptr;
	f_ = nullptr;
}

const char* ModelRenderer::RenderObjectName() const
{
	return ModelNodeObject;
}

bool ModelRenderer::Init(QOpenGLWidget* _widget, QOpenGLFunctions_3_3_Core* _function)
{
	widget_ = _widget;
	f_ = _function;
	QFile vert_file(":/shader/model.vert");
	QFile frag_file(":/shader/model.frag");
	if (vert_file.open(QIODevice::ReadOnly)){
		int length = vert_file.size();
		vert_memery_ = new char[length + 1];
		memset(vert_memery_, 0, length + 1);
		vert_file.read(vert_memery_, length);
		vert_file.close();
	}
	if (frag_file.open(QIODevice::ReadOnly)){
		int length = frag_file.size();
		frag_memery_ = new char[length + 1];
		memset(frag_memery_, 0, length + 1);
		frag_file.read(frag_memery_, length);
		frag_file.close();
	}

	bool success = modelProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, vert_memery_);
	if (!success) {
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << modelProgram.log();
		return success;
	}
	success = modelProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, frag_memery_);
	if (!success) {
		QString str_log = modelProgram.log();
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << str_log;
		return success;
	}
	success = modelProgram.link();
	if (!success) {
		qDebug() << "shaderProgram link failed!" << modelProgram.log();
		return success;
	}
	return success;
}

void ModelRenderer::Render(FzSituationSystem* _system, std::vector<FzNode*> _node_vector)
{
	if (_node_vector.empty())
		return;
	map<string, pair<Modeling*, vector<FzModelNode*>>> map_name_vec_node;
	for (size_t i = 0; i < _node_vector.size(); i++)
	{
		FzModelNode* model_node = static_cast<FzModelNode*>(_node_vector[i]);
		string model_name = model_node->GetModelingName();
		auto iter = map_name_vec_node.find(model_node->GetModelingName());
		if (iter == map_name_vec_node.end())
		{
			Modeling* modeling_ = model_manager_->FindbyKeyName(model_name);
			pair<Modeling*, vector<FzModelNode*>> tmp(modeling_, {});
			iter = map_name_vec_node.insert({ model_name, tmp }).first;
		}
		iter->second.second.push_back(model_node);
	}
	f_->glDisable(GL_CULL_FACE);
	static bool test = false;
	if (test)
		f_->glPolygonMode(GL_BACK, GL_LINE);
	f_->glEnable(GL_DEPTH_TEST);
	f_->glEnable(GL_BLEND);
	f_->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FzCamera* camera = _system->Camera();
	gMatrix4f world;
	gMatrix4f view = camera->getViewMatrix4();
	gMatrix4f prjection = _system->getProjectionMatrix4();

	modelProgram.bind();
	f_->glUniformMatrix4fv(modelProgram.uniformLocation("view"), 1, GL_FALSE, &view[0][0]);
	f_->glUniformMatrix4fv(modelProgram.uniformLocation("projection"), 1, GL_FALSE, &prjection[0][0]);

	for (auto& iter : map_name_vec_node)
	{
		Modeling*& modeling = iter.second.first;
		if (modeling == nullptr||!modeling->Usable())
			continue;
		vector<FzModelNode*>& model_nodes = iter.second.second;
		vector<gMatrix4f> models_mtr_vec(model_nodes.size());
		for (size_t i = 0; i < model_nodes.size(); i++)
		{
			models_mtr_vec[i] = Matrix4Scaling(model_nodes[i]->GetScale())*MatrixUpOrder4(model_nodes[i]->GetBody2World(), model_nodes[i]->GetCartesianPos()-camera->getCameraPosition());
		}
		for (size_t j = 0; j < modeling->meshes.size(); j++)
		{
			const Mesh& msh = *(modeling->meshes[j]);
			// 绑定适当的纹理
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;
			for (unsigned int i = 0; i < msh.textures.size(); i++)
			{
				// 获取纹理编号（diffuse_textureN中的N）
				string number;
				string name = msh.textures[i]->type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++);
				else if (name == "texture_normal")
					number = std::to_string(normalNr++);
				else if (name == "texture_height")
					number = std::to_string(heightNr++);
				// 现在将采样器设置为正确的纹理单元
				f_->glUniform1i(modelProgram.uniformLocation((name + number).c_str()), i);
				// 绑定前激活适当的纹理单元
				f_->glActiveTexture(GL_TEXTURE0 + i);
				// 最后绑定纹理
				f_->glBindTexture(GL_TEXTURE_2D, msh.textures[i]->id);
			}

			f_->glUniform1i(modelProgram.uniformLocation("hasTex"), msh.hasTexture);

			f_->glUniform3f(modelProgram.uniformLocation("ambientLight.intensities"), 0.3, 0.3, 0.3);

			static	gVector3f light = camera->getCameraPosition().GetNormalize();
			f_->glUniform3f(modelProgram.uniformLocation("directionalLight.direction"), light.x, light.y, light.z);
			f_->glUniform3f(modelProgram.uniformLocation("directionalLight.intensities"), 0.8, 0.8, 0.8);
			f_->glUniform3f(modelProgram.uniformLocation("eyePosition"), 0, 0, 0);
			f_->glUniform3fv(modelProgram.uniformLocation("material.ka"), 1, (GLfloat*)&msh.mats.Ka.x);
			f_->glUniform3fv(modelProgram.uniformLocation("material.kd"), 1, (GLfloat*)&msh.mats.Kd.x);
			f_->glUniform3fv(modelProgram.uniformLocation("material.ke"), 1, (GLfloat*)&msh.mats.Ke.x);
			f_->glUniform3fv(modelProgram.uniformLocation("material.ks"), 1, (GLfloat*)&msh.mats.Ks.x);
			f_->glUniform1f(modelProgram.uniformLocation("material.Ns"), msh.mats.Ns);

			f_->glUniform4fv(modelProgram.uniformLocation("aAmbient"), 1, &(msh.mats.Ka.x));
			f_->glUniform4fv(modelProgram.uniformLocation("aDiffuse"), 1, &(msh.mats.Kd.x));
			f_->glUniform4fv(modelProgram.uniformLocation("aSpecular"), 1, &(msh.mats.Ks.x));
			// 设置顶点属性指针
			f_->glEnableVertexAttribArray(0);
			f_->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), msh.vertPos);
			f_->glEnableVertexAttribArray(1);
			f_->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), msh.vertNor);
			f_->glEnableVertexAttribArray(2);
			f_->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), msh.vertTex);

			// 按vec4偏移
			f_->glEnableVertexAttribArray(3);
			f_->glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(gMatrix4f), &models_mtr_vec.front()[0]);
			f_->glEnableVertexAttribArray(4);
			f_->glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(gMatrix4f), &models_mtr_vec.front()[1]);
			f_->glEnableVertexAttribArray(5);
			f_->glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(gMatrix4f), &models_mtr_vec.front()[2]);
			f_->glEnableVertexAttribArray(6);
			f_->glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(gMatrix4f), &models_mtr_vec.front()[3]);
			f_->glVertexAttribDivisor(3, 1);
			f_->glVertexAttribDivisor(4, 1);
			f_->glVertexAttribDivisor(5, 1);
			f_->glVertexAttribDivisor(6, 1);
			f_->glDrawElementsInstanced(GL_TRIANGLES, msh.indices.size(), GL_UNSIGNED_INT, &msh.indices[0], models_mtr_vec.size());
			f_->glDisableVertexAttribArray(0);
			f_->glDisableVertexAttribArray(1);
			f_->glDisableVertexAttribArray(2);
			f_->glDisableVertexAttribArray(3);
			f_->glDisableVertexAttribArray(4);
			f_->glDisableVertexAttribArray(5);
			f_->glDisableVertexAttribArray(6);
			f_->glVertexAttribDivisor(3, 0);
			f_->glVertexAttribDivisor(4, 0);
			f_->glVertexAttribDivisor(5, 0);
			f_->glVertexAttribDivisor(6, 0);
		}
	}
	modelProgram.release();
}

void ModelRenderer::Uninit()
{

}
