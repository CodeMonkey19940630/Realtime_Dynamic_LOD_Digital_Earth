#include "TerrainRenderer.h"
#include "../../../include/glRender/FzNode/FzTerrainNode.h"
#include <QFile>
#include "FzQuadTile.h"
#include "TileDataManager.h"

extern TileDataManager* tile_manager_;
TerrainRenderer::TerrainRenderer()
{
	widget_ = nullptr;
	f_ = nullptr;
	if (tile_manager_ == nullptr)
		tile_manager_ = new TileDataManager();
}

TerrainRenderer::~TerrainRenderer()
{
	surfProgram.removeAllShaders();
	delete[] vert_memery_;
	delete[] frag_memery_;
	widget_ = nullptr;
	f_ = nullptr;
}

const char* TerrainRenderer::RenderObjectName() const
{
	return TerrainNodeObject;
}

bool TerrainRenderer::Init(QOpenGLWidget* _widget, QOpenGLFunctions_3_3_Core* _function)
{
	widget_ = _widget;
	f_ = _function;
	QFile vert_file(":/shader/terrain.vert");
	QFile frag_file(":/shader/terrain.frag");
	if (vert_file.open(QIODevice::ReadOnly)){
		int length = vert_file.size();
		vert_memery_ = new char[length + 1];
		memset(vert_memery_, 0, length + 1);
		vert_file.read(vert_memery_,length);
		vert_file.close();
	}
	if (frag_file.open(QIODevice::ReadOnly)){
		int length = frag_file.size();
		frag_memery_ = new char[length + 1];
		memset(frag_memery_, 0, length + 1);
		frag_file.read(frag_memery_, length);
		frag_file.close();
	}

	bool success = surfProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, vert_memery_);
	if (!success) {
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << surfProgram.log();
		return success;
	}
	success = surfProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, frag_memery_);
	if (!success) {
		QString str_log = surfProgram.log();
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << str_log;
		return success;
	}
	success = surfProgram.link();
	if (!success) {
		qDebug() << "shaderProgram link failed!" << surfProgram.log();
		return success;
	}
	return success;
}

void TerrainRenderer::Render(FzSituationSystem* _system, std::vector<FzNode *> _node_vector)
{
	if (_node_vector.empty())
		return;
	FzTerrainNode* node = static_cast<FzTerrainNode*>(_node_vector.front());
	//地形
	//剔除背面
	f_->glDisable(GL_CULL_FACE);
	static bool test = false;
	//线框方式绘制测试
	if (test)
	{
		f_->glPolygonMode(GL_FRONT, GL_POINT);
		f_->glPolygonMode(GL_BACK, GL_LINE);
	}
	//深度测试
	f_->glEnable(GL_DEPTH_TEST);
	//透明物体颜色混合
	f_->glEnable(GL_BLEND);
	f_->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	const auto& vec_tile = node->GetVisibleQuadTile();
	FzCamera* camera = _system->Camera();
	//世界矩阵
	gMatrix4f world;
	//观察矩阵
	gMatrix4f view = camera->getViewMatrix4();
	//投影矩阵
	gMatrix4f prjection = _system->getProjectionMatrix4();

	surfProgram.bind();
	f_->glUniformMatrix4fv(surfProgram.uniformLocation("u_viewMatrix"), 1, GL_TRUE, &view[0][0]);
	f_->glUniformMatrix4fv(surfProgram.uniformLocation("u_projectionMatrix"), 1, GL_TRUE, &prjection[0][0]);
 	for (const auto& var:vec_tile)
 	{
#if 0
		static int view_level = 3;
		if (var->GetTilePos().level != view_level)
			continue;
#endif
 		//世界矩阵对于每个地块都不同
 		gVector3d transfrom_word = var->GetCenterPos() - camera->getCameraPosition();
 		world[3]= gVector4f(transfrom_word, 1);
 		f_->glUniformMatrix4fv(surfProgram.uniformLocation("u_worldMatrix"), 1, GL_TRUE, &world[0][0]);
 
 		//纹理设置
 		f_->glActiveTexture(GL_TEXTURE0);
 		f_->glBindTexture(GL_TEXTURE_2D, var->GetTextureId());
 		f_->glUniform1i(surfProgram.uniformLocation("shaderTexture"), 0);
 
 
 		const GLfloat* pos_ptr_ = nullptr, *tex_ptr_ = nullptr;
 		unsigned index_count = 0;
 		unsigned short* index_ptr_ = nullptr;
 
 		pos_ptr_ = var->GetPosVertexPtr()->c_ptr();
 		tex_ptr_ = var->GetTexVertexPtr()->c_ptr();
 		index_count = var->GetIndexCount();
 		index_ptr_ = (unsigned short*)var->GetIndexPtr();
 
 		// 设置坐标顶点
 		f_->glEnableVertexAttribArray(0);
 		f_->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), pos_ptr_);
 		// 设置纹理顶点
 		f_->glEnableVertexAttribArray(1);
		if (tex_ptr_)
			f_->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), tex_ptr_);
 		// 按索引绘制;
		f_->glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_SHORT, index_ptr_);
 		// 清除地块绘制资源
 		f_->glBindTexture(GL_TEXTURE_2D, 0);
 		f_->glDisableVertexAttribArray(0);
 		f_->glDisableVertexAttribArray(1);
 	}
	surfProgram.release();
}

void TerrainRenderer::Uninit()
{
	surfProgram.removeAllShaders();
}
