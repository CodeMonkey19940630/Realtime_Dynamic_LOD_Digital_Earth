#include "ModelingManager.h"
#include "io.h"
#include "imageLoad.h"
#include "modeling.h"
#pragma comment(lib,"assimp-vc120-mt.lib")

extern ModelingManager* model_manager_;

void CreateTextureOtherThread(char const * _texture_name, unsigned int * _id)
{
	emit model_manager_->signal_texture(QString::fromLocal8Bit(_texture_name), _id);
}

ModelingManager::ModelingManager(QObject *parent)
	: QThread(parent)
{
	abort = false;
	connect(this, &ModelingManager::signal_texture, this, &ModelingManager::slot_texture);
}


ModelingManager::~ModelingManager()
{
	abort = true;
	io_mutex.lock();
	condition.wakeOne();
	io_mutex.unlock();
}

bool ModelingManager::LoadModelingByExplain(ModelingExplain _explain)
{
	QMutexLocker locker(&io_mutex);
	if (map_name_modeling_.find(_explain.modeling_key_name_) != map_name_modeling_.end() || -1==_access(_explain.modeling_file_path_.c_str(), 0))
		return false;
	Modeling* _modeling = new Modeling();
	map_name_modeling_[_explain.modeling_key_name_] = _modeling;
	queue_modeling_file_.push_back({ _modeling,_explain.modeling_file_path_});

	if (!isRunning()) {
		start(LowPriority);
	}
	else
		condition.wakeOne();
	return true;
}

bool ModelingManager::UnloadModelingByName(std::string _key_name)
{
	QMutexLocker locker(&io_mutex);
	auto& iter = map_name_modeling_.find(_key_name);
	if (iter == map_name_modeling_.end())
		return false;
	else
	{
		delete iter->second, iter->second = nullptr;
		map_name_modeling_.erase(iter);
		return true;
	}
}

bool ModelingManager::UnloadAllModeling()
{
	QMutexLocker locker(&io_mutex);
	for (auto& iter : map_name_modeling_)
	{
		delete iter.second, iter.second = nullptr;
	}
	map_name_modeling_.clear();
	return true;
}

Modeling* ModelingManager::FindbyKeyName(const std::string _key_name)
{
	return map_name_modeling_[_key_name];
}

void ModelingManager::slot_texture(QString _texture_file, unsigned int * _id)
{
	*_id = CreateTexture(_texture_file.toLocal8Bit().data());
}

void ModelingManager::run()
{
	while (!abort)
	{
		io_mutex.lock();
		QQueue<QPair<Modeling*, std::string>> temp_modeling_;
		std::swap(temp_modeling_, queue_modeling_file_);
		io_mutex.unlock();
		for (const auto& iter : temp_modeling_)
		{
			iter.first->loadModel(iter.second);
		}
		io_mutex.lock();
		if (queue_modeling_file_.empty())
		{
			condition.wait(&io_mutex);
		}
		io_mutex.unlock();
	}
}
