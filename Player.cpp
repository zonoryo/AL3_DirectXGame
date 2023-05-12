#include "Player.h"
#include <cassert>
void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	// 引数として受け取ってデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
}
void Player::Update() { worldTransform_.TransferMatrix(); }
void Player::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
