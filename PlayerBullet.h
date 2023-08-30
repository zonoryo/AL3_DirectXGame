#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class PlayerBullet {
public:
	void Initialize(Model* model, const Vector3& pos, const Vector3& velocity);
	void Update();
	void Draw(ViewProjection& view);
	Vector3 velocity_;
	bool IsDead() const { return isDead_; }
	// 衝突を検出したら呼び出されるコールバック
	void OnCollision();

	Vector3 GetWorldPosition();
	float GetRadius() const { return radius_; }

	// 親となるワールドトランスフォームをセット
	//void SetParent(const WorldTransform* parent);

private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t texturehandle_;
	//寿命
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
	float radius_= 2;

	int frameCounter_ = 0; // フレームカウンタ
};
