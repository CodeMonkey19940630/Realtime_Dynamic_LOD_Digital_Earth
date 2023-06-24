#pragma once
#include "FzNode.h"
#define  TerrainNodeObject "object.terrain"
#ifdef TERRAINRENDERER_LIB
# define TERRAINRENDERER_EXPORT __declspec(dllexport)
#else
# define TERRAINRENDERER_EXPORT __declspec(dllexport)
#endif
class FzQuadTile;
struct TILEPOS;

class TerrainRenderer;
class TERRAINRENDERER_EXPORT FzTerrainNode :public FzNode
{
public:
	FzTerrainNode(const std::string& _node_id = "");
	~FzTerrainNode();

	virtual bool ViewRangeTest(const FzCamera* _camera, double* _near = nullptr, double* _far = nullptr) override;

	virtual void SetMapDataDir(std::string _file_path);

	const std::vector<FzQuadTile*> GetVisibleQuadTile();

protected:
	friend class TerrainRenderer;

protected:
	std::string dem_dir_path_;

	std::string picture_dir_path_;

	std::string vector_dir_path_;

	std::vector<FzQuadTile*> root_level3_tilevec_;

	std::map<TILEPOS, FzQuadTile*> map_tiles_;

	std::vector<TILEPOS> expect_tpos_;

	int			max_level_;

	int			min_level_;

	int			current_level_;
};

