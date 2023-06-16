#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class EnemyBullet {
public:
	void Initialize(Model* model, const Vector3& pos, const Vector3& velocity);
	void Update();
	void Draw(ViewProjection& view);
	Vector3 velocity_;

private:
	WorldTransform would_;
	Model* model_;
	uint32_t texturehandle_;
};
