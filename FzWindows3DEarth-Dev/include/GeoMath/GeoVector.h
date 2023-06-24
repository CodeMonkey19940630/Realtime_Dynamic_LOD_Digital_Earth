#ifndef GEOVECTOR_H
#define GEOVECTOR_H
#include "GeoMathematics.h"
//������������ֵ��Lerp������ˣ�Dot������ˣ�Cross��������GeoFunction.h�鿴��
#pragma region 2ά����
template <typename T>
struct gVector2T
{
	gVector2T();
	gVector2T(T _s);
	gVector2T(T _s1, T _s2);
	template <typename U>
	gVector2T(const gVector2T<U>& _v);
	//�ӷ�
	gVector2T<T>	operator+(const gVector2T<T>& _v2) const;
	//����
	gVector2T<T>	operator-(const gVector2T<T>& _v2) const;
	//�ӵ���
	void	operator+=(const gVector2T<T>& _v2);
	//������
	void	operator-=(const gVector2T<T>& _v2);
	//�����˷�
	gVector2T<T>	operator*(const T& _s) const;
	//�����˷�
	template <typename T>
	friend gVector2T<T>	operator*(const double& _s, const gVector2T<T>& _v2);
	//��������
	gVector2T<T>	operator/(const T& _s) const;
	//�˵���
	void	operator*=(const T& _s);
	//������
	void	operator/=(const T& _s);
	//ȡ��
	gVector2T<T>	operator-() const;
	//�е�
	bool operator==(const gVector2T<T>& _t) const;
	//�в���
	bool operator!=(const gVector2T<T>& _t) const;
	//�±�ȡֵ
	T&	operator[](int _i);
	//������ģ��
	T	Length() const;
	//������ƽ����
	T	LengthSquare() const;
	//���������
	T	Dot(const gVector2T<T>& _v2) const;
	//�������н�����
	T	Cosine(const gVector2T<T>& _v2) const;
	//���������
	T	Cross(const gVector2T<T>& _v2) const;
	//�������Բ�ֵ
	gVector2T<T>	Lerp(const gVector2T<T>& _v2, double _t) const;
	//��λ����
	gVector2T<T>	GetNormalize() const;
	//תΪ��λ����
	void	toNormalize();
	//���������׵�ַ
	T*	ptr();
	//���������׵�ַ
	const T*	c_ptr() const;

	T	x;
	T	y;
};

#pragma endregion

#pragma region 3ά����
template <typename T>
struct gVector3T
{
	gVector3T();
	gVector3T(T _s);
	gVector3T(T _s1, T _s2, T _s3);
	gVector3T(gVector2T<T> _v, T _s3 = 0);
	template <typename U>
	gVector3T(const gVector3T<U>& _v);
	//�ӷ�
	gVector3T<T>	operator+(const gVector3T<T>& _v3) const;
	//����
	gVector3T<T>	operator-(const gVector3T<T>& _v3) const;
	//�ӵ���
	void	operator+=(const gVector3T<T>& _v3);
	//������
	void	operator-=(const gVector3T<T>& _v3);
	//�����˷�
	gVector3T<T>	operator*(const T& _s) const;
	//�����˷�
	template <typename T>
	friend gVector3T<T>	operator*(const double& _s, const gVector3T<T>& _v3);
	//��������
	gVector3T<T>	operator/(const T& _s) const;
	//�˵���
	void	operator*=(const T& _s);
	//������
	void	operator/=(const T& _s);
	//ȡ��
	gVector3T<T>	operator-() const;
	//�е�
	bool operator==(const gVector3T<T>& _t) const;
	//�в���
	bool operator!=(const gVector3T<T>& _t) const;
	//�±�ȡֵ
	T&	operator[](int _i);
	//������ģ��
	T	Length() const;
	//������ƽ����
	T	LengthSquare() const;
	//���������
	T	Dot(const gVector3T<T>& _v3) const;
	//�������н�����
	T	Cosine(const gVector3T<T>& _v3) const;
	//���������
	gVector3T<T>	Cross(const gVector3T<T>& _v3) const;
	//�������Բ�ֵ
	gVector3T<T>	Lerp(const gVector3T<T>& _v3, double _t) const;
	//��λ����
	gVector3T<T>	GetNormalize()  const;
	//תΪ��λ����
	void	toNormalize();
	//���������׵�ַ
	T*	ptr();
	//���������׵�ַ
	const T*	c_ptr() const;

