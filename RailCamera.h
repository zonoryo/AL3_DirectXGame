#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
class RailCamera {
public:
	void Initialize(const Vector3& pos, const Vector3& rotate);
	void Update();
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	// ビュープロジェクション
	ViewProjection viewProjection_;
	//ワールド変換データ
	WorldTransform worldTransform_;
};
