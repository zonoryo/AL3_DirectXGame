#include "Enemy.h"
#include <cassert>
#include "Player.h"
#define _USE_MATH_DEFINES
#include <math.h>
void Enemy::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);
	model_ = model;
	texturehandle_ = TextureManager::Load("GHOST.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
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
	worldTransform_.translation_.x -= velocity_.x;
	worldTransform_.translation_.y -= velocity_.y;
	worldTransform_.translation_.z -= velocity_.z;
	worldTransform_.UpdateMatrix(); 
	//状態遷移
	switch (phase_) {
	case Phase::Approach:
	default:
		//ここで敵の動きを決める。
		//接近
		/*world_.translation_.x += velocity_.x;
		world_.translation_.y += velocity_.y;
		world_.translation_.z += velocity_.z;*/
		if (worldTransform_.translation_.z < 0.0f) {
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
		worldTransform_.translation_.x += 0.5;
		worldTransform_.translation_.y += 0.5;
		worldTransform_.translation_.z += 0.5;
		break;
	}

	for (EnemyBullet* enemybullet : enemybullets_) {
		enemybullet->Update();
	}
	
}

void Enemy::Draw(ViewProjection& view) { 
  model_->Draw(worldTransform_, view, texturehandle_);
	for (EnemyBullet* enemybullet : enemybullets_) {
		enemybullet->Draw(view);
	}
}

void Enemy::Fire() {
	assert(player_);
	const float kBulletSpeed = -1.0f;
	Vector3 playerpos = player_->GetWorldPosition();
	Vector3 enemypos = GetWorldPosition();
	Vector3 differencepos;
	differencepos.x =playerpos.x - enemypos.x;
	differencepos.y =playerpos.y - enemypos.y;
	differencepos.z =playerpos.z - enemypos.z;
	//正規化
	float length = sqrt(differencepos.x * differencepos.x + differencepos.y * differencepos.y+differencepos.z*differencepos.z);
	if (length != 0.0f) {
		differencepos.x /= length;
		differencepos.y /= length;
		differencepos.z /= length;
	}
	//ベクトルの長さを速さに合わせる
	differencepos.x *= kBulletSpeed;
	differencepos.y *= kBulletSpeed;
	differencepos.z *= kBulletSpeed;

	//Vector3 velocity(0, 0, kBulletSpeed);
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, differencepos);

	enemybullet_ = newBullet;
	enemybullets_.push_back(newBullet);
	
}

Enemy::~Enemy() {
	for (EnemyBullet* enemybullet : enemybullets_) {

		delete enemybullet;
	}
}

void Enemy::Approach() { EnemyBulletTimer_ = kFireInterval; }


Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}