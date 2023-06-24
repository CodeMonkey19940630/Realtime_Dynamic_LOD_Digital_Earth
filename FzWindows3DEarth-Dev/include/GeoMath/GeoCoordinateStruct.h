#ifndef GEOCOORDINATESTRUCT_H
#define GEOCOORDINATESTRUCT_H
#include "GeoMathematics.h"

//�Ƕ���,��γ
struct dLB
{
	dLB() :L(0), B(0)
	{}
	dLB(double _l, double _b)
		: L(_l), B(_b)
	{}
	//��ؾ���(��)
	double L;
	//���γ��(��)
	double B;

	bool operator==(const dLB& _data) const
	{
		return L == _data.L&&B == _data.B;
	}
	bool operator!=(const dLB& _data) const
	{
		return L != _data.L||B != _data.B;
	}
};

//������,��γ,��ͨ��dLB��ֵ
struct rLB
{
	rLB() :L(0), B(0)
	{}
	rLB(double _l, double _b)
		: L(_l), B(_b)
	{}
	rLB(const dLB& _d)
		:L(D2R(_d.L)), B(D2R(_d.B))
	{}
	dLB toDegree() const
	{
		return dLB(R2D(L), R2D(B));
	}
	//��ؾ���(����)
	double L;
	//���γ��(����)
	double B;

	bool operator==(const rLB& _data) const
	{
		return L == _data.L&&B == _data.B;
	}
	bool operator!=(const rLB& _data) const
	{
		return L != _data.L || B != _data.B;
	}
};

//�Ƕ��ƣ����ƴ�ؾ�γ��
struct dLBH
{
	dLBH() :L(0), B(0), H(0)
	{}
	dLBH(double _l, double _b, double _h = 0)
		:L(_l), B(_b), H(_h)
	{}
	dLBH(const dLB& _lb, double _h = 0)
		:L(_lb.L), B(_lb.B), H(_h)
	{}
	dLB getdLB() const
	{
		return dLB(L, B);
	}
	//���γ��(��)
	double L;
	//��ؾ���(��)
	double B;
	//��ظ߶�(m)
	double H;
	bool operator==(const dLBH& _data) const
	{
		return L == _data.L&&B == _data.B&&H == _data.H;
	}
	bool operator!=(const dLBH& _data) const
	{
		return L != _data.L||B != _data.B||H != _data.H;
	}
};

//�����ƣ����ƴ�ؾ�γ��,��ͨ��dLBH��ֵ
struct rLBH
{
	rLBH() :L(0), B(0), H(0)
	{}
	rLBH(double _l, double _b, double _h = 0)
		:L(_l), B(_b), H(_h)
	{}
	rLBH(const rLB& _lb, double _h = 0)
		:L(_lb.L), B(_lb.B), H(_h)
	{}
	rLBH(const dLBH& _d)
		:L(D2R(_d.L)), B(D2R(_d.B)), H(_d.H)
	{}
	dLBH toDegree() const
	{
		return dLBH(R2D(L), R2D(B), H);
	}
	rLB getrLB() const
	{
		return rLB(L, B);
	}
	//��ؾ���(����)(longitude)
	double L;
	//���γ��(����)(latitude)
	double B;
	//��ظ߶�(m)
	double H;

	bool operator==(const rLB& _data) const
	{
		return L == _data.L&&B == _data.B;
	}
	bool operator!=(const rLB& _data) const
	{
		return L != _data.L || B != _data.B;
	}
};

/*!
 * \fn	inline bool IsLBHApproximate(rLBH _pos_a, rLBH _pos_b, const double _tolerance_m=0.1)
 *
 * \brief	������������Ƿ�����غ�.
 *
 * \author	�����
 * \date	2022/2/7
 *
 * \param	_pos_a			a������.
 * \param	_pos_b			b������.
 * \param	_tolerance_m	�������.
 *
 * \return	����غϷ���true,���غϷ���false.
 */

inline bool IsLBHApproximate(rLBH _pos_a, rLBH _pos_b, const double _tolerance_m=0.1)
{
	if (fabs(_pos_a.H - _pos_b.H) > _tolerance_m)
		return false;
	if (fabs(_pos_a.B - _pos_b.B)* GMath::earth_radius_ > _tolerance_m)
		return false;
	double _deta_l = _pos_a.L - _pos_b.L;
	_deta_l = _deta_l - round(_deta_l / (2 * M_PI))*(2 * M_PI);
	if (fabs(_pos_a.L*cos(_pos_a.B) - _pos_b.L*cos(_pos_b.B))*GMath::earth_radius_ > _tolerance_m)
		return false;

	return true;
}

