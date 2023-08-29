#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Player;
class GameScene;
class Enemy {
public:
	// 初期化

	void Initialize(Model* model, const Vector3& pos, const Vector3& velocity);

	// 更新
	void Update();

	// 描画
	void Draw(ViewProjection& view);

	Vector3 velocity_;

	// 衝突を検出したら呼び出されるコールバック
	void OnCollision();

	Vector3 GetWorldPosition();
	bool IsDead() const { return isDead_; }

private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t texturehandle_;
	// デスフラグ
	bool isDead_ = false;
};