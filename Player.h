#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include <Input.h>
#include "PlayerBullet.h"
#include <list>


class Player {
public:
	// 初期化

	void Initialize(Model* model, uint32_t textureHandle, Vector3 pos);

	// 更新
	void Update();

	// 描画
	void Draw(ViewProjection&viewProjection);
	//親となるワールドトランスフォームをセット
	void SetParent(const WorldTransform* parent);
	// 攻撃
	void Attack();
	//衝突を検出したら呼び出されるコールバック
	void OnCollision();

	~Player();

	std::list<PlayerBullet*> bullets_;
	
	Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
	//ワールド座標を取得
	Vector3 GetWorldPosition();
	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	

	// キーボード入力
	Input* input_ = nullptr;

	//弾
	PlayerBullet* bullet_ = nullptr;

	
	
};
