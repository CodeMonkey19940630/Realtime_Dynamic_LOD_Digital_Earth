#pragma once
#include <string>
#include "../../../include/GeoMath/GeoMathHeader.h"
#include "../../../include/GeoMath/CustomMath.h"
#include <memory>
#include <QObject>

//�ؿ����ö��ֵ
enum TILEDIRECT :unsigned char
{
	None = 0,//Ϊ��
	WN = 1,//������
	WS = 2,//���Ͻ�
	EN = 4,//������
	ES = 8,//���Ͻ�
	West = 16,//����
	East = 32,//����
	North = 64,//����
	South = 128,//����
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



//����ؿ����λ�ã�������Ϊmap�ṹ��keyʹ��
// ��/��/�� X/0/0Ϊ��ͼ���Ͻ�(���ȣ�0��γ�ȣ�85��)
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

	//�õ����ؿ�����λ��
	const TILEPOS GetParentPos() const
	{
		return TILEPOS(level - 1, col / 2, row / 2);
	}

	//��ñ��ؿ���Ը��ؿ�ķ�λ
	const TILEDIRECT GetSelfDirectofParent() const
	{
		return TILEDIRECT(1<<((col % 2) * 2 + (row % 2)));
	}

	//���ز㼶�������Ƿ񳬹����壬���ѳ�������������޸������ʷ�Χ
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

	//λ�üӷ���õ��µ�λ��
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
	//�߳�,����,Ӱ��,ʸ���Ƿ����
	bool		data_usable_[4];
	//�߳�����
	float		altitude_[9][9];
	//��������
	gVector3f	normal_[9][9];
	//������������
	const gVector2f*	tex_;
	//ʸ������
	std::vector<std::vector<rLBH>> vector_part_;
	//������
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

	//centercoord:������Ϊԭ�㣬XYZ����ECRһ��
	std::vector<gVector3f>	posvec_center_offset_;

	std::vector<gVector3f>	norvec_ecr_;

	//�ؿ���Ҫ��ʾ���Ľ�
	TILEDIRECT	possess_direct_;

	//�ؿ���Ҫ��������ڵؿ�
	TILEDIRECT	near_direct_;

	const std::vector<unsigned short>*	indexvec_;

	unsigned	texture_id_;

	int			mode_ = -1;

	std::shared_ptr<TILEDATA> 	data_;
};