//���ƣ��Ƕ���������
struct dSphereCoord
{
	dSphereCoord() :r(0), azimuth(0), elevation(0)
	{}
	dSphereCoord(double _az, double _tilt, double _r)
		:azimuth(_az), elevation(_tilt), r(_r)
	{}
	//��λ��(��)
	double azimuth;
	//��б��(��)
	double elevation;
	//�뾶(m)
	double r;
	
	bool operator==(const dSphereCoord& _data) const
	{
		return azimuth==_data.azimuth&&elevation==_data.elevation&&r==_data.r;
	}
	bool operator!=(const dSphereCoord& _data) const
	{
		return azimuth!=_data.azimuth||elevation!=_data.elevation||r!=_data.r;
	}
};

//���ƣ�������������,��ͨ��dSphereCoord��ֵ
struct rSphereCoord
{
	rSphereCoord() :r(0), azimuth(0), elevation(0)
	{}
	rSphereCoord(double _az, double _tilt, double _r)
		:azimuth(_az), elevation(_tilt), r(_r)
	{}
	rSphereCoord(const dSphereCoord& _d)
		:azimuth(D2R(_d.azimuth)), elevation(D2R(_d.elevation)), r(_d.r)
	{}
	dSphereCoord toDegree() const
	{
		return dSphereCoord(R2D(azimuth), R2D(elevation), r);
	}
	//��λ��(����)
	double azimuth;
	//��б��(����)
	double elevation;
	//�뾶(m)
	double r;
	
	bool operator==(const rSphereCoord& _data) const
	{
		return azimuth==_data.azimuth&&elevation==_data.elevation&&r==_data.r;
	}
	bool operator!=(const rSphereCoord& _data) const
	{
		return azimuth!=_data.azimuth||elevation!=_data.elevation||r!=_data.r;
	}
};


//�Ƕ��� ��λ����,�����
struct dOrientation
{
	dOrientation() : yaw(0), pitch(0)
	{}
	dOrientation(double _yaw, double _pitch)
		: yaw(_yaw), pitch(_pitch)
	{}
	//��λ��(��)
	double yaw;
	//��б��(��)
	double pitch;
	
	bool operator==(const dOrientation& _data) const
	{
		return yaw==_data.yaw&&pitch==_data.pitch;
	}
	bool operator!=(const dOrientation& _data) const
	{
		return yaw!=_data.yaw||pitch!=_data.pitch;
	}
};

//������ ��λ����,��ͨ��dOrientation��ֵ
struct rOrientation
{
	rOrientation() : yaw(0), pitch(0)
	{}
	rOrientation(double _yaw, double _pitch)
		: yaw(_yaw), pitch(_pitch)
	{}
	rOrientation(const dOrientation& _d)
		: yaw(D2R(_d.yaw)), pitch(D2R(_d.pitch))
	{}
	dOrientation toDegree() const
	{
		return dOrientation(R2D(yaw), R2D(pitch));
	}
	//��λ��(����)
	double yaw;
	//��б��(����)
	double pitch;
	
	bool operator==(const rOrientation& _data) const
	{
		return yaw==_data.yaw&&pitch==_data.pitch;
	}
	bool operator!=(const rOrientation& _data) const
	{
		return yaw!=_data.yaw||pitch!=_data.pitch;
	}
};

//�Ƕ���ƫ������������ת����̬��
struct dAttitude
{
	dAttitude() :yaw(0), pitch(0), roll(0)
	{}
	dAttitude(double _yaw, double _pitch, double _roll)
		:yaw(_yaw), pitch(_pitch), roll(_roll)
	{}
	dAttitude(const dOrientation& _rori, double _roll = 0)
		:yaw(_rori.yaw), pitch(_rori.pitch), roll(_roll)
	{}
	dOrientation getdOrientation() const
	{
		return dOrientation(yaw,pitch);
	}
	//ƫ����(��)
	double yaw;
	//������(��)
	double pitch;
	//��ת��(��)
	double roll;
	
	bool operator==(const dAttitude& _data) const
	{
		return yaw==_data.yaw&&pitch==_data.pitch&&roll==_data.roll;
	}
	bool operator!=(const dAttitude& _data) const
	{
		return yaw!=_data.yaw||pitch!=_data.pitch||roll!=_data.roll;
	}
};

