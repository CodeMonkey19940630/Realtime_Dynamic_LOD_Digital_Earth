#include "TileDataManager.h"
#include "FzQuadTile.h"
#include <stb_image.h>
#include <io.h>

//_row，_col是二维网格左上角的行列号，_horizontal标识连在一起的三角形是作为行还是列，_col_count是列的数量
void IndexVecAddFace(int _row, int _col, const bool& _horizontal, const int& _col_count, std::vector<unsigned short>& _vec)
{
	//下图左上角为0行0列，中心为1行1列，所有的偶数行列所形成的三角形列表都与之相似。
	// |\------+------/|
	// |  \  1 | 2  /  |
	// |    \  |  /	   |
	// | 0    \|/ 	 3 |
	// +-------+-------+
	// | 2    /|\    1 |
	// |    /  |  \	   |
	// |  /  3 |  0 \  |
	// |/------+------\|
	//三角形根据构成直角边的两点相对位置不同分为0,1,2,3四中类别，根据左上角行列号奇偶相应也有四种组合方式。
	//索引顺序都是从左上角出发，顺时针连接点
	int key = (_row % 2 != _col % 2) * 2 + (_col % 2 != (int)_horizontal);
	//(_row % 2 == _col % 2) && (_col % 2 == _horizontal);//0
	//(_row % 2 == _col % 2) && (_col % 2 != _horizontal);//1
	//(_row % 2 != _col % 2) && (_col % 2 == _horizontal);//2
	//(_row % 2 != _col % 2) && (_col % 2 != _horizontal);//3
	switch (key)
	{
	case 0:
		_vec.push_back(_col + _row * _col_count);
		_vec.push_back(_col + 1 + (_row + 1) * _col_count);
		_vec.push_back(_col + (_row + 1) * _col_count);
		break;
	case 1:
		_vec.push_back(_col + _row * _col_count);
		_vec.push_back(_col + 1 + _row * _col_count);
		_vec.push_back(_col + 1 + (_row + 1) * _col_count);
		break; 
	case 2:
		_vec.push_back(_col + _row * _col_count);
		_vec.push_back(_col + 1 + _row * _col_count);
		_vec.push_back(_col + (_row + 1)*_col_count);
		break;
	case 3:
		_vec.push_back(_col + 1 + _row * _col_count);
		_vec.push_back(_col + 1 + (_row + 1) * _col_count);
		_vec.push_back(_col + (_row + 1) * _col_count);
		break;
	default:
		break;
	}
}

