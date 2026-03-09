#ifndef _THREE_MATH_HXX
#define _THREE_MATH_HXX

#include <vector>
#include <string>
#include <map>
#include <set>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include<windows.h>

using namespace std;

namespace three {

class Matrix4
{
public:
	double elements[16];

	Matrix4()
	{
		double _elements[16] = {1, 0, 0, 0,	0, 1, 0, 0,	0, 0, 1, 0,	0, 0, 0, 1};
		memcpy(elements,_elements,sizeof(_elements));
	}

	Matrix4(vector<double> _m)
	{
		for (int i = 0; i < 16; i++)
		{
			elements[i] = _m[i];
		}
	}

	Matrix4 multiplyMatrices(const Matrix4& a, const Matrix4& b) {

		double ae[16];
		double be[16];

		memcpy(ae, a.elements, sizeof(a.elements));
		memcpy(be, b.elements, sizeof(b.elements));

		double a11 = ae[ 0 ], a12 = ae[ 4 ], a13 = ae[ 8 ], a14 = ae[ 12 ];
		double a21 = ae[ 1 ], a22 = ae[ 5 ], a23 = ae[ 9 ], a24 = ae[ 13 ];
		double a31 = ae[ 2 ], a32 = ae[ 6 ], a33 = ae[ 10 ], a34 = ae[ 14 ];
		double a41 = ae[ 3 ], a42 = ae[ 7 ], a43 = ae[ 11 ], a44 = ae[ 15 ];

		double b11 = be[ 0 ], b12 = be[ 4 ], b13 = be[ 8 ], b14 = be[ 12 ];
		double b21 = be[ 1 ], b22 = be[ 5 ], b23 = be[ 9 ], b24 = be[ 13 ];
		double b31 = be[ 2 ], b32 = be[ 6 ], b33 = be[ 10 ], b34 = be[ 14 ];
		double b41 = be[ 3 ], b42 = be[ 7 ], b43 = be[ 11 ], b44 = be[ 15 ];

		elements[ 0 ] = a11 * b11 + a12 * b21 + a13 * b31 + a14 * b41;
		elements[ 4 ] = a11 * b12 + a12 * b22 + a13 * b32 + a14 * b42;
		elements[ 8 ] = a11 * b13 + a12 * b23 + a13 * b33 + a14 * b43;
		elements[ 12 ] = a11 * b14 + a12 * b24 + a13 * b34 + a14 * b44;

		elements[ 1 ] = a21 * b11 + a22 * b21 + a23 * b31 + a24 * b41;
		elements[ 5 ] = a21 * b12 + a22 * b22 + a23 * b32 + a24 * b42;
		elements[ 9 ] = a21 * b13 + a22 * b23 + a23 * b33 + a24 * b43;
		elements[ 13 ] = a21 * b14 + a22 * b24 + a23 * b34 + a24 * b44;

		elements[ 2 ] = a31 * b11 + a32 * b21 + a33 * b31 + a34 * b41;
		elements[ 6 ] = a31 * b12 + a32 * b22 + a33 * b32 + a34 * b42;
		elements[ 10 ] = a31 * b13 + a32 * b23 + a33 * b33 + a34 * b43;
		elements[ 14 ] = a31 * b14 + a32 * b24 + a33 * b34 + a34 * b44;

		elements[ 3 ] = a41 * b11 + a42 * b21 + a43 * b31 + a44 * b41;
		elements[ 7 ] = a41 * b12 + a42 * b22 + a43 * b32 + a44 * b42;
		elements[ 11 ] = a41 * b13 + a42 * b23 + a43 * b33 + a44 * b43;
		elements[ 15 ] = a41 * b14 + a42 * b24 + a43 * b34 + a44 * b44;

		return *this;
	}

