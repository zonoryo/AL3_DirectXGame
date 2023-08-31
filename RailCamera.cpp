#include "RailCamera.h"
#include <cassert>
#include "Matrix4x4.h"
#include "ImGuiManager.h"
void RailCamera::Initialize(const Vector3& pos, const Vector3& rotate) {
	//ワールド変換の初期化
	worldTransform_.Initialize();
	//引数で受け取った
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = rotate;
	//ビュープロジェクションの初期化
	viewProjection_.farZ = 100.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update() {
//加算する値
	Vector3 move ={0.0f, 0.0f, 0.0f};  
	Vector3 rot ={0.0f, 0.0f, 0.0f}; 

	//座標の加算
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
	//角度の加算
	worldTransform_.rotation_.x += rot.x;
	worldTransform_.rotation_.y += rot.y;
	worldTransform_.rotation_.z += rot.z;
	//ワールドトランスフォームのワールド行列再計算
	worldTransform_.UpdateMatrix();
	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	viewProjection_.TransferMatrix();

	ImGui::Begin("Camera");
	//スライダーでカメラのtranslationを表示
	ImGui::SliderFloat3("translation", &worldTransform_.translation_.x, 0.01f, 100.0f);
	// スライダーでカメラのrotationを表示
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, 0.01f, 100.0f);
	ImGui::End();
}

