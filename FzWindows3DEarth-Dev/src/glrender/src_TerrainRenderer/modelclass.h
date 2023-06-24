//
// modelclass.h
//

#pragma once

#include "../include/uniHead.h"
#include "QuadTile.h"
#include "terrain_tile_shapes.h"

//////////////////////////////////////////////////////////////////////////
//
class ModelClass
{
public:
	enum e_constants
	{
		k_totalDetailGridLevels = 6,
		k_totalShapes = 16,
	};

	enum e_side_flags
	{
		k_top = 0,
		k_left,
		k_right,
		k_bottom,
		k_totalSides
	};

	// Data Types & Constants...
	struct sDetailLevel
	{
		detail_index_data*  tileBodies[k_totalShapes];
	};

	// this set of bit fields is used to record which of our
	// cIndexBuffers we wish to render. This is passed as user
	// data to the render queue, so we can render the proper
	// index buffer when the render callback is triggered
	union sUserData
	{
		struct
		{
			uint32 bodyTileFlag		:1;
			uint32 detailLevel		:2;
			uint32 side				:2;
			uint32 connectionLevel	:4;
			uint32 shapeNumber		:4;
		};
		uint32 value;
	};

public:
	sDetailLevel s_detailLevel[k_totalDetailGridLevels];


public:
	ModelClass();
	~ModelClass();

	bool Initialize();
	void Shutdown();

private:
	void ShutdownBuffers();

	//////////////////////////////////////////////////////////////////////////
	bool createIndexBuffer(detail_index_data** ppIndexBuffer, detail_index_data* source);
	bool generateDetailLevels();

public:
    GLint m_Texture;

};