	T	x;
	T	y;
	T	z;
};

#pragma endregion

#pragma region 4ά����
template <typename T>
struct gVector4T
{
	gVector4T();
	gVector4T(T _s);
	gVector4T(T _s1, T _s2, T _s3, T _s4);
	gVector4T(gVector2T<T> _v, T _s3 = 0, T _s4 = 0);
	gVector4T(gVector3T<T> _v, T _s4 = 0);
	template <typename U>
	gVector4T(const gVector4T<U>& _v);
	//�ӷ�
	gVector4T<T>	operator+(const gVector4T<T>& _v4) const;
	//����
	gVector4T<T>	operator-(const gVector4T<T>& _v4) const;
	//�ӵ���
	void	operator+=(const gVector4T<T>& _v4);
	//������
	void	operator-=(const gVector4T<T>& _v4);
	//�����˷�
	gVector4T<T>	operator*(const T& _s) const;
	//�����˷�
	template <typename T>
	friend gVector4T<T>	operator*(const double& _s, const gVector4T<T>& _v4);
	//��������
	gVector4T<T>	operator/(const T& _s) const;
	//�˵���
	void	operator*=(const T& _s);
	//������
	void	operator/=(const T& _s);
	//ȡ��
	gVector4T<T>	operator-() const;
	//�е�
	bool operator==(const gVector4T<T>& _t) const;
	//�в���
	bool operator!=(const gVector4T<T>& _t) const;
	//�±�ȡֵ
	T&	operator[](int _i);
	//������ģ��
	T	Length() const;
	//������ƽ����
	T	LengthSquare() const;
	//���������
	T	Dot(const gVector4T<T>& _v4) const;
	//�������н�����
	T	Cosine(const gVector4T<T>& _v4) const;
	//�������Բ�ֵ
	gVector4T<T>	Lerp(const gVector4T<T>& _v4, double _t) const;
	//��λ����
	gVector4T<T>	GetNormalize() const;
	//תΪ��λ����
	void	toNormalize();
	//���iλ����
	T	value(unsigned _i) const;
	//���������׵�ַ
	T*	ptr();
	//���������׵�ַ
	const T*	c_ptr() const;
	//����xyz��ά����
	const gVector3T<T>	GetXYZ() const;

	T	x;
	T	y;
	T	z;
	T	w;
};

#pragma endregion

template<typename T>
gVector2T<T>::gVector2T() :x(0), y(0)
{

}

template <typename T>
gVector2T<T>::gVector2T(T _s) :x(_s), y(_s)
{

}

template<typename T>
gVector2T<T>::gVector2T(T _s1, T _s2) : x(_s1), y(_s2)
{

}

template<typename T>
template<typename U>
gVector2T<T>::gVector2T(const gVector2T<U>& _v) : x(_v.x), y(_v.y)
{

}

//�ӷ�
template<typename T>
gVector2T<T>	gVector2T<T>::operator+(const gVector2T<T>& _v2) const
{
	gVector2T _vt(x + _v2.x, y + _v2.y);
	return _vt;
}

//����
template<typename T>
gVector2T<T>	gVector2T<T>::operator-(const gVector2T<T>& _v2) const
{
	gVector2T<T> _vt(x - _v2.x, y - _v2.y);
	return _vt;
}

//�ӵ���
template<typename T>
void	gVector2T<T>::operator+=(const gVector2T<T>& _v2)
{
	x += _v2.x;
	y += _v2.y;
}

//������
template<typename T>
void	gVector2T<T>::operator-=(const gVector2T<T>& _v2)
{
	x -= _v2.x;
	y -= _v2.y;
}

