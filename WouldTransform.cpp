﻿#include "WorldTransform.h"

void WorldTransform::UpdateMatrix() { 
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	if (parent_) {
		matWorld_ *= parent_->matWorld_;
	}
	TransferMatrix(); 
}