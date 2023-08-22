﻿#pragma once

#include "Vector3.h"

/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
};

// スケーリング行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// 回転行列
Matrix4x4 MakeRotationXMatrix(float theta);
Matrix4x4 MakeRotationYMatrix(float theta);
Matrix4x4 MakeRotationZMatrix(float theta);

// 平行移動行列
Matrix4x4 MakeTransfomelateMatrix(const Vector3& trans);

// アフィン行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& tanslate);

// 代入演算子オーバーロード
Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2);

// 2項演算子
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 Inverse(const Matrix4x4& m);

Vector3 Normalize(const Vector3& v);

Vector3 Add(const Vector3& v1, const Vector3& v2);

Vector3 Multiply(float scalar, const Vector3& v);

Vector3& operator*=(Vector3& v, float s);

const Vector3 operator*(const Vector3& v, float s);

Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);