//�����˷�
template<typename T>
gVector2T<T>	gVector2T<T>::operator*(const T& _s) const
{
	gVector2T<T> _vt(x * _s, y * _s);
	return _vt;
}

//�����˷�
template<typename T>
gVector2T<T>	operator*(const double& _s, const gVector2T<T>& _v2)
{
	return gVector2T<T>(_v2.x*_s, _v2.y*_s);
}

//��������
template<typename T>
gVector2T<T>	gVector2T<T>::operator/(const T& _s) const
{
	gVector2T<T> _vt(x / _s, y / _s);
	return _vt;
}

//�˵���
template<typename T>
void	gVector2T<T>::operator*=(const T& _s)
{
	x *= _s;
	y *= _s;
}

//������
template<typename T>
void	gVector2T<T>::operator/=(const T& _s)
{
	x /= _s;
	y /= _s;
}

//ȡ��
template<typename T>
gVector2T<T>	gVector2T<T>::operator-() const
{
	gVector2T<T> _vt(-x, -y);
	return _vt;
}

//�е�
template <typename T>
bool gVector2T<T>::operator==(const gVector2T<T>& _t) const
{
	return x == _t.x&&y == _t.y;
}

//�в���
template <typename T>
bool gVector2T<T>::operator!=(const gVector2T<T>& _t) const
{
	return x != _t.x||y != _t.y;
}

template <typename T>
T&	gVector2T<T>::operator[](int _i)
{
	return ptr()[_i];
}

//������ģ��
template<typename T>
T	gVector2T<T>::Length() const
{
	return sqrt(x*x + y*y);
}

//������ƽ����
template<typename T>
T	gVector2T<T>::LengthSquare() const
{
	return (x*x + y*y);
}

template <typename T>
T	gVector2T<T>::Dot(const gVector2T<T>& _v2) const
{
	return (x*_v2.x + y*_v2.y);
}

template <typename T>
T	gVector2T<T>::Cosine(const gVector2T<T>& _v2) const
{
	T _cosine = Dot(_v2) / sqrt(LengthSquare()*_v2.LengthSquare());
	if (_cosine > 1)
		_cosine = 1;
	else if (_cosine < -1)
		_cosine = -1;
	return _cosine ;
}

template <typename T>
T	gVector2T<T>::Cross(const gVector2T<T>& _v2) const
{
	return x*_v2.y - y*_v2.x;
}

template <typename T>
gVector2T<T> gVector2T<T>::Lerp(const gVector2T<T>& _v2, double _t) const
{
	return *this*(1 - _t) + _v2*_t;
}

//��λ����
template<typename T>
gVector2T<T>	gVector2T<T>::GetNormalize() const
{
	T _n = Length();
	return *this / _n;
}

//תΪ��λ����
template<typename T>
void	gVector2T<T>::toNormalize()
{
	T _n = Length();
	x /= _n;
	y /= _n;
}

//���������׵�ַ
template<typename T>
T*	gVector2T<T>::ptr() 
{
	return (&x);
}

//���������׵�ַ
template<typename T>
const T*	gVector2T<T>::c_ptr() const
{
	return (&x);
}

/***********************************************************************************/

template<typename T>
gVector3T<T>::gVector3T() :x(0), y(0), z(0)
{

}

template <typename T>
gVector3T<T>::gVector3T(T _s) :x(_s), y(_s), z(_s)
{

}

template<typename T>
gVector3T<T>::gVector3T(T _s1, T _s2, T _s3) : x(_s1), y(_s2), z(_s3)
{

}

template<typename T>
gVector3T<T>::gVector3T(gVector2T<T> _v, T _s3) : x(_v.x), y(_v.y), z(_s3)
{

}

template<typename T>
template<typename U>
gVector3T<T>::gVector3T(const gVector3T<U>& _v) : x(_v.x), y(_v.y), z(_v.z)
{

}

//�ӷ�
template<typename T>
gVector3T<T>	gVector3T<T>::operator+(const gVector3T<T>& _v3) const
{
	gVector3T _vt(x + _v3.x, y + _v3.y, z + _v3.z);
	return _vt;
}

