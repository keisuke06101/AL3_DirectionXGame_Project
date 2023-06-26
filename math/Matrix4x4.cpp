#include "Matrix4x4.h"
#include <cmath>

Matrix4x4 MakeScaleMatrix(const Vector3 scale)
{
	Matrix4x4 result;

	result = {scale.x, 0.0f,    0.0f,    0.0f,
		      0.0f,    scale.y, 0.0f,    0.0f,
	          0.0f,    0.0f,    scale.z, 0.0f,
		      0.0f,    0.0f,    0.0f,    1.0f};

	return result;
};

Matrix4x4 MakeRotX(float theta) {
	float sin = std::sinf(theta);
	float cos = std::cosf(theta);

	Matrix4x4 result;

	result = {1.0f, 0.0f,    0.0f,   0.0f, 
		      0.0f, cos,     sin,    0.0f, 
		      0.0f, -sin,    cos,    0.0f, 
		      0.0f, 0.0f,    0.0f,   1.0f};

	return result;
};

Matrix4x4 MakeRotY(float theta) {
	float sin = std::sinf(theta);
	float cos = std::cosf(theta);

	Matrix4x4 result;

	result = {cos,    0.0f, -sin,    0.0f, 
		      0.0f,   1.0f, 0.0f,    0.0f, 
		      sin,    0.0f, cos,     0.0f, 
		      0.0f,   0.0f, 0.0f,    1.0f};

	return result;
};