	Matrix4 multiplyMatrices(const vector<double>& ae, const vector<double>& be) {

		//double te[16];
		//memcpy(te, elements, sizeof(elements));

		double a11 = ae[ 0 ], a12 = ae[ 4 ], a13 = ae[ 8 ], a14 = ae[ 12 ];
		double a21 = ae[ 1 ], a22 = ae[ 5 ], a23 = ae[ 9 ], a24 = ae[ 13 ];
		double a31 = ae[ 2 ], a32 = ae[ 6 ], a33 = ae[ 10 ], a34 = ae[ 14 ];
		double a41 = ae[ 3 ], a42 = ae[ 7 ], a43 = ae[ 11 ], a44 = ae[ 15 ];

		double b11 = be[ 0 ], b12 = be[ 4 ], b13 = be[ 8 ], b14 = be[ 12 ];
		double b21 = be[ 1 ], b22 = be[ 5 ], b23 = be[ 9 ], b24 = be[ 13 ];
		double b31 = be[ 2 ], b32 = be[ 6 ], b33 = be[ 10 ], b34 = be[ 14 ];
		double b41 = be[ 3 ], b42 = be[ 7 ], b43 = be[ 11 ], b44 = be[ 15 ];

		elements[ 0 ] = a11 * b11 + a12 * b21 + a13 * b31 + a14 * b41;
		elements[ 4 ] = a11 * b12 + a12 * b22 + a13 * b32 + a14 * b42;
		elements[ 8 ] = a11 * b13 + a12 * b23 + a13 * b33 + a14 * b43;
		elements[ 12 ] = a11 * b14 + a12 * b24 + a13 * b34 + a14 * b44;

		elements[ 1 ] = a21 * b11 + a22 * b21 + a23 * b31 + a24 * b41;
		elements[ 5 ] = a21 * b12 + a22 * b22 + a23 * b32 + a24 * b42;
		elements[ 9 ] = a21 * b13 + a22 * b23 + a23 * b33 + a24 * b43;
		elements[ 13 ] = a21 * b14 + a22 * b24 + a23 * b34 + a24 * b44;

		elements[ 2 ] = a31 * b11 + a32 * b21 + a33 * b31 + a34 * b41;
		elements[ 6 ] = a31 * b12 + a32 * b22 + a33 * b32 + a34 * b42;
		elements[ 10 ] = a31 * b13 + a32 * b23 + a33 * b33 + a34 * b43;
		elements[ 14 ] = a31 * b14 + a32 * b24 + a33 * b34 + a34 * b44;

		elements[ 3 ] = a41 * b11 + a42 * b21 + a43 * b31 + a44 * b41;
		elements[ 7 ] = a41 * b12 + a42 * b22 + a43 * b32 + a44 * b42;
		elements[ 11 ] = a41 * b13 + a42 * b23 + a43 * b33 + a44 * b43;
		elements[ 15 ] = a41 * b14 + a42 * b24 + a43 * b34 + a44 * b44;

		return *this;
	}
};

class Vector3
{
public:
	double x, y, z;
	Vector3(double vx = 0., double vy = 0., double vz = 0.) : x(vx), y(vy), z(vz) {}

