#pragma once
#include <QMutex>
#include <QThread>
#include <QQueue>
#include <QWaitCondition>
#include <memory>
#include "../../../include/glRender/FzNode/FzModelNode.h"

class Modeling;
class ModelingManager : public QThread
{
	Q_OBJECT

public:
	ModelingManager(QObject *parent = 0);
	~ModelingManager();

	bool LoadModelingByExplain(ModelingExplain _explain);
	bool UnloadModelingByName(std::string _key_name);
	bool UnloadAllModeling();
	Modeling* FindbyKeyName(const std::string _key_name);
Q_SIGNALS:
	void signal_texture(QString _texture_file, unsigned int * _id);
public Q_SLOTS:
	void slot_texture(QString _texture_file, unsigned int * _id);
protected:
	void run() override;

protected:
	bool abort;
	QQueue<QPair<Modeling*,std::string>> queue_modeling_file_;
	QMutex io_mutex;
	QWaitCondition condition;
	std::map<std::string, Modeling*> map_name_modeling_;
};
