#include "terrainrenderer_global.h"
#include "FzQuadTile.h"
#include "../../../include/glRender/FzNode/FzTerrainNode.h"
#include "TileDataManager.h"
TileDataManager* tile_manager_ = nullptr;

bool cmpTilePtr(const FzQuadTile* _a, const FzQuadTile* _b)
{
	return *_a < *_b;
}

bool cmpTile(const FzQuadTile* _a, const TILEPOS& _b)
{
	return _a->GetTilePos() < _b;
}

int LevelComputer(int max_level, float near_alt, int min_level, int far_alt, double current_alt, double _power = 1.0)
{
	if (current_alt>far_alt){
		return min_level;
	}
	else if (current_alt<near_alt){
		return max_level;
	}
	double ln_far = log(far_alt / near_alt) ;
	double ln_cur = log(current_alt / near_alt) ;

	double t = pow(ln_cur / ln_far, _power);
	//最大层级不能取等
	max_level = max_level - 1;
	return ceil(max_level - t*(max_level - min_level));
}

FzTerrainNode::FzTerrainNode(const std::string& _node_id /*= ""*/)
{
	object_name_ = TerrainNodeObject;
	min_level_ = 3;
	max_level_ = 18;
}

FzTerrainNode::~FzTerrainNode()
{
	if (tile_manager_)
	{
		delete tile_manager_, tile_manager_ = nullptr;
	}
	root_level3_tilevec_.clear();
	for (auto& iter:map_tiles_)
	{
		delete iter.second, iter.second = nullptr;
	}
	map_tiles_.clear();
}

bool FzTerrainNode::ViewRangeTest(const FzCamera* _camera, double* _near /*= nullptr*/, double* _far /*= nullptr*/)
{
	dLBH ground_ = EcrToLBH(_camera->getCameraPosition()).toDegree();
	current_level_ = LevelComputer(max_level_, EARTH_MEANRADIUS * 8 * M_PI / (1 << max_level_), min_level_, EARTH_MEANRADIUS * 8 * M_PI / (1 << min_level_), ground_.H);

	expect_tpos_.clear();
	for (size_t i = min_level_; i <= current_level_; i++)
	{
		int col = floor(dlon2tilex(ground_.L, i));
		int row = 0;
		if (abs(ground_.B) < 85)
			row = floor(dlat2tiley(ground_.B, i));
		else if (ground_.B > 85)
			row = -1;
		else
		{
			row = 1 << i;
		}
		for (int j = 0; j < 9; j++)
		{
			TILEPOS tile_pos(i, col - 1 + j / 3, row - 1 + j % 3);
			//位置非法则返回
			if (tile_pos.isNan())
				continue;
			expect_tpos_.push_back(tile_pos);
		}
	}
	for (size_t i = 0; i < expect_tpos_.size(); i++)
	{
		auto iter = map_tiles_.find(expect_tpos_[i]);
		if (iter==map_tiles_.end())
		{
			if (tile_manager_->Loadable(expect_tpos_[i]))
			{
				FzQuadTile* new_tile = new FzQuadTile(expect_tpos_[i]);
				tile_manager_->LoadTile(new_tile);
				map_tiles_.insert({ expect_tpos_[i], new_tile });
			}
		}
	}


	return true;
}

void FzTerrainNode::SetMapDataDir(std::string _file_path)
{
	dem_dir_path_ = _file_path + "/dem";
	picture_dir_path_ = _file_path + "/satellite";
	vector_dir_path_ = _file_path + "/vt";
	if (tile_manager_)
	{
		tile_manager_->dem_dir_path_ = dem_dir_path_;
		tile_manager_->picture_dir_path_ = picture_dir_path_;
		tile_manager_->vector_dir_path_ = vector_dir_path_;
	}

	int count_ = 1 << min_level_;
	for (size_t i = 0; i < count_; i++)
	{
		for (size_t j = 0; j < count_; j++)
		{
			FzQuadTile* _tile = new FzQuadTile(TILEPOS(min_level_, i, j));
			root_level3_tilevec_.push_back(_tile);
			map_tiles_.insert(map_tiles_.end(), { _tile->GetTilePos(), _tile });
		}
	}

	for (auto& _var: root_level3_tilevec_)
	{
		const TILEPOS t_pos = _var->GetTilePos();
		tile_manager_->LoadTile(_var);
	}
}

