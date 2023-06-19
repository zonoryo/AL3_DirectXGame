#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);
	model_ = model;
	texturehandle_ = TextureManager::Load("GHOST.png");
	would_.Initialize();
	would_.translation_ = pos;
	velocity_ = velocity;
	//Fire();
	//接近フェーズ初期化
	Approach();
}

void Enemy::Update() { 
	enemybullets_.remove_if([](EnemyBullet* enemybullet) {
		if (enemybullet->IsDead()) {
			delete enemybullet;
			return true;
		}
		return false;
	});
	would_.translation_.x -= velocity_.x;
	would_.translation_.y -= velocity_.y;
	would_.translation_.z -= velocity_.z;
	would_.UpdateMatrix(); 
	//状態遷移
	switch (phase_) {
	case Phase::Approach:
	default:
		//ここで敵の動きを決める。
		//接近
		/*would_.translation_.x += velocity_.x;
		would_.translation_.y += velocity_.y;
		would_.translation_.z += velocity_.z;*/
		if (would_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}

		EnemyBulletTimer_--;
		if (EnemyBulletTimer_ == 0) {
			Fire();
			EnemyBulletTimer_ = kFireInterval;
		}
		break;
	case Phase::Leave:
		//離脱
		would_.translation_.x += 10;
		would_.translation_.y += 10;
		would_.translation_.z += 10;
		break;
	}

	if (enemybullet_) {
		enemybullet_->Update();
	}
	
}

void Enemy::Draw(ViewProjection& view) { 
  model_->Draw(would_, view, texturehandle_);
	if (enemybullet_) {
		enemybullet_->Draw(view);
	}
}

void Enemy::Fire() {
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, would_.translation_, velocity);

	enemybullet_ = newBullet;
	
}

Enemy::~Enemy() {
	for (EnemyBullet* enemybullet:enemybullets_) {
		delete enemybullet;
	}
}

void Enemy::Approach() { EnemyBulletTimer_ = kFireInterval; }