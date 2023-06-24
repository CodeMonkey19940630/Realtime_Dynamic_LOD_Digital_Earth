#pragma once
#include "situationwidget_global.h"
#include <vector>
#include <string>
#include <map>
#include "../../../include/glRender/FzRenderProgramme/IRenderer.h"


typedef IRenderer* (*func_CreateRenderer)();
class SITUATIONWIDGET_EXPORT RendererMgr
{
protected:
	RendererMgr();
	static RendererMgr* render_mgr_;
public:
	static RendererMgr* getInstance()
	{
		if (render_mgr_ == nullptr)
			render_mgr_ = new RendererMgr();
		return render_mgr_;
	}
	~RendererMgr();

	//加载渲染程序
	void LoadRenderProgramme(const std::string & _file_name);

	//创建已加载程序的绘制器
	std::vector<IRenderer*> CreateLoadedRenderer();

private:
	std::vector<func_CreateRenderer>				vec_renderer_func_;

};