const std::vector<FzQuadTile*> FzTerrainNode::GetVisibleQuadTile()
{
	std::vector<FzQuadTile*> _quadtile;
	std::map<TILEPOS, unsigned char> parent_children;
	std::map<TILEPOS, unsigned char> current_children;
#if 1
	if (expect_tpos_.empty())
		return{};
	int record_level = expect_tpos_.back().level;
	for (auto i = expect_tpos_.rbegin(); i != expect_tpos_.rend(); i++)
	{
		if (record_level != i->level)
		{
			for (const auto& iter : current_children)
			{
				FzQuadTile* tile = map_tiles_[iter.first];
				unsigned char tile_near_direct = TILEDIRECT::None;
				for (size_t left_shift = 0; left_shift < 4; left_shift++)
				{
					TILEDIRECT near_dir = TILEDIRECT(West << left_shift);
					TILEPOS near_pos = (iter.first + near_dir);
					if (!near_pos.isNan() && current_children.find(near_pos) != current_children.end())
						tile_near_direct |= near_dir;
				}
				tile->SetNearDirect((TILEDIRECT)tile_near_direct);
				tile->UpdateIndex();
			}
			current_children.clear();
			//层级改变后，交换；
			std::swap(parent_children, current_children);
			//记录层级修改
			record_level = i->level;
		}
		FzQuadTile* tile = nullptr;
		auto find_iter = map_tiles_.find(*i);
		if (find_iter != map_tiles_.end())
			tile = find_iter->second;
		if (tile&&tile->isUsable())
		{
			const TILEPOS parent_pos = i->GetParentPos();
			//反向遍历，反向添加
			_quadtile.insert(_quadtile.begin(), tile);
			tile->resetDormantCount();
			TILEDIRECT _direct = (TILEDIRECT)current_children[*i];
			//通过异或方式把子节点存在区域取反,能得到自己应该绘制的区域
			tile->SetPossessDirect((TILEDIRECT)~_direct);
			if (i->level == min_level_)
				tile->UpdateIndex();
			unsigned char &p_child = parent_children[parent_pos];
			p_child += i->GetSelfDirectofParent();
		}
		else
		{
			current_children.erase(*i);
		}
	}
	//补全地图区域
#endif
	{
		auto iter_min_level_end = map_tiles_.lower_bound(TILEPOS(min_level_ + 1, 0, 0));
		auto iter = map_tiles_.begin();
		while (iter != iter_min_level_end)
		{
			if (current_children.find(iter->second->GetTilePos()) == current_children.end() && iter->second)
			{
				_quadtile.push_back(iter->second);
			}
			++iter;
		}
	}
	std::sort(_quadtile.begin(), _quadtile.end(), cmpTilePtr);
	//时间间隔每大于20s,删除一次不需要显示的地块
	if (true)
	{
		auto vec_iter = std::lower_bound(_quadtile.begin(), _quadtile.end(), TILEPOS(min_level_ + 1, 0, 0), cmpTile);
		for (auto map_iter = map_tiles_.lower_bound(TILEPOS(min_level_ + 1, 0, 0)); map_iter != map_tiles_.end();)
		{
			if (*(vec_iter._Ptr) == map_iter->second){
				++vec_iter;
				map_iter++;
				continue;
			}
			if (!map_iter->second->isLoaded() || map_iter->second->isUsable())
			{
				map_iter++;
				continue;
			}
			if (map_iter->second->addDormantCount())
			{
				delete map_iter->second, map_iter->second = nullptr;
				map_iter = map_tiles_.erase(map_iter);
			}
		}
	}
	return _quadtile;
}