//_row，_col是二维网格左上角的行列号，_horizontal标识连在一起的三角形是作为行还是列，_col_count是列的数量
template <class T>
void IndexVecAddFace(int _row, int _col, const bool& _horizontal, const int& _col_count, T* _arr)
{
	//下图左上角为0行0列，中心为1行1列，所有的偶数行列所形成的三角形列表都与之相似。
	// |\------+------/|
	// |  \  1 | 2  /  |
	// |    \  |  /	   |
	// | 0    \|/ 	 3 |
	// +-------+-------+
	// | 2    /|\    1 |
	// |    /  |  \	   |
	// |  /  3 |  0 \  |
	// |/------+------\|
	//三角形根据构成直角边的两点相对位置不同分为0,1,2,3四中类别，根据左上角行列号奇偶相应也有四种组合方式。
	//索引顺序都是从左上角出发，顺时针连接点
	int key = (_row % 2 != _col % 2) * 2 + (_col % 2 != (int)_horizontal);
	//(_row % 2 == _col % 2) && (_col % 2 == _horizontal);//0
	//(_row % 2 == _col % 2) && (_col % 2 != _horizontal);//1
	//(_row % 2 != _col % 2) && (_col % 2 == _horizontal);//2
	//(_row % 2 != _col % 2) && (_col % 2 != _horizontal);//3
	switch (key)
	{
	case 0:
		_arr[0] = (_col + _row * _col_count);
		_arr[1] = (_col + 1 + (_row + 1) * _col_count);
		_arr[2] = (_col + (_row + 1) * _col_count);
		break;
	case 1:
		_arr[0] = (_col + _row * _col_count);
		_arr[1] = (_col + 1 + _row * _col_count);
		_arr[2] = (_col + 1 + (_row + 1) * _col_count);
		break;
	case 2:
		_arr[0] = (_col + _row * _col_count);
		_arr[1] = (_col + 1 + _row * _col_count);
		_arr[2] = (_col + (_row + 1)*_col_count);
		break;
	case 3:
		_arr[0] = (_col + 1 + _row * _col_count);
		_arr[1] = (_col + 1 + (_row + 1) * _col_count);
		_arr[2] = (_col + (_row + 1) * _col_count);
		break;
	default:
		break;
	}
}
TileDataManager::TileDataManager(QObject *parent)
	: QThread(parent)
{
	abort = false;
	connect(this, &TileDataManager::signal_load_tile_data, this, &TileDataManager::slot_load_tile_data);
	for (size_t i = 0; i < vertexpos_count; i++)
	{//纹理坐标数组
		int row = i / sidepos_count;
		int col = i % sidepos_count;
		tex[i].x = col / (sidepos_count - 1.0);
		tex[i].y = row / (sidepos_count - 1.0);
	}
	//四角区域核心三角形索引
	std::vector<unsigned short> quad_core_index_[4];
	//各边索引数量
	int side_index_count = 0, side_space_index_count = 0;
	//四边连续三角形索引
	std::map<unsigned char, std::vector<unsigned short>> side_index_;
	//四边间隔三角形索引
	std::map<unsigned char, std::vector<unsigned short>> side_space_index_;
	int half_sscount = sidesegment_count / 2;
	for (size_t order = 0; order < 4; order++)
	{
		// order与坐标方位
		// 0 | 2
		// --+--
		// 1 | 3
		quad_core_index_[order].clear();
		for (size_t i = 0; i < half_sscount*half_sscount; i++)
		{
			int row = i / half_sscount + (order % 2)*half_sscount;//order % 2?北:南
			int col = i % half_sscount + (order / 2)*half_sscount;//order / 2?西:东
			if (row != 0 && row != sidesegment_count-1)//行靠边不留行
			{
				IndexVecAddFace(row, col, true, sidepos_count, quad_core_index_[order]);
			}
			if (col != 0 && col != sidesegment_count - 1)//列靠边不留列
			{
				IndexVecAddFace(row, col, false, sidepos_count, quad_core_index_[order]);
			}
		}
	}
	//没有间隔的邻边索引
	//\-|-/\-|-/\-|-/\-|-/
	// \|/  \|/  \|/  \|/ 
	for (size_t i = 0; i < sidesegment_count; i++)
	{
		side_index_count += 3;
		IndexVecAddFace(i, 0, false, sidepos_count, side_index_[West]);
		IndexVecAddFace(0, i, true, sidepos_count, side_index_[North]);
		IndexVecAddFace(sidesegment_count-1, i, true, sidepos_count, side_index_[South]);
		IndexVecAddFace(i, sidesegment_count-1, false, sidepos_count, side_index_[East]);
	}
	//有间隔的邻边索引
	//\---/\---/\---/\---/
	// \+/  \+/  \+/  \+/
	for (size_t i = 0; i < sidesegment_count; i+=2)
	{
		side_space_index_count += 3;
		unsigned short temp_index[3];
		//西
		IndexVecAddFace(i, 0, false, sidepos_count, temp_index);
		temp_index[2] += sidepos_count;//从(row+1,col)下移一行
		side_space_index_[West].insert(side_space_index_[West].end(), temp_index, temp_index + 3);
		//北
		IndexVecAddFace(0, i, true, sidepos_count, temp_index);
		temp_index[1] += 1;//从(row,col+1)右移一列
		side_space_index_[North].insert(side_space_index_[North].end(), temp_index, temp_index + 3);
		//南
		IndexVecAddFace(sidesegment_count -1, i, true, sidepos_count, temp_index);
		temp_index[1] += 1;//从(row+1,col+1)右移一列
		side_space_index_[South].insert(side_space_index_[South].end(), temp_index, temp_index + 3);
		//东
		IndexVecAddFace(i, sidesegment_count -1, false, sidepos_count, temp_index);
		temp_index[1] += sidepos_count;//从(row+1,col+1)下移一行
		side_space_index_[East].insert(side_space_index_[East].end(), temp_index, temp_index + 3);
	}
	//按四角区域是否存在,相邻地块是否存在,组成256个索引数组
	dirct_index_.resize(256);
	for (size_t i = 0; i < 256; i++)
	{
		if (i&TILEDIRECT::WN){//西北角索引
			dirct_index_[i].insert(dirct_index_[i].end(), quad_core_index_[0].begin(), quad_core_index_[0].end());
			if (i&TILEDIRECT::West)//西边靠北的一半索引
				dirct_index_[i].insert(dirct_index_[i].end(), &side_index_[West][0], &side_index_[West][0] + side_index_count / 2);//无间隔索引
			else
				dirct_index_[i].insert(dirct_index_[i].end(), &side_space_index_[West][0], &side_space_index_[West][0] + side_space_index_count / 2);//有间隔索引
			if (i&TILEDIRECT::North)//北边靠西的一半索引
				dirct_index_[i].insert(dirct_index_[i].end(), &side_index_[North][0], &side_index_[North][0] + side_index_count / 2);
			else
				dirct_index_[i].insert(dirct_index_[i].end(), &side_space_index_[North][0], &side_space_index_[North][0] + side_space_index_count / 2);
		}
		if (i&TILEDIRECT::WS){//西南角索引
			dirct_index_[i].insert(dirct_index_[i].end(), quad_core_index_[1].begin(), quad_core_index_[1].end());
			if (i&TILEDIRECT::West)
				dirct_index_[i].insert(dirct_index_[i].end(), &side_index_[West][0] + side_index_count / 2, &side_index_[West][0] + side_index_count);
			else
				dirct_index_[i].insert(dirct_index_[i].end(), &side_space_index_[West][0] + side_space_index_count / 2, &side_space_index_[West][0] + side_space_index_count);
			if (i&TILEDIRECT::South)
				dirct_index_[i].insert(dirct_index_[i].end(), &side_index_[South][0], &side_index_[South][0] + side_index_count / 2);
			else
				dirct_index_[i].insert(dirct_index_[i].end(), &side_space_index_[South][0], &side_space_index_[South][0] + side_space_index_count / 2);
		}
		if (i&TILEDIRECT::EN){//东北角索引
			dirct_index_[i].insert(dirct_index_[i].end(), quad_core_index_[2].begin(), quad_core_index_[2].end());
			if (i&TILEDIRECT::East)
				dirct_index_[i].insert(dirct_index_[i].end(), &side_index_[East][0], &side_index_[East][0] + side_index_count / 2);
			else
				dirct_index_[i].insert(dirct_index_[i].end(), &side_space_index_[East][0], &side_space_index_[East][0] + side_space_index_count / 2);
			if (i&TILEDIRECT::North)
				dirct_index_[i].insert(dirct_index_[i].end(), &side_index_[North][0] + side_index_count / 2, &side_index_[North][0] + side_index_count);
			else
				dirct_index_[i].insert(dirct_index_[i].end(), &side_space_index_[North][0] + side_space_index_count / 2, &side_space_index_[North][0] + side_space_index_count);
		}
		if (i&TILEDIRECT::ES){//东南角索引
			dirct_index_[i].insert(dirct_index_[i].end(), quad_core_index_[3].begin(), quad_core_index_[3].end());
			if (i&TILEDIRECT::East)
				dirct_index_[i].insert(dirct_index_[i].end(), &side_index_[East][0] + side_index_count / 2, &side_index_[East][0] + side_index_count);
			else
				dirct_index_[i].insert(dirct_index_[i].end(), &side_space_index_[East][0] + side_space_index_count / 2, &side_space_index_[East][0] + side_space_index_count);
			if (i&TILEDIRECT::South)
				dirct_index_[i].insert(dirct_index_[i].end(), &side_index_[South][0] + side_index_count / 2, &side_index_[South][0] + side_index_count);
			else
				dirct_index_[i].insert(dirct_index_[i].end(), &side_space_index_[South][0] + side_space_index_count / 2, &side_space_index_[South][0] + side_space_index_count);
		}
	}
}

