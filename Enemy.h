#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
class Player;

class Enemy {
public:
	// 初期化

	void Initialize(Model* model, const Vector3& pos, const Vector3& velocity);

	// 更新
	void Update();

	// 描画
	void Draw(ViewProjection& view);

	void Fire();
	// 衝突を検出したら呼び出されるコールバック
	void OnCollision();
	Vector3 velocity_;

	enum class Phase {
		Approach, //接近
		Leave, //離脱
	};
	
	~Enemy();

	std::list<EnemyBullet*> enemybullets_;

	//発射間隔
	static const int kFireInterval = 60;
	//接近フェーズ初期化
	void Approach();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();
	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return enemybullets_; }

private:
	
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t texturehandle_;
	Phase phase_ = Phase::Approach;

	EnemyBullet* enemybullet_ = nullptr;

	
	int32_t EnemyBulletTimer_ = 0;

	//自キャラ
	Player* player_ = nullptr;
	
};
