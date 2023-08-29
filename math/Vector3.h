#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;


	    // ベクトル同士の減算演算子オーバーロード
	 Vector3 operator-(const Vector3& other) const {
		 return Vector3{x - other.x, y - other.y, z - other.z};
	 }

	 // ベクトルの長さの二乗を計算するメソッド
	 float LengthSquared() const { return x * x + y * y + z * z; }
};