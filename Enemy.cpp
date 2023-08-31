#include "Enemy.h"
#include <cassert>
#include "Player.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "GameScene.h"
#include "ImGuiManager.h"
void Enemy::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);
	model_ = model;
	texturehandle_ = TextureManager::Load("box.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	velocity_ = velocity;
}

void Enemy::Update() {
	if (!isDead_) {
		worldTransform_.translation_.x -= velocity_.x;
		worldTransform_.translation_.y -= velocity_.y;
		worldTransform_.translation_.z -= velocity_.z;

		if (worldTransform_.translation_.x > 15 || worldTransform_.translation_.x < -15) {
			velocity_.x *= -1;
		}

		
	}
	if (shrinkFlag_ == true) {
		worldTransform_.scale_.x -= shrinkSpeed_;
		worldTransform_.scale_.y -= shrinkSpeed_;
		worldTransform_.scale_.z -= shrinkSpeed_;
	}
	worldTransform_.UpdateMatrix();
	/*ImGui::Begin("Enemy");
	ImGui::Text("position : %f", worldTransform_.scale_.x);
	ImGui::End()*/;
}

void Enemy::Draw(ViewProjection& view) {
	if (worldTransform_.scale_.x > 0 && worldTransform_.scale_.y > 0 &&
	    worldTransform_.scale_.z > 0) {
		model_->Draw(worldTransform_, view, texturehandle_);
	}
}
void Enemy::SetParent(const WorldTransform* parent) {
	 worldTransform_.parent_ = parent; 
}
void Enemy::OnCollision() {
	shrinkFlag_ = true;
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

