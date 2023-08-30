#include "Enemy.h"
#include <cassert>
#include "Player.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "GameScene.h"
void Enemy::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);
	model_ = model;
	texturehandle_ = TextureManager::Load("cube.jpg");
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	velocity_ = velocity;
}

void Enemy::Update() {

	worldTransform_.translation_.x -= velocity_.x;
	worldTransform_.translation_.y -= velocity_.y;
	worldTransform_.translation_.z -= velocity_.z;
	if (worldTransform_.translation_.x > 40 || worldTransform_.translation_.x < -40) {
		velocity_.x *= -1;
	}
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& view) {
	if (!isDead_) {
		model_->Draw(worldTransform_, view, texturehandle_);
	}
}
void Enemy::SetParent(const WorldTransform* parent) {
	 worldTransform_.parent_ = parent; 
}
void Enemy::OnCollision() {
	isDead_ = true; // isDead_ フラグを true に設定
}

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

