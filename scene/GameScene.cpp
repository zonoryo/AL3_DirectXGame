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
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete enemy_;
	delete debugCamera_;
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
	//enemy_ = new Enemy();

	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);
	// 敵の初期化
	//ステージ1
	const int EnemyCount = 3;
	const float offset = 2.0f; 
	//const float kEnemySpeed = 0.5f;
	const float kEnemySpeed2 = -0.5f;
	
	for (int i = 0; i < EnemyCount; ++i) {
		Enemy* newEnemy = new Enemy();

		// 各敵の初期位置を計算して設定
		Vector3 enemypos = Vector3(0.0f, -0.0f, 15.0f+i * offset);

		const float kEnemySpeed = 0.5f;
		Vector3 velocity(kEnemySpeed, 0, 0);
		//二番目の敵の座標ずらし
		 if (i == 1) {
			enemypos.x += 5;
			 velocity = Vector3(kEnemySpeed2, 0, 0);
		}
		
		newEnemy->Initialize(model_, enemypos, velocity);
		enemies_.push_back(newEnemy); // 敵をリストや配列に追加
	}
	
	
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update(viewProjection_);

	debugCamera_->Update();

	// 敵の更新
	for (Enemy* enemy : enemies_) {
		if (enemy != NULL) {
			 enemy->Update();
			 if (enemy->IsDead()) {
				 delete enemy;    // isDead_ フラグが true の敵を削除
				 enemy = nullptr; // enemies_ リスト内のポインタを null にする
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
	
   for (Enemy* enemy : enemies_) {
		//if (enemy != nullptr && !enemy->IsDead()) { // IsDead() メソッドを呼び出して判定
			enemy->Draw(viewProjection_);
		//}
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
	player_->DarwUI();
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	for (Enemy* enemy : enemies_) {
	

	// 敵キャラ全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
			// 敵キャラの座標
		posA = enemy->GetWorldPosition();
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
		enemy->OnCollision();
		}
	  }
  }
}