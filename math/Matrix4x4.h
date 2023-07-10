#pragma once
#include "Vector3.h"
#define USE_MATH_DEFINES
#include <math.h>
#include <assert.h>

/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
};

//スケーリング行列
Matrix4x4 MakeScaleMatrix(const Vector3 scale);


//回転行列 X
Matrix4x4 MakeRotX(float theta);

// 回転行列 Y
Matrix4x4 MakeRotY(float theta);

// 回転行列 Z
Matrix4x4 MakeRotZ(float theta);

//平行移動行列
Matrix4x4 MakeTranslate(const Vector3& trans);

//代入演算子オーバーロード
Matrix4x4& operator*=(Matrix4x4 & m1, const Matrix4x4& m2);

//2公演算子オーバーロード
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

Vector3& operator+=(Vector3& v1, const Vector3& v2);
const Vector3 operator+(const Vector3& v1, const Vector3& v2);

Vector3& operator-=(Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);

Vector3& operator*=(Vector3& v, float s);
const Vector3 operator*(const Vector3& v, float s);

Vector3& operator/=(Vector3& v, float s);
const Vector3 operator/(const Vector3& v, float s);

//アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);

// 長さ（ノルム）
float Length(const Vector3& v);

// 正規化
Vector3 Normalize(const Vector3& v);

// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

// Liner InterPolation
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