Matrix4x4 MakeRotZ(float theta) {
	float sin = std::sinf(theta);
	float cos = std::cosf(theta);

	Matrix4x4 result;

	result = {cos,     sin,    0.0f, 0.0f,
		      -sin,    cos,    0.0f, 0.0f,
		      0.0f,    0.0f,   1.0f, 0.0f, 
		      0.0f,    0.0f,   0.0f, 1.0f};

	return result;
}
Matrix4x4 MakeTranslate(const Vector3& trans) {
	Matrix4x4 result;

	result = 
	{
		1.0f,    0.0f,    0.0f,    0.0f, 
		0.0f,    1.0f,    0.0f,    0.0f, 
		0.0f,    0.0f,    1.0f,    0.0f, 
		trans.x, trans.y, trans.z, 1.0f
	};
	return result; 
}
Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2) 
{ 
	Matrix4x4 result = {};
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			for (size_t k = 0; k < 4; k++)
			{
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	m1 = result;
	return m1;
}
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result = m1;


	return result *= m2;
}
Vector3& operator+=(Vector3& v1, const Vector3& v2) {
	// TODO: return ステートメントをここに挿入します
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;

	return v1;
}
const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 tmp(v1);
	return tmp += v2;
}
Vector3& operator-=(Vector3& v1, const Vector3& v2)
{
	// TODO: return ステートメントをここに挿入します
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;

	return v1;
}
const Vector3 operator-(const Vector3& v1, const Vector3& v2) 
{ 
	Vector3 tmp(v1);
	return tmp -= v2;
}
Vector3& operator*=(Vector3& v, float s) {
	// TODO: return ステートメントをここに挿入します
	v.x *= s;
	v.y *= s;
	v.z *= s;

	return v;
}
const Vector3 operator*(const Vector3& v, float s) 
{
	Vector3 tmp(v);
	return tmp *= s;
}
Vector3& operator/=(Vector3& v, float s) {
	// TODO: return ステートメントをここに挿入します
	v.x /= s;
	v.y /= s;
	v.z /= s;

	return v;
}
const Vector3 operator/(const Vector3& v, float s) 
{ 
	Vector3 tmp(v);
	return tmp /= s;
}
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate)
{
	//スケーリング行列
	Matrix4x4 matScale = MakeScaleMatrix(scale);

	//回転行列
	Matrix4x4 matRotX = MakeRotX(rot.x);
	Matrix4x4 matRotY = MakeRotY(rot.y); 
	Matrix4x4 matRotZ = MakeRotZ(rot.z);
	//合成（Z * X * Y）
	Matrix4x4 matRot = matRotZ * matRotX * matRotY;

	//平行移動行列
	Matrix4x4 matTrans = MakeTranslate(translate);

	//合成
	Matrix4x4 result = matScale * matRot * matTrans;

	return result;
}

Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result;
	result = {
	    width / 2,
	    0.0f,
	    0.0f,
	    0.0f,
	    0.0f,
	    -height / 2,
	    0.0f,
	    0.0f,
	    0.0f,
	    0.0f,
	    maxDepth - minDepth,
	    0.0f,
	    left + width / 2,
	    top + height / 2,
	    minDepth,
	    1.0f};

	return result;
}

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 resultInverse;
	float determinant = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
	                    m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
	                    m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -

	                    m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
	                    m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
	                    m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -

	                    m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
	                    m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
	                    m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +

	                    m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
	                    m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
	                    m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +

	                    m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
	                    m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
	                    m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -

	                    m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
	                    m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
	                    m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -

	                    m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
	                    m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
	                    m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +

	                    m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
	                    m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
	                    m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	assert(determinant != 0.0f);
	float determinantRecp = 1.0f / determinant;

	// 1行目
	resultInverse.m[0][0] =
	    (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
	     m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
	     m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]) *
	    determinantRecp;

	resultInverse.m[0][1] =
	    -(m.m[0][1] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][1] +
	      m.m[0][3] * m.m[2][1] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][1] -
	      m.m[0][2] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][2]) *
	    determinantRecp;

	resultInverse.m[0][2] =
	    (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
	     m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
	     m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]) *
	    determinantRecp;

	resultInverse.m[0][3] =
	    -(m.m[0][1] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][1] +
	      m.m[0][3] * m.m[1][1] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][1] -
	      m.m[0][2] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][2]) *
	    determinantRecp;

	// 2行目
	resultInverse.m[1][0] =
	    -(m.m[1][0] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][0] +
	      m.m[1][3] * m.m[2][0] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][0] -
	      m.m[1][2] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][2]) *
	    determinantRecp;

	resultInverse.m[1][1] =
	    (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
	     m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
	     m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]) *
	    determinantRecp;

	resultInverse.m[1][2] =
	    -(m.m[0][0] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][0] +
	      m.m[0][3] * m.m[1][0] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][0] -
	      m.m[0][2] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][2]) *
	    determinantRecp;

	resultInverse.m[1][3] =
	    (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
	     m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
	     m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]) *
	    determinantRecp;

	// 3行目
	resultInverse.m[2][0] =
	    (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
	     m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
	     m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]) *
	    determinantRecp;

	resultInverse.m[2][1] =
	    -(m.m[0][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][0] +
	      m.m[0][3] * m.m[2][0] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][0] -
	      m.m[0][1] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][1]) *
	    determinantRecp;

	resultInverse.m[2][2] =
	    (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
	     m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
	     m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]) *
	    determinantRecp;

	resultInverse.m[2][3] =
	    -(m.m[0][0] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][0] +
	      m.m[0][3] * m.m[1][0] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][0] -
	      m.m[0][1] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][1]) *
	    determinantRecp;

	// 4行目
	resultInverse.m[3][0] =
	    -(m.m[1][0] * m.m[2][1] * m.m[3][2] + m.m[1][1] * m.m[2][2] * m.m[3][0] +
	      m.m[1][2] * m.m[2][0] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][0] -
	      m.m[1][1] * m.m[2][0] * m.m[3][2] - m.m[1][0] * m.m[2][2] * m.m[3][1]) *
	    determinantRecp;

	resultInverse.m[3][1] =
	    (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
	     m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
	     m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]) *
	    determinantRecp;

	resultInverse.m[3][2] =
	    -(m.m[0][0] * m.m[1][1] * m.m[3][2] + m.m[0][1] * m.m[1][2] * m.m[3][0] +
	      m.m[0][2] * m.m[1][0] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][0] -
	      m.m[0][1] * m.m[1][0] * m.m[3][2] - m.m[0][0] * m.m[1][2] * m.m[3][1]) *
	    determinantRecp;

	resultInverse.m[3][3] =
	    (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
	     m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
	     m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]) *
	    determinantRecp;

	return resultInverse;
}



Vector3 Multiply(float scalar, const Vector3& v) {
	return {scalar * v.x, scalar * v.y, scalar * v.z};
}

float Length(const Vector3& v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

Vector3 Normalize(const Vector3& v)
{
	float length = Length(v);
	return {v.x / length, v.y / length, v.z / length};
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) 
{
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
	           1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
	           1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
	           1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
	          1.0f * matrix.m[3][3];
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}