#include "terrainrenderer_global.h"
#include <QOpenGLWidget>
#include "imageLoad.h"
#include "FzQuadTile.h"
#include "TileDataManager.h"
#include <QDebug>

extern TileDataManager* tile_manager_;

FzQuadTile::FzQuadTile(const TILEPOS& _t_pos)
{
	texture_id_ = 0;
	t_pos_ = _t_pos;
	left_top_ = tile2LB(gVector2d(t_pos_.col, t_pos_.row), t_pos_.level);
	r_dlon_ = (2 * M_PI) / (1 << t_pos_.level);
	r_dlat_ = left_top_.B - D2R(tiley2dlat(t_pos_.row + 1, t_pos_.level));
	center_pos_ = LBHToEcr(tile2LB(gVector2d(t_pos_.col + 0.5, t_pos_.row + 0.5), t_pos_.level));
	possess_direct_ = WN | WS | EN | ES;
	near_direct_ = West | East | North | South;
	indexvec_ = nullptr;
}


FzQuadTile::~FzQuadTile()
{
}

const TILEPOS FzQuadTile::GetTilePos() const
{
	return t_pos_;
}

void FzQuadTile::SetTileDate(const std::shared_ptr<TILEDATA>& _data)
{
	if (_data)
	{
		data_ = _data;
		if (tile_manager_)
			indexvec_ = tile_manager_->GetTileIndexbyDirect(possess_direct_);
		posvec_center_offset_.resize(9 * 9);
		for (size_t i = 0; i < posvec_center_offset_.size(); i++)
		{
			int row = i / 9;
			int col = i % 9;
			rLBH g_pos(tile2LB(gVector2d(t_pos_.col + col / 8.0, t_pos_.row + row / 8.0), t_pos_.level), _data->altitude_[row][col]*10);
			posvec_center_offset_[i] = LBHToEcr(g_pos) - center_pos_;
		}
		if (texture_id_)
		{
			glDeleteTextures(1, &texture_id_);
			texture_id_ = 0;
		}

		if (data_->pix_image_)
		{
			texture_id_ = CreateTexture(data_->pix_image_, data_->width_, data_->height_);
		}
	}
}

void FzQuadTile::toMode(bool _is_3d)
{
	int _mode = _is_3d ? 1 : 0;
	if (mode_ != _mode)
	{
		switch (_mode)
		{
		case 0:
			break;
		case 1:
			break;
		default:
			break;
		}
		mode_ = _mode;
	}
}

void FzQuadTile::SetPossessDirect(TILEDIRECT _self_dirct)
{
	possess_direct_ = _self_dirct&(TILEDIRECT)0x0f;
}

TILEDIRECT FzQuadTile::GetPossessDirect()
{
	return possess_direct_;
}

void FzQuadTile::SetNearDirect(TILEDIRECT _near_dirct)
{
	near_direct_ = _near_dirct&(TILEDIRECT)0xf0;
}

TILEDIRECT FzQuadTile::GetNearDirect()
{
	return near_direct_;
}

void FzQuadTile::UpdateIndex()
{
	indexvec_ = tile_manager_->GetTileIndexbyDirect(possess_direct_ | near_direct_);
}

unsigned FzQuadTile::GetTextureId()
{
	return texture_id_;
}

const gVector3f* FzQuadTile::GetPosVertexPtr() const
{
	if (posvec_center_offset_.empty())
		return nullptr;
	return &posvec_center_offset_[0];
}

const gVector3f* FzQuadTile::GetNorVertexPtr() const
{
	if (norvec_ecr_.empty())
		return nullptr;
	return &norvec_ecr_[0];
}

const gVector2f* FzQuadTile::GetTexVertexPtr() const
{
	if (data_)
		return data_->tex_;
	else
		return nullptr;
}

unsigned int FzQuadTile::GetVertexCount() const
{
	return posvec_center_offset_.size();
}

const unsigned short* FzQuadTile::GetIndexPtr() const
{
	if (!indexvec_ || indexvec_->empty())
		return nullptr;
	return &indexvec_->front();
}

unsigned short FzQuadTile::GetIndexCount() const
{
	if (!indexvec_)
		return 0;
	return indexvec_->size();
}

gVector3d FzQuadTile::GetCenterPos() const
{
	return center_pos_;
}

bool FzQuadTile::isUsable() const
{
	return data_ && data_->isUsable();
}

bool FzQuadTile::isLoaded() const
{
	return data_ != nullptr;
}
