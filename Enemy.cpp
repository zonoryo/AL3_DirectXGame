#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);
	model_ = model;
	texturehandle_ = TextureManager::Load("cube.jpg");
	would_.Initialize();
	would_.translation_ = pos;
	velocity_ = velocity;
}

void Enemy::Update() { 
	
	would_.translation_.x -= velocity_.x;
	would_.translation_.y -= velocity_.y;
	would_.translation_.z -= velocity_.z;
	would_.UpdateMatrix(); 
	//状態遷移
	switch (phase_) {
	case Phase::Approach:
	default:
		//接近
		/*would_.translation_.x += velocity_.x;
		would_.translation_.y += velocity_.y;
		would_.translation_.z += velocity_.z;*/
		if (would_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		//離脱
		would_.translation_.x += 10;
		would_.translation_.y += 10;
		would_.translation_.z += 10;
		break;
	}

	
}

void Enemy::Draw(ViewProjection& view) { 
  model_->Draw(would_, view, texturehandle_); 
}

void Enemy::Fire() {
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, would_.translation_, velocity);

	bullet_ = newBullet;
	
}