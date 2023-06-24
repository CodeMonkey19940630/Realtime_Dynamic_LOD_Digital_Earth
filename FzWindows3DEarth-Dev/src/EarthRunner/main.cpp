#include <QtWidgets/QApplication>
#include <QFile>
#include <QThread>
#include "../glrender/src_SituationWidget/situationwidget.h"
#include "../glrender/src_SituationWidget/RendererMgr.h"

#include <memory>
#include "../../../include/glRender/FzNode/FzTerrainNode.h"

#include "EnityNode.h"
#pragma comment(lib,"FzNode.lib")
#pragma comment(lib,"SituationWidget.lib")
#pragma comment(lib,"TerrainRenderer.lib")
#pragma comment(lib,"ModelRenderer.lib")
#pragma comment(lib,"LabelRenderer.lib")

template <typename T>
const T* CopyContinuousRect(const T* _data, gVector2i _pixel = (9, 9), gVector2d _lt_pos = (0, 0), gVector2d _rect = (1, 1))
{
	T* des_ = new T[_pixel.x*_pixel.y];
	gVector2d interval_src(1.0 / (_pixel.x - 1), 1.0 / (_pixel.y - 1));
	gVector2d pos2cr((_pixel.x - 1), (_pixel.y - 1));
	gVector2d interval_des(_rect.x / (_pixel.x - 1), _rect.y / (_pixel.y - 1));

	for (size_t i = 0; i < _pixel.y; i++)
	{
		for (size_t j = 0; j < _pixel.x; j++)
		{
			std::pair<T*, double> h_average[4];
			gVector2d pos_ = _lt_pos + interval_des*gVector2d(j, i);
			gVector2i colrow = pos_*pos2cr;
			gVector2d division_ = (pos_ - colrow*interval_src)*pos2cr;
			T* base_ = _data + colrow.y*_pixel.x + colrow.x;
			h_average[0]->first = base_;
			h_average[0]->second = (1 - division_.x)*(1 - division_.y);
			if (colrow.x + 1 < _pixel.x)
				h_average[1]->first = base_ + 1;
			else
				h_average[1]->first = nullptr;
			h_average[1]->second = division_.x*(1 - division_.y);
			if (colrow.y + 1 < _pixel.y)
				h_average[2]->first = base_ + _pixel.x;
			else
				h_average[2]->first = nullptr;
			h_average[2]->second = (1 - division_.x)*division_.y;
			if (h_average[1]->first != nullptr && h_average[2]->first != nullptr)
				h_average[2]->first = base_ + 1 + _pixel.x;
			else
				h_average[3] = first = nullptr;
			double weighted = 0;
			for (size_t k = 0; k < 4; k++)
			{
				if (h_average[k]->first != nullptr)
				{
					des_[i*_pixel.y + j] += *h_average[k]->first*h_average[k]->second;
					weighted += h_average[k]->second;
				}
				if (k == 3 && weighted != 1)
					des_[i*_pixel.y + j] = des_[i*_pixel.y + j] / weighted;
			}
		}
	}
}

#if 0
template <typename T>
const T* CopyDiscreteRect(const T* _data, gVector2i _pixel = (9, 9), bool _transpose = false, gVector2d _lt_pos = (0, 0), gVector2d _rect = (1, 1), T _default)
{
	T* des_ = new T[_pixel.x*_pixel.y];
	gVector2d interval_src(1.0 / (_pixel.x - 1), 1.0 / (_pixel.y - 1));
	gVector2d interval_des(_rect.x / (_pixel.x - 1), _rect.y / (_pixel.y - 1));
	if (_transpose == false)
	{
		for (size_t i = 0; i < _pixel.y; i++)
		{
			for (size_t j = 0; j < _pixel.x; j++)
			{
				std::vector<T*>
			}
		}
	}
	else
	{
	}
}
#endif

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	gVector4c* widget = new gVector4c[3000000];
	delete widget->c_ptr();
	widget = nullptr;
	std::shared_ptr<double> t;
	auto load_mgr = RendererMgr::getInstance();
	load_mgr->LoadRenderProgramme((QApplication::applicationDirPath() + "/../GLRender/TerrainRenderer.dll").toLocal8Bit().data());
	load_mgr->LoadRenderProgramme((QApplication::applicationDirPath() + "/../GLRender/ModelRenderer.dll").toLocal8Bit().data());
	load_mgr->LoadRenderProgramme((QApplication::applicationDirPath() + "/../GLRender/LabelRenderer.dll").toLocal8Bit().data());
	SituationWidget _widget;

	ModelingExplain model_explain("J-20", (std::string)QCoreApplication::applicationDirPath().toLocal8Bit().data() + "/../../res/model/¼ß-20Õ½¶·»ú/¼ß20.obj");
	FzModelNode::LoadModeling(model_explain);

	FzTerrainNode*					terrain_;
	terrain_ = new FzTerrainNode();
	terrain_->SetParent(&_widget);
	if (terrain_) {
		QFile data_file(QCoreApplication::applicationDirPath() + "/../../config/GlRender/map.txt");
		if (data_file.open(QIODevice::ReadOnly)){
			QString Date_dir;
			Date_dir = data_file.readAll();
			terrain_->SetMapDataDir(Date_dir.toLocal8Bit().data());
		}
	}
//	EntityNode				entity_;
//	entity_.SetParent(&_widget);
//	QNodeTimer timer(nullptr);
//	timer.connectNode(&entity_);
	_widget.show();
	return a.exec();
}
