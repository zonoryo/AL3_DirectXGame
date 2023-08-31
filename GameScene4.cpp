#include "GameScene4.h"

#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <fstream>


GameScene4::GameScene4() {}

GameScene4::~GameScene4() {

	delete player_;
	delete model_;
	for (Enemy* enemy : enemy_) {
		delete enemy;
	}
	delete debugCamera_;

	delete playerBullet_;
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
}

void GameScene4::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// リソース
	textureHandle_ = TextureManager::Load("sample.png");
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	textureHandle_2 = TextureManager::Load("Level4.png");
	sprite_ = Sprite::Create(textureHandle_2, {0, 0});

	viewProjection_.Initialize();

	model_ = Model::Create();

	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// 自キャラの生成
	player_ = new Player();

	// 最初の雑魚
	// AddEnemy({0.0f, 5.0f, 30.0f});

	// LoadEnemyPopDate();
	// 　敵の生成
	// enemy_ = new Enemy();
	// 天球の生成
	skydome_ = new Skydome();

	// スカイドームの初期化
	skydome_->Initialize(modelSkydome_);
	// 自キャラの初期化
	Vector3 playerPosition(0, -5, 15);
	player_->Initialize(model_, textureHandle_, playerPosition);
	// レールカメラの生成/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	railCamera_ = new RailCamera2();
	// レールカメラ初期化//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	railCamera_->Initialize({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
	// 自キャラとレールカメラの親子関係を結ぶ
	// player_->SetParent(&railCamera_->GetWorldTransform());

	// playerBullet_->SetParent(&railCamera_->GetWorldTransform());
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///敵の初期化
	// ステージ4
	const int EnemyCount = 3;
	const float offset = 2.0f;
	// const float kEnemySpeed = 0.5f;
	const float kEnemySpeed2 = -0.6f;

	for (int i = 0; i < EnemyCount; ++i) {
		Enemy* newEnemy = new Enemy();

		// 各敵の初期位置を計算して設定
		Vector3 enemypos = Vector3(2.0f, -5.0f, 20.0f + i * offset);

		const float kEnemySpeed = 0.4f;
		Vector3 velocity(kEnemySpeed, 0, 0);
		// 二番目の敵の座標ずらし
		if (i == 1) {
			enemypos.x -= 10;
			velocity = Vector3(kEnemySpeed2, 0, 0);
		}

		newEnemy->Initialize(model_, enemypos, velocity);
		enemy_.push_back(newEnemy); // 敵をリストや配列に追加
	}

	// 敵とレールカメラの親子関係
	/*for (Enemy* enemy : enemy_) {
	    enemy->SetParent(&railCamera_->GetWorldTransform());
	}*/

	// 敵キャラにゲームシーンを渡す
	// enemy_->SetGameScene(this);

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//AxisIndicator::GetInstance()->SetVisible(true);
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// enemy_->SetPlayer(player_);
}

void GameScene4::Update() {

	// 自キャラの更新
	player_->Update(viewProjection_);

	// UpdateEnemyPopCommands();

	debugCamera_->Update();

	// 当たり判定の更新
	CheckAllCollisions();

	// スカイドームの更新
	skydome_->Update();
	// レールカメラの更新
	railCamera_->Update();
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

	// 敵の更新
	for (Enemy* enemy : enemy_) {
		if (enemy != NULL) {
			enemy->Update();
		}
	}
	// クリア判定
	bool allEnemiesDead = true; // すべての敵キャラが死亡しているかのフラグ

	for (Enemy* enemy : enemy_) {
		if (enemy != NULL) {
			enemy->Update();
			if (!enemy->IsDead()) {
				allEnemiesDead = false; // まだ生きている敵キャラがいる
			}
		}
	}

	// 敵がすべて消えたらフレームカウンタを増加させる
	if (allEnemiesDead && noNext_ == false) {
		frameCounter_++;

		// フレームカウンタが90に達したら toNext_ を true に設定
		if (frameCounter_ >= 90) {
			toNext_ = true;
		}
	} else {
		// 敵が生きている場合はフレームカウンタをリセット
		frameCounter_ = 0;
	}

	if (!allEnemiesDead) {
		const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
		for (PlayerBullet* bullet : playerBullets) {
			Vector3 bulletPos = bullet->GetWorldPosition();
			if (bulletPos.z > 150.0f) {
				noNext_ = true;
				break; // 一度でも条件が成立したらループを抜ける
			}
		}
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
	} else if (!isDebugCameraActve_) {
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
}

void GameScene4::Draw() {

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
	for (Enemy* enemy : enemy_) {
		if (enemy != NULL) {
			enemy->Draw(viewProjection_);
		}
	}

	// スカイドームの描画
	skydome_->Draw(viewProjection_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();
	player_->DarwUI();
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene4::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

#pragma region 自弾と敵キャラの当たり判定
	// 敵キャラの座標
	for (Enemy* enemy : enemy_) {
		// posA = enemy->GetWorldPosition();

		// 敵キャラと敵弾全ての当たり判定
		for (PlayerBullet* bullet : playerBullets) {
			posA = enemy->GetWorldPosition();
			// 自弾の座標
			posB = bullet->GetWorldPosition();

			// 座標 A・B の距離を求める
			Vector3 dis;
			dis.x = (posB.x - posA.x) * (posB.x - posA.x);
			dis.y = (posB.y - posA.y) * (posB.y - posA.y);
			dis.z = (posB.z - posA.z) * (posB.z - posA.z);
			float distance = dis.x + dis.y + dis.z;

			const float RadiusA = enemy->GetRadius();
			const float RadiusB = bullet->GetRadius();
			const float RadiusAB = (RadiusA + RadiusB) * (RadiusA + RadiusB);
			if (distance <= RadiusAB) {
				enemy->OnCollision();
			}
		}
#pragma endregion
	}
}
