#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "Enemy.h"
#include "Vector3.h"


GameScene::GameScene() {}

GameScene::~GameScene() {
	
	delete player_;
	delete model_;
	delete enemy_;
	delete debugCamera_;
	delete enemyBullet_;
	delete playerBullet_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// リソース
	textureHandle_ = TextureManager::Load("sample.png");

	

	viewProjection_.Initialize();

	model_ = Model::Create();

	// 自キャラの生成
	player_ = new Player();
	//　敵の生成
	enemy_ = new Enemy();

	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);
	// 敵の初期化
	const float kEnemySpeed = 0.2f;
	Vector3 velocity(0, 0, kEnemySpeed);
	                          //敵の座標
	enemy_->Initialize(model_, {20,10,50}, velocity);

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	enemy_->SetPlayer(player_);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	debugCamera_->Update();

	//当たり判定の更新
	CheckAllCollisions();

	// 敵の更新
	if (enemy_ != NULL) {
		enemy_->Update();
	}
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_BACKSPACE)) {
		isDebugCameraActve_ = true;
	}

#endif

	if (isDebugCameraActve_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();
	} else {

		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	player_->Draw(viewProjection_);
	// 敵の描画
	if (enemy_ != NULL) {
		enemy_->Draw(viewProjection_);
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	//判定対象AとBの座標
	Vector3 posA, posB;
	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();
	#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();
	for (EnemyBullet* bullet : enemyBullets) {
		posB = bullet->GetWorldPosition();
		// 座標 A・B の距離を求める
		Vector3 dis;
		dis.x = posA.x - posB.x;
		dis.y = posA.y - posB.y;
		dis.z = posA.z - posB.z;

		float distance = sqrt((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z));

		if (distance <= 2) {
			// 自キャラの衝突時コールバックを呼び出す
			//player_->OnCollision();

			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region 自弾と敵キャラの当たり判定
	// 敵キャラの座標
	posA = enemy_->GetWorldPosition();

	// 敵キャラと敵弾全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 自弾の座標
		posB = bullet->GetWorldPosition();

		// 座標 A・B の距離を求める
		Vector3 dis;
		dis.x = posA.x - posB.x;
		dis.y = posA.y - posB.y;
		dis.z = posA.z - posB.z;

		float distance = sqrt((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z));

		if (distance <= 2) {
			// 敵キャラの衝突時コールバックを呼び出す
			//enemy_->OnCollision();

			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
    #pragma endregion

	#pragma region 自弾と敵弾の当たり判定
	// 自弾と敵弾全ての当たり判定
	for (PlayerBullet* bulletPlayer : playerBullets) {
		posA = bulletPlayer->GetWorldPosition();

		for (EnemyBullet* bulletEnemy : enemyBullets) {
			// 自弾の座標
			posB = bulletEnemy->GetWorldPosition();

			// 座標 A・B の距離を求める
			Vector3 dis;
			dis.x = posA.x - posB.x;
			dis.y = posA.y - posB.y;
			dis.z = posA.z - posB.z;

			float distance = sqrt((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z));

			if (distance <= 2) {
				// 敵キャラの衝突時コールバックを呼び出す
				bulletPlayer->OnCollision();

				// 自弾の衝突時コールバックを呼び出す
				bulletEnemy->OnCollision();
			}
		}
	}
    #pragma endregion
}