TileDataManager::~TileDataManager()
{
	abort = true;
	io_mutex.lock();
	condition.wakeOne();
	io_mutex.unlock();
}


bool TileDataManager::Loadable(const TILEPOS& _pos)
{
	bool ret = false;
	char file_path[512];
	ret = ret || _access(SpliceTilePos(&_pos, picture_dir_path_.c_str(), ".jpg", file_path), 0) == 0;
	ret = ret || _access(SpliceTilePos(&_pos, dem_dir_path_.c_str(), ".dem", file_path), 0) == 0;
	return ret;
}

void TileDataManager::LoadTile(FzQuadTile* _tile)
{
	QMutexLocker locker(&io_mutex);
	queue_tile_.push_back(_tile);

	if (!isRunning()) {
		start(LowPriority);
	}
	else
		condition.wakeOne();
}

void TileDataManager::LoadTileChoke(FzQuadTile* _tile)
{
	map_mutex.lock();
	auto& p = map_data_[_tile->GetTilePos()];
	map_mutex.unlock();
	if (nullptr == p)
		p = GetTileData(_tile->GetTilePos());
	_tile->SetTileDate(p);
}

const std::vector<unsigned short>* TileDataManager::GetTileIndexbyDirect(const TILEDIRECT& _direct) const
{
	unsigned char var = _direct;
	if (var >= dirct_index_.size())
		var = dirct_index_.size() - 1;
	return &dirct_index_[var];
}

