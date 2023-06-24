#pragma once
#include <string>
#include "../../../include/GeoMath/GeoMathHeader.h"
#include "../../../include/GeoMath/CustomMath.h"
#include <memory>
#include <QObject>

//地块角落枚举值
enum TILEDIRECT :unsigned char
{
	None = 0,//为空
	WN = 1,//西北角
	WS = 2,//西南角
	EN = 4,//东北角
	ES = 8,//东南角
	West = 16,//正西
	East = 32,//正东
	North = 64,//正北
	South = 128,//正南
};

inline const TILEDIRECT operator|(const TILEDIRECT& a, const TILEDIRECT& b)
{
	return (TILEDIRECT)((unsigned char)a | (unsigned char)b);
}

inline const TILEDIRECT operator&(const TILEDIRECT& a, const TILEDIRECT& b)
{
	return (TILEDIRECT)((unsigned char)a & (unsigned char)b);
}

inline const TILEDIRECT operator^(const TILEDIRECT& a, const TILEDIRECT& b)
{
	return (TILEDIRECT)((unsigned char)a ^ (unsigned char)b);
}



//地球地块基础位置，可以作为map结构的key使用
// 级/列/行 X/0/0为地图左上角(经度：0，纬度：85度)
struct TILEPOS
{
	int		level;
	int		col;
	int		row;
	TILEPOS()
		:level(0), col(0), row(0) {}

	TILEPOS(int levels, int cols, int rows)
		:level(levels), col(cols), row(rows) {}

	bool operator<(const TILEPOS tp) const
	{
		if (level != tp.level)
			return level < tp.level;
		if (col != tp.col)
			return col < tp.col;
		return row < tp.row;
	}

	bool operator == (const TILEPOS tp) const
	{
		return (row == tp.row && col == tp.col && level == tp.level);
	}

	bool operator != (const TILEPOS tp) const
	{
		return (row != tp.row || col != tp.col || level != tp.level);
	}

	//得到父地块所在位置
	const TILEPOS GetParentPos() const
	{
		return TILEPOS(level - 1, col / 2, row / 2);
	}

	//获得本地块相对父地块的方位
	const TILEDIRECT GetSelfDirectofParent() const
	{
		return TILEDIRECT(1<<((col % 2) * 2 + (row % 2)));
	}

	//返回层级、行数是否超过定义，并把超过定义的列数修改至合适范围
	const bool isNan()
	{
		const int t = (1 << level);
		if (level < 0)
			return true;
		if (row < 0 || row >= t)
			return true;
		if (col < 0)
			col = t - ((-col) % t);
		else if (col >= t)
			col = col%t;
		return false;
	}

	//位置加方向得到新的位置
	const TILEPOS operator+(const TILEDIRECT& _direct) const
	{
		int col_move = 0, row_move = 0;
		switch (_direct)
		{
		case None:
			break;
		case WN:
			col_move -= 1;
			row_move -= 1;
			break;
		case WS:
			col_move -= 1;
			row_move += 1;
			break;
		case EN:
			col_move += 1;
			row_move -= 1;
			break;
		case ES:
			col_move += 1;
			row_move += 1;
			break;
		case West:
			col_move -= 1;
			break;
		case East:
			col_move += 1;
			break;
		case North:
			row_move -= 1;
			break;
		case South:
			row_move += 1;
			break;
		default:
			break;
		}
		return TILEPOS(level, col + col_move, row + row_move);
	}
};

struct TILEDATA
{
	TILEDATA()
	{
		for (size_t i = 0; i < 4; i++)
		{
			data_usable_[i] = false;
		}
		for (int i = 0; i < 9; i++){
			for (int j = 0; j < 9; j++){
				altitude_[i][j] = 0;
			}
		}
		pix_image_ = nullptr;
		width_ = 0;
		height_ = 0;
		tex_ = nullptr;
	}
	~TILEDATA()
	{
		if (pix_image_)
			free(pix_image_);
		width_ = 0;
		height_ = 0;
		tex_ = nullptr;
	}
	bool isUsable() const
	{
		return 0 != *(int*)data_usable_;
	}
	//高程,法线,影像,矢量是否可用
	bool		data_usable_[4];
	//高程数组
	float		altitude_[9][9];
	//法线数组
	gVector3f	normal_[9][9];
	//纹理坐标数组
	const gVector2f*	tex_;
	//矢量数据
	std::vector<std::vector<rLBH>> vector_part_;
	//纹理缓存
	unsigned char* pix_image_;
	int			width_;
	int			height_;
};

class BaseTile
{
public:
	BaseTile() :dormantcount_(0){};
	virtual ~BaseTile(){};

	virtual bool isUsable() const = 0;

	virtual bool isLoaded() const = 0;

	virtual bool addDormantCount(){ return ++dormantcount_ <= 300; };

	virtual void resetDormantCount(){ dormantcount_ = 0; };
protected:
	unsigned int dormantcount_;
};


class FzQuadTile :public BaseTile
{
public:
	FzQuadTile(const TILEPOS& _t_pos);
	~FzQuadTile();

	const TILEPOS GetTilePos() const;

	void SetTileDate(const std::shared_ptr<TILEDATA>& _data);

	void toMode(bool _is_3d);

	void SetPossessDirect(TILEDIRECT _self_dirct);

	TILEDIRECT GetPossessDirect();

	void SetNearDirect(TILEDIRECT _near_dirct);

	TILEDIRECT GetNearDirect();

	void UpdateIndex();
	
	unsigned	GetTextureId();

	const gVector3f*	GetPosVertexPtr() const;

	const gVector3f*	GetNorVertexPtr() const;

	const gVector2f*	GetTexVertexPtr() const;

	unsigned int	GetVertexCount() const;

	const unsigned short*	GetIndexPtr() const;

	unsigned short	GetIndexCount() const;

	gVector3d	GetCenterPos() const;

	bool isUsable() const;

	bool isLoaded() const;

	bool operator <(const FzQuadTile& _tile) const{ return t_pos_ < _tile.t_pos_;}
protected:

	TILEPOS		t_pos_;

	rLB			left_top_;

	double		r_dlon_;

	double		r_dlat_;

	gVector3d	center_pos_;

	//centercoord:以中心为原点，XYZ轴与ECR一致
	std::vector<gVector3f>	posvec_center_offset_;

	std::vector<gVector3f>	norvec_ecr_;

	//地块需要显示的四角
	TILEDIRECT	possess_direct_;

	//地块需要间隔的相邻地块
	TILEDIRECT	near_direct_;

	const std::vector<unsigned short>*	indexvec_;

	unsigned	texture_id_;

	int			mode_ = -1;

	std::shared_ptr<TILEDATA> 	data_;
};

