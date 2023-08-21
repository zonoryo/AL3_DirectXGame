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
	if (would_.translation_.x > 50 || would_.translation_.x < -50) {
      velocity_.x*=-1;
	}
	would_.UpdateMatrix(); 
	
}

void Enemy::Draw(ViewProjection& view) { 
  model_->Draw(would_, view, texturehandle_); 
  
}