//����
template<typename T>
gVector3T<T>	gVector3T<T>::operator-(const gVector3T<T>& _v3) const
{
	gVector3T<T> _vt(x - _v3.x, y - _v3.y, z - _v3.z);
	return _vt;
}

//�ӵ���
template<typename T>
void	gVector3T<T>::operator+=(const gVector3T<T>& _v3)
{
	x += _v3.x;
	y += _v3.y;
	z += _v3.z;
}

//������
template<typename T>
void	gVector3T<T>::operator-=(const gVector3T<T>& _v3)
{
	x -= _v3.x;
	y -= _v3.y;
	z -= _v3.z;
}

//�����˷�
template<typename T>
gVector3T<T>	gVector3T<T>::operator*(const T& _s) const
{
	gVector3T<T> _vt(x * _s, y * _s, z * _s);
	return _vt;
}

//�����˷�
template<typename T>
gVector3T<T>	operator*(const double& _s, const gVector3T<T>& _v3)
{
	return gVector3T<T>(_v3.x*_s, _v3.y*_s, _v3.z*_s);
}

//��������
template<typename T>
gVector3T<T>	gVector3T<T>::operator/(const T& _s) const
{
	gVector3T<T> _vt(x / _s, y / _s, z / _s);
	return _vt;
}

//�˵���
template<typename T>
void	gVector3T<T>::operator*=(const T& _s)
{
	x *= _s;
	y *= _s;
	z *= _s;
}

//������
template<typename T>
void	gVector3T<T>::operator/=(const T& _s)
{
	x /= _s;
	y /= _s;
	z /= _s;
}

//ȡ��
template<typename T>
gVector3T<T>	gVector3T<T>::operator-() const
{
	gVector3T<T> _vt(-x, -y, -z);
	return _vt;
}

//�е�
template <typename T>
bool gVector3T<T>::operator==(const gVector3T<T>& _t) const
{
	return x == _t.x&&y == _t.y&&z == _t.z;
}

//�в���
template <typename T>
bool gVector3T<T>::operator!=(const gVector3T<T>& _t) const
{
	return x != _t.x || y != _t.y || z != _t.z;
}

template <typename T>
T& gVector3T<T>::operator[](int _i)
{
	return ptr()[_i];
}

//������ģ��
template<typename T>
T	gVector3T<T>::Length() const
{
	return sqrt(x*x + y*y + z*z);
}

//������ƽ����
template<typename T>
T	gVector3T<T>::LengthSquare() const
{
	return (x*x + y*y + z*z);
}

template <typename T>
T	gVector3T<T>::Dot(const gVector3T<T>& _v3) const
{
	return (x*_v3.x + y*_v3.y + z*_v3.z);
}

template <typename T>
T	gVector3T<T>::Cosine(const gVector3T<T>& _v3) const
{
	T _cosine = Dot(_v3) / sqrt(LengthSquare()*_v3.LengthSquare());
	if (_cosine > 1)
		_cosine = 1;
	else if (_cosine < -1)
		_cosine = -1;
	return _cosine;
}

template <typename T>
gVector3T<T>	gVector3T<T>::Cross(const gVector3T<T>& _v3) const
{
	gVector3T<T> _ret;
	_ret.x = y*_v3.z - z*_v3.y;
	_ret.y = z*_v3.x - x*_v3.z;
	_ret.z = x*_v3.y - y*_v3.x;
	return _ret;
}

template <typename T>
gVector3T<T> gVector3T<T>::Lerp(const gVector3T<T>& _v3, double _t) const
{
	return *this*(1 - _t) + _v3*_t;
}

//��λ����
template<typename T>
gVector3T<T>	gVector3T<T>::GetNormalize() const
{
	T _n = Length();
	return *this / _n;
}

//תΪ��λ����
template<typename T>
void	gVector3T<T>::toNormalize()
{
	T _n = Length();
	x /= _n;
	y /= _n;
	z /= _n;
}

