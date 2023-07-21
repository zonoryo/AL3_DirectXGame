#include "Skydome.h"
#include <cassert>
void Skydome::Initialize(Model* model) {
	assert(model);

	model_ = model;
	
	worldTransform_.Initialize();
}
void Skydome::Update() { worldTransform_.UpdateMatrix(); }
void Skydome::Draw(ViewProjection& viewProjection_) { 
	model_->Draw(worldTransform_, viewProjection_);
}