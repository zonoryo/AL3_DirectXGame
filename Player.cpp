#include "Player.h"
#include <cassert>
#include "Matrix4x4.h"

#include "ImGuiManager.h"

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 pos) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform3DReticle_.Initialize();
	worldTransform_.translation_ = pos;
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("target.png");
	//スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});
	//レティクルのテクスチャ
	TextureManager::Load("target.png");

}

void Player::Update(ViewProjection& viewProjection) {
	worldTransform_.TransferMatrix();
	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	if (input_->TriggerKey(DIK_SPACE) && !isBulletShot_) {
		Attack();
		isBulletShot_ = true; // 弾が発射されたのでフラグを設定
	}

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	// 移動ベクトルの変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 移動ベクトルの変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	//旋回
	const float kRotSpeed = 0.02f;

	if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	//キャラクターの攻撃処理
	Attack();
	//弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// 範囲制限
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 座標移動 (ベクトルの加算)
	worldTransform_.translation_.x += move.x; // ※数学の授業で作った関数を当てはめる
	worldTransform_.translation_.y += move.y;
	worldTransform_.UpdateMatrix();

	//ここ消さないとworldTransform.cppで書いたことが上書きされて更新されてしまう
	/*worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();*/

	//自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = {0.0f, 0.0f, 1.0f};
	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	// ベクトルの長さを整える
	offset = Normalize(offset) * kDistancePlayerTo3DReticle;
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = Add(worldTransform_.translation_ , offset);
	// ワールド行列の更新
	worldTransform3DReticle_.UpdateMatrix();
	// ワールド行列の転送
	worldTransform3DReticle_.TransferMatrix();
	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle;
	positionReticle.x = worldTransform3DReticle_.matWorld_.m[3][0];
	positionReticle.y = worldTransform3DReticle_.matWorld_.m[3][1];
	positionReticle.z = worldTransform3DReticle_.matWorld_.m[3][2];
	//ビューポート行列
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	//ビュー行列とプロジェクション行列、ビューポートを合成する
	Matrix4x4 matViewProjectionViewport =
	    viewProjection.matView * viewProjection.matProjection * matViewport;
	//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = Transform(positionReticle, matViewProjectionViewport);
	//スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	// キャラクターの座標を表示
	ImGui::SetNextWindowPos({60, 60});
	ImGui::SetNextWindowContentSize({300, 100});

	ImGui::Begin("Player");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("position", sliderValue, -20.0f, 20.0f);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();
}

void Player::Draw(ViewProjection&viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
    //弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	model_->Draw(worldTransform3DReticle_, viewProjection);
}






//void Player::Attack() {
//	const float kBulletSpeed = 1.0f;
//	Vector3 velocity(0, 0, kBulletSpeed);
//	// 自キャラのワールド行列
//	velocity = TransformNormal(velocity, worldTransform_.matWorld_);
//	if (input_->TriggerKey(DIK_SPACE)) {
//		PlayerBullet* newBullet = new PlayerBullet();
//		newBullet->Initialize(model_, GetWorldPosition(), velocity);
//
//		bullet_ = newBullet;
//		bullets_.push_back(newBullet);
//	}
//
//	//velocity.x = worldTransform3DReticle_.matWorld_.m[3][0] - worldTransform_.matWorld_.m[3][0];
//	//velocity.y = worldTransform3DReticle_.matWorld_.m[3][1] - worldTransform_.matWorld_.m[3][1];
//	//velocity.z = worldTransform3DReticle_.matWorld_.m[3][2] - worldTransform_.matWorld_.m[3][2];
//	//velocity = Normalize(velocity) * kBulletSpeed;
//}
	
void Player::Attack() {
	const float kBulletSpeed = 7.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	if (input_->TriggerKey(DIK_SPACE) && !isBulletShot_) {
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		bullets_.push_back(newBullet);

		isBulletShot_ = true;
	}

	velocity.x = worldTransform3DReticle_.matWorld_.m[3][0] - worldTransform_.matWorld_.m[3][0];
	velocity.y = worldTransform3DReticle_.matWorld_.m[3][1] - worldTransform_.matWorld_.m[3][1];
	velocity.z = worldTransform3DReticle_.matWorld_.m[3][2] - worldTransform_.matWorld_.m[3][2];
	velocity = Normalize(velocity) * kBulletSpeed;
}	

void Player::DarwUI() { sprite2DReticle_->Draw(); }

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

Vector3 Player::TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{

	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],

	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],

	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]};

	return result;
}

Vector3 Player::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0]; 
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}


void Player::SetParent(const WorldTransform* parent) { 
	worldTransform_.parent_ = parent;
	//bullet_->SetParent(parent);
}