//���������׵�ַ
template<typename T>
T*	gVector3T<T>::ptr() 
{
	return (&x);
}

//���������׵�ַ
template<typename T>
const T*	gVector3T<T>::c_ptr() const
{
	return (&x);
}

/***********************************************************************************/

template<typename T>
gVector4T<T>::gVector4T() :x(0), y(0), z(0), w(0)
{

}

template <typename T>
gVector4T<T>::gVector4T(T _s) :x(_s), y(_s), z(_s), w(_s)
{

}

template<typename T>
gVector4T<T>::gVector4T(T _s1, T _s2, T _s3, T _s4) : x(_s1), y(_s2), z(_s3), w(_s4)
{

}

template<typename T>
gVector4T<T>::gVector4T(gVector2T<T> _v, T _s3, T _s4) : x(_v.x), y(_v.y), z(_s3), w(_s4)
{

}

template<typename T>
gVector4T<T>::gVector4T(gVector3T<T> _v, T _s4) : x(_v.x), y(_v.y), z(_v.z), w(_s4)
{

}

template<typename T>
template<typename U>
gVector4T<T>::gVector4T(const gVector4T<U>& _v) : x(_v.x), y(_v.y), z(_v.z), w(_v.w)
{

}

//�ӷ�
template<typename T>
gVector4T<T>	gVector4T<T>::operator+(const gVector4T<T>& _v4) const
{
	gVector4T _vt(x + _v4.x, y + _v4.y, z + _v4.z, w + _v4.w);
	return _vt;
}

//����
template<typename T>
gVector4T<T>	gVector4T<T>::operator-(const gVector4T<T>& _v4) const
{
	gVector4T<T> _vt(x - _v4.x, y - _v4.y, z - _v4.z, w - _v4.w);
	return _vt;
}

//�ӵ���
template<typename T>
void	gVector4T<T>::operator+=(const gVector4T<T>& _v4)
{
	x += _v4.x;
	y += _v4.y;
	z += _v4.z;
	w += _v4.w;
}

//������
template<typename T>
void	gVector4T<T>::operator-=(const gVector4T<T>& _v4)
{
	x -= _v4.x;
	y -= _v4.y;
	z -= _v4.z;
	w -= _v4.w;
}

//�����˷�
template<typename T>
gVector4T<T>	gVector4T<T>::operator*(const T& _s) const
{
	gVector4T<T> _vt(x * _s, y * _s, z * _s, w * _s);
	return _vt;
}

//�����˷�
template<typename T>
gVector4T<T>	operator*(const double& _s, const gVector4T<T>& _v4)
{
	return gVector4T<T>(_v4.x*_s, _v4.y*_s, _v4.z*_s, _v4.w*_s);
}

//��������
template<typename T>
gVector4T<T>	gVector4T<T>::operator/(const T& _s) const
{
	gVector4T<T> _vt(x / _s, y / _s, z / _s, w / _s);
	return _vt;
}

//�˵���
template<typename T>
void	gVector4T<T>::operator*=(const T& _s)
{
	x *= _s;
	y *= _s;
	z *= _s;
	w *= _s;
}

//������
template<typename T>
void	gVector4T<T>::operator/=(const T& _s)
{
	x /= _s;
	y /= _s;
	z /= _s;
	w /= _s;
}

//ȡ��
template<typename T>
gVector4T<T>	gVector4T<T>::operator-() const
{
	gVector4T<T> _vt(-x, -y, -z, -w);
	return _vt;
}

//�е�
template <typename T>
bool gVector4T<T>::operator==(const gVector4T<T>& _t) const
{
	return x == _t.x&&y == _t.y&&z == _t.z&&w == _t.w;
}

//�в���
template <typename T>
bool gVector4T<T>::operator!=(const gVector4T<T>& _t) const
{
	return x != _t.x || y != _t.y || z != _t.z || w != _t.w;
}

template <typename T>
T& gVector4T<T>::operator[](int _i)
{
	return ptr()[_i];
}

