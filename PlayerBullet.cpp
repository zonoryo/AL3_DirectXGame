#include "PlayerBullet.h"
#include <cassert>
void PlayerBullet::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) { 
	assert(model);
	model_ = model;
	texturehandle_ = TextureManager::Load("cube.jpg");
	would_.Initialize();
	would_.translation_ = pos;
	velocity_ = velocity;
}
void PlayerBullet::Update() { 
	would_.UpdateMatrix();
	would_.translation_.x += velocity_.x;
	would_.translation_.y += velocity_.y;
	would_.translation_.z += velocity_.z;
	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}
void PlayerBullet::Draw(ViewProjection& view) {
	model_->Draw(would_, view, texturehandle_);
}