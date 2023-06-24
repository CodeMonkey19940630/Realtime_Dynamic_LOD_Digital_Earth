#pragma once
#include <QMutex>
#include <QThread>
#include <QQueue>
#include <QWaitCondition>
#include <memory>
#include "FzQuadTile.h"

class TileDataManager : public QThread
{
	Q_OBJECT

public:
	TileDataManager(QObject *parent = 0);
	~TileDataManager();

	bool Loadable(const TILEPOS& _pos);
	void LoadTile(FzQuadTile* _tile);
	void LoadTileChoke(FzQuadTile* _tile);

	const std::vector<unsigned short>* GetTileIndexbyDirect(const TILEDIRECT& _direct) const;

	std::string dem_dir_path_;
	std::string picture_dir_path_;
	std::string vector_dir_path_;
Q_SIGNALS:
	void signal_load_tile_data(FzQuadTile* _tile, std::shared_ptr<TILEDATA>* _tile_data);
public Q_SLOTS:
	void slot_load_tile_data(FzQuadTile* _tile, std::shared_ptr<TILEDATA>* _tile_data);
protected:
	void run() override;

	char* SpliceTilePos(const TILEPOS* _pos, const char* _root_dir, const char* _suffix, char _out_str[512]);

	std::shared_ptr<TILEDATA> GetTileData(const TILEPOS& _pos);
protected:
	bool abort;
	QQueue<FzQuadTile*> queue_tile_;
	QMutex io_mutex;
	QMutex map_mutex;
	QWaitCondition condition;

	std::map<TILEPOS, std::shared_ptr<TILEDATA>> map_data_;
	/*复用数据*/
	// 每条边的点数量
	const static int sidepos_count = 9;
	// 总顶点数量
	const static int vertexpos_count = sidepos_count*sidepos_count;
	// 沿边所成线段的数量
	const static int sidesegment_count = sidepos_count - 1;
	// 纹理坐标
	gVector2f tex[sidepos_count * sidepos_count];
	// 索引数组
	std::vector<std::vector<unsigned short>> dirct_index_;
};