//������ģ��
template<typename T>
T	gVector4T<T>::Length() const
{
	return sqrt(x*x + y*y + z*z + w*w);
}

//������ƽ����
template<typename T>
T	gVector4T<T>::LengthSquare() const
{
	return (x*x + y*y + z*z + w*w);
}

template <typename T>
T	gVector4T<T>::Dot(const gVector4T<T>& _v4) const
{
	return (x*_v4.x + y*_v4.y + z*_v4.z + w*_v4.w);
}

template <typename T>
T	gVector4T<T>::Cosine(const gVector4T<T>& _v4) const
{
	T _cosine = Dot(_v4) / sqrt(LengthSquare()*_v4.LengthSquare());
	if (_cosine > 1)
		_cosine = 1;
	else if (_cosine < -1)
		_cosine = -1;
	return _cosine;
}

template <typename T>
gVector4T<T> gVector4T<T>::Lerp(const gVector4T<T>& _v4, double _t) const
{
	return *this*(1 - _t) + _v4*_t;
}

//��λ����
template<typename T>
gVector4T<T>	gVector4T<T>::GetNormalize() const
{
	T _n = Length();
	return *this / _n;
}

//תΪ��λ����
template<typename T>
void	gVector4T<T>::toNormalize()
{
	T _n = Length();
	x /= _n;
	y /= _n;
	z /= _n;
	w /= _n;
}

//���iλ����
template<typename T>
T	gVector4T<T>::value(unsigned _i) const
{
	return (&x)[_i];
}

//���������׵�ַ
template<typename T>
T*	gVector4T<T>::ptr() 
{
	return (&x);
}

//���������׵�ַ
template<typename T>
const T*	gVector4T<T>::c_ptr() const
{
	return (&x);
}

template <typename T>
const gVector3T<T> gVector4T<T>::GetXYZ() const
{
	return gVector3T<T>(x, y, z);
}


typedef gVector2T<char>				gVector2c;
typedef gVector2T<short>			gVector2s;
typedef gVector2T<int>				gVector2i;
typedef gVector2T<unsigned char>	gVector2uc;
typedef gVector2T<unsigned short>	gVector2us;
typedef gVector2T<unsigned int>		gVector2ui;
typedef gVector2T<float>			gVector2f;
typedef gVector2T<double>			gVector2d;


typedef gVector3T<char>				gVector3c;
typedef gVector3T<short>			gVector3s;
typedef gVector3T<int>				gVector3i;
typedef gVector3T<unsigned char>	gVector3uc;
typedef gVector3T<unsigned short>	gVector3us;
typedef gVector3T<unsigned int>		gVector3ui;
typedef gVector3T<float>			gVector3f;
typedef gVector3T<double>			gVector3d;

typedef gVector4T<char>				gVector4c;
typedef gVector4T<short>			gVector4s;
typedef gVector4T<int>				gVector4i;
typedef gVector4T<unsigned char>	gVector4uc;
typedef gVector4T<unsigned short>	gVector4us;
typedef gVector4T<unsigned int>		gVector4ui;
typedef gVector4T<float>			gVector4f;
typedef gVector4T<double>			gVector4d;

//32λ��ɫ����
struct gColor32bit :public gVector4uc
{
	gColor32bit(){
		x = 255;
		y = 255;
		z = 255;
		w = 255;
	}
	gColor32bit(const gVector4uc& _rgba){
		*this = _rgba;
	}
	gColor32bit(gVector3uc _rgb, unsigned char _alpha = 255){
		*this = { _rgb, _alpha };
	}
	gColor32bit(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _alpha = 255){
		x = _r;
		y = _g;
		z = _b;
		w = _alpha;
	}
	unsigned char& r(){
		return x;
	}
	unsigned char& g(){
		return y;
	}
	unsigned char& b(){
		return z;
	}
	unsigned char& a(){
		return w;
	}
	const gVector4f GetVector4f() const
	{
		gVector4f _vec = *this;
		return _vec / 255.0;
	}
};

#endif // GEOVECTOR