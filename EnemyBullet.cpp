#include "EnemyBullet.h"
#include <cassert>
void EnemyBullet::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);
	model_ = model;
	texturehandle_ = TextureManager::Load("cube.jpg");
	would_.Initialize();
	would_.translation_ = pos;
	velocity_ = velocity;
}

void EnemyBullet::Update() {
	would_.UpdateMatrix();
	would_.translation_.x -= velocity_.x;
	would_.translation_.y -= velocity_.y;
	would_.translation_.z -= velocity_.z;
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(ViewProjection& view) { 
	model_->Draw(would_, view, texturehandle_); 
}