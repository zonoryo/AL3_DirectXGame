#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Enemy {
public:
	// 初期化

	void Initialize(Model* model, const Vector3& pos, const Vector3& velocity);

	// 更新
	void Update();

	// 描画
	void Draw(ViewProjection& view);

	Vector3 velocity_;

	enum class Phase {
		Approach, //接近
		Leave, //離脱
	};
	

private:
	
	WorldTransform would_;
	Model* model_;
	uint32_t texturehandle_;
	Phase phase_ = Phase::Approach;
};