void TileDataManager::slot_load_tile_data(FzQuadTile* _tile, std::shared_ptr<TILEDATA>* _tile_data)
{
	_tile->SetTileDate(*_tile_data);
}

void TileDataManager::run()
{
	while (!abort)
	{
		io_mutex.lock();
		QQueue<FzQuadTile*> temp_tile_;
		std::swap(temp_tile_, queue_tile_);
		io_mutex.unlock();
		for (auto iter : temp_tile_)
		{
			map_mutex.lock();
			auto& p = map_data_[iter->GetTilePos()];
			map_mutex.unlock();
			if (nullptr == p)
				p = GetTileData(iter->GetTilePos());
			emit signal_load_tile_data(iter, &p);
		}
		io_mutex.lock();
		if (queue_tile_.empty())
		{
			condition.wait(&io_mutex);
		}
		io_mutex.unlock();
	}
}

char* TileDataManager::SpliceTilePos(const TILEPOS* _pos, const char* _root_dir, const char* _suffix, char _out_str[512])
{
	sprintf_s(_out_str, 512, "%s/%d/%d/%d%s\0", _root_dir, _pos->level, _pos->col, _pos->row, _suffix);
	return _out_str;
}

std::shared_ptr<TILEDATA> TileDataManager::GetTileData(const TILEPOS& _pos)
{
	char file_path[512];
	std::shared_ptr<TILEDATA> p(new TILEDATA);
	int nrChannels = 0;
	SpliceTilePos(&_pos, picture_dir_path_.c_str(), ".jpg", file_path);
	p->pix_image_ = stbi_load(file_path, &p->width_, &p->height_, &nrChannels, STBI_rgb_alpha);
	if (p->pix_image_){
#if 0
		unsigned char _white[4] = { 255, 255, 255, 255 };
		_white[_pos.level % 3] = 127;
		unsigned int* test = (unsigned int*)p->pix_image_;
		for (size_t i = 0; i < p->width_; i++)
		{
			memcpy(test + i, _white, sizeof(_white));
			memcpy(test + i + (p->height_ - 1)*p->width_, _white, sizeof(_white));
		}
		for (size_t i = 0; i < p->height_; i++)
		{
			memcpy(test + i*p->width_, _white, sizeof(_white));
			memcpy(test + (i + 1)*p->width_ - 1, _white, sizeof(_white));
		}
#endif
		p->data_usable_[2] = true;
		p->tex_ = tex;
	}


	SpliceTilePos(&_pos, dem_dir_path_.c_str(), ".dem", file_path);
	FILE *fp = nullptr;
	int err = 0;
	int numread = 0;

	err = fopen_s(&fp, file_path, "rb");
	//如果高程文件存在
	if (err == 0)
	{
		p->data_usable_[1] = true;
		numread = fread(p->altitude_, sizeof(float)*sidepos_count*sidepos_count, 1, fp);
		fclose(fp);
	}
	return p;
}