	double dot(const Vector3& v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	Vector3 cross(const Vector3& v)               // ( y1 * z2 - y2 *z1 , x2 * z1 - z2 * x1 , x1 * y2 - x2 * y1 )
	{
		Vector3 ret(y * v.z - v.y *z , v.x * z - v.z * x , x * v.y - v.x * y);
		return ret;
	}

	void normalize()
	{
		double dis = sqrt(x*x + y*y + z*z);
		if (fabs(dis) < 1e-7)
		{
			x = 0.0;
			y = 0.0;
			z = 1.0;
		}
		else
		{
			x = x / dis;
			y = y / dis;
			z = z / dis;
		}
	}

	void Dump()
	{
		cout.precision(20);
		cout <<  x << ";  " << y << ";  " << z << endl;
	}

	// 与nodejs一致
	template<typename T>
	void fromBufferAttribute(const T& buf, int i)
	{
		x = buf[i * 3];
		y = buf[i * 3 + 1];
		z = buf[i * 3 + 2];
	}

	Vector3 set( double vx, double vy, double vz ) 
	{
		x = vx;
		y = vy;
		z = vz;

		return *this;
	}

	Vector3 addScaledVector(const Vector3& v, double s)
	{
		x += v.x * s;
		y += v.y * s;
		z += v.z * s;
		return *this;
	}

	Vector3 copy(const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}

	Vector3 add(const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	Vector3 sub(const Vector3& v) 
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	Vector3 applyMatrix4(const Matrix4 &m) {
		double e[16];
		memcpy(e, m.elements, sizeof(m.elements));
		double _x = x;
		double _y = y;
		double _z = z;
		double w = 1 / ( e[ 3 ] * _x + e[ 7 ] * _y + e[ 11 ] * _z + e[ 15 ] );

		x = ( e[ 0 ] * _x + e[ 4 ] * _y + e[ 8 ] * _z + e[ 12 ] ) * w;
		y = ( e[ 1 ] * _x + e[ 5 ] * _y + e[ 9 ] * _z + e[ 13 ] ) * w;
		z = ( e[ 2 ] * _x + e[ 6 ] * _y + e[ 10 ] * _z + e[ 14 ] ) * w;

		return *this;
	}

	Vector3 applyMatrix4(const vector<double> &e) {

		double _x = x;
		double _y = y;
		double _z = z;

		double w = 1 / ( e[ 3 ] * x + e[ 7 ] * y + e[ 11 ] * z + e[ 15 ] );

		x = ( e[ 0 ] * _x + e[ 4 ] * _y + e[ 8 ] * _z + e[ 12 ] ) * w;
		y = ( e[ 1 ] * _x + e[ 5 ] * _y + e[ 9 ] * _z + e[ 13 ] ) * w;
		z = ( e[ 2 ] * _x + e[ 6 ] * _y + e[ 10 ] * _z + e[ 14 ] ) * w;

		return *this;
	}

	//Operator Overload ： +
	Vector3 operator+(const Vector3& ref)
	{
		Vector3 ret(x + ref.x, y + ref.y, z + ref.z);
		return ret;
	}

	//Operator Overload ： -
	Vector3 operator-(const Vector3& ref)
	{
		return Vector3(x - ref.x, y - ref.y, z - ref.z);
	}

	//Operator Overload ： *         Dot
	Vector3 operator*(const Vector3& ref)
	{
		return x * ref.x + y * ref.y + z * ref.z;
	}


	//Operator Overload ： ^         Cross
	Vector3 operator^(const Vector3& ref)
	{
		return Vector3(y * ref.z - ref.y *z , ref.x * z - ref.z * x , x * ref.y - ref.x * y);
	}

	Vector3& operator=(const Vector3 &ref)
	{
		x = ref.x;
		y = ref.y;
		z = ref.z;
		return *this;
	}

};

template<typename T>
class Vector4
{
public:
	T x, y, z, w;
	Vector4(T vx = 0., T vy = 0., T vz = 0., T vw = 1.) : x(vx), y(vy), z(vz), w(vw) {}

	template<typename T>
	void fromBufferAttribute(const T& buf, int i)
	{
		x = buf[i * 4];
		y = buf[i * 4 + 1];
		z = buf[i * 4 + 2];
		w = buf[i * 4 + 3];
	}

	T getComponent(int index) {

		switch ( index ) {

		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default:
			return 0;
		}

	}
	Vector4 <T> applyMatrix4(const Matrix4 &m) {

		T e[16];
		memcpy(e, m.elements, sizeof(m.elements));

		x = e[ 0 ] * x + e[ 4 ] * y + e[ 8 ] * z + e[ 12 ] * w;
		y = e[ 1 ] * x + e[ 5 ] * y + e[ 9 ] * z + e[ 13 ] * w;
		z = e[ 2 ] * x + e[ 6 ] * y + e[ 10 ] * z + e[ 14 ] * w;
		w = e[ 3 ] * x + e[ 7 ] * y + e[ 11 ] * z + e[ 15 ] * w;

		return *this;

	}
	void Dump()
	{
		cout << "x: " << x << endl;
		cout << "y: " << y << endl;
		cout << "z: " << z << endl;
		cout << "w: " << w << endl;
	}
};

} // three
#endif