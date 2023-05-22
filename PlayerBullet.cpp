#include "PlayerBullet.h"
#include <cassert>
void PlayerBullet::Initialize(Model* model, const Vector3& pos) { 
	assert(model);
	model_ = model;
	texturehandle_ = TextureManager::Load("cube.jpg");
	would_.Initialize();
	would_.translation_ = pos;
}
void PlayerBullet::Update() { 
	would_.UpdateMatrix();
}
void PlayerBullet::Draw(ViewProjection& view) {
	model_->Draw(would_, view, texturehandle_);
}