//������ƫ������������ת
struct rAttitude
{
	rAttitude() :yaw(0), pitch(0), roll(0)
	{}
	rAttitude(double _yaw, double _pitch, double _roll)
		:yaw(_yaw), pitch(_pitch), roll(_roll)
	{}
	rAttitude(const dAttitude& _d)
		: yaw(D2R(_d.yaw)), pitch(D2R(_d.pitch)), roll(D2R(_d.roll))
	{}
	rAttitude(const rOrientation& _rori, double _roll = 0)
		:yaw(_rori.yaw), pitch(_rori.pitch), roll(_roll)
	{}
	dAttitude toDegree() const
	{
		return dAttitude(R2D(yaw), R2D(pitch), R2D(roll));
	}
	rOrientation getrOrientation() const
	{
		return rOrientation(yaw, pitch);
	}
	//ƫ����(����)
	double yaw;
	//������(����)
	double pitch;
	//��ת��(����)
	double roll;
	
	bool operator==(const rAttitude& _data) const
	{
		return yaw==_data.yaw&&pitch==_data.pitch&&roll==_data.roll;
	}
	bool operator!=(const rAttitude& _data) const
	{
		return yaw!=_data.yaw||pitch!=_data.pitch||roll!=_data.roll;
	}
};

//�Ƕ���ŷ���� 
struct dEuler
{
	dEuler() :psi(0), theta(0), phi(0)
	{}
	dEuler(double _psi, double _theta, double _phi)
		:psi(_psi), theta(_theta), phi(_phi)
	{}
	//������һ(�Ƕ�)
	double psi;
	//�����Ƕ�(�Ƕ�)
	double theta;
	//��������(�Ƕ�)
	double phi;
	
	bool operator==(const dEuler& _data) const
	{
		return psi==_data.psi&&theta==_data.theta&&phi==_data.phi;
	}
	bool operator!=(const dEuler& _data) const
	{
		return psi!=_data.psi||theta!=_data.theta||phi!=_data.phi;
	}
};

//������ŷ���� 
struct rEuler
{
	rEuler() :psi(0), theta(0), phi(0)
	{}
	rEuler(double _psi, double _theta, double _phi)
		:psi(_psi), theta(_theta), phi(_phi)
	{}
	rEuler(const dEuler& _d)
		:psi(D2R(_d.psi)), theta(D2R(_d.theta)), phi(D2R(_d.phi))
	{}
	dEuler toDegree() const
	{
		return dEuler(R2D(psi), R2D(theta), R2D(phi));
	}
	//������һ(����)
	double psi;
	//�����Ƕ�(����)
	double theta;
	//��������(����)
	double phi;
	
	bool operator==(const rEuler& _data) const
	{
		return psi==_data.psi&&theta==_data.theta&&phi==_data.phi;
	}
	bool operator!=(const rEuler& _data) const
	{
		return psi!=_data.psi||theta!=_data.theta||phi!=_data.phi;
	}
};


//������
struct ellipsoid_standard
{
	/*! \brief ���� */
	double a = 6371000;
	/*! \brief ���� */
	double b = 6371000;
	/*! \brief ���� alpha=(a-b)/a; */
	double f = 0;
	/*! \brief �����ʰ뾶 c = (a ^ 2) / b; */
	double c = 1;
	/*! \brief ��һƫ���� e = sqrt(a ^ 2 - b ^ 2) / a; */
	double e1 = 0;
	/*! \brief ee = (a ^ 2 - b ^ 2) / a ^ 2; */
	double e1_sqaure = 0;
	/*! \brief �ڶ�ƫ���� e = sqrt(a ^ 2 - b ^ 2) / b; */
	double e2 = 0;
	/*! \brief ee = (a ^ 2 - b ^ 2) / b ^ 2; */
	double e2_sqaure = 0;

	ellipsoid_standard()
	{}
	ellipsoid_standard(double _radius)
	{
		a = _radius;
		b = _radius;
	}
	ellipsoid_standard(double _a, double _b)
	{
		a = _a;
		b = _b;
		f = (a - b) / a;
		c = a*a / b;
		e1_sqaure = (a *a - b*b) / (a*a);
		e1 = sqrt(e1_sqaure);
		e2_sqaure = (a *a - b*b) / (b*b);
		e2 = sqrt(e2_sqaure);
	}
	~ellipsoid_standard()
	{}
};

/*! \brief	wgs84 ��׼������� */
const ellipsoid_standard ellipsoid_wgs84_(6378137.0, 6356752.314251795);
/*! \brief	rew���� IAG75 ������� */
const ellipsoid_standard ellipsoid_iag75_(6378140, 6356755);
/*! \brief	Բ�� ������� */
const ellipsoid_standard ellipsoid_sphere_(6378137);

#endif // GEOVECTOR