#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <fstream>


GameScene::GameScene() {}

GameScene::~GameScene() {
	
	delete player_;
	delete model_;
	for (Enemy* enemy : enemy_) {
	    delete enemy;
	}
	delete debugCamera_;
	delete enemyBullet_;
	delete playerBullet_;
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
	for (EnemyBullet* enemybullet : enemybullets_) {
		delete enemybullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// リソース
	textureHandle_ = TextureManager::Load("sample.png");


	
	viewProjection_.Initialize();
	
	
	model_ = Model::Create();

	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// 自キャラの生成
	player_ = new Player();

	AddEnemy({0.0f, 5.0f, 30.0f});

	LoadEnemyPopDate();
	//　敵の生成
	//enemy_ = new Enemy();
	//天球の生成
	skydome_ = new Skydome();


	
	//スカイドームの初期化
	skydome_->Initialize(modelSkydome_);
	//自キャラの初期化
	Vector3 playerPosition(0, 0, 15);
	player_->Initialize(model_, textureHandle_,playerPosition);
	// レールカメラの生成
	railCamera_ = new RailCamera();
	//レールカメラ初期化
	railCamera_->Initialize({0.0f, 0.0f, 1.0f}, {0.0f,0.0f,0.0f});
	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());
	// 敵の初期化
	const float kEnemySpeed = 0.2f;
	Vector3 velocity(0, 0, kEnemySpeed);
	                          //敵の座標
	//enemy_->Initialize(model_, {20,10,50}, velocity);
	
	//敵キャラにゲームシーンを渡す
	//enemy_->SetGameScene(this);

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//enemy_->SetPlayer(player_);

}

void GameScene::Update() {
	
	// 自キャラの更新
	player_->Update();

	UpdateEnemyPopCommands();

	debugCamera_->Update();

	//当たり判定の更新
	CheckAllCollisions();


	// スカイドームの更新
	skydome_->Update();
	//レールカメラの更新
	railCamera_->Update();
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

	enemybullets_.remove_if([](EnemyBullet* enemybullet) {
		if (enemybullet->IsDead()) {
			delete enemybullet;
			return true;
		}
		return false;
	});
	// 敵の更新
	for (Enemy* enemy : enemy_) {
		if (enemy != NULL) {
			enemy->Update();
		}
	}

	for (EnemyBullet* enemybullet : enemybullets_) {
		enemybullet->Update();
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
	}
	else if(!isDebugCameraActve_){
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	
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
	for (Enemy* enemy : enemy_) {
		if (enemy != NULL) {
			enemy->Draw(viewProjection_);
		}
	}
	

	for (EnemyBullet* enemybullet : enemybullets_) {
		enemybullet->Draw(viewProjection_);
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
	//const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();
	#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();
	for (EnemyBullet* bullet : enemybullets_) {
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
	for (Enemy* enemy : enemy_) {
		posA = enemy->GetWorldPosition();
	}
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

		for (EnemyBullet* bulletEnemy : enemybullets_) {
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { enemybullets_.push_back(enemyBullet); }
void GameScene::AddEnemy(Vector3 pos) {
	// 敵の生成
	Enemy* obj = new Enemy();
	// 敵の初期化
	obj->Initialize(model_, pos);
	// 敵キャラに自キャラのアドレスを渡す
	obj->SetPlayer(player_);
	// 敵キャラのゲームシーンを渡す
	obj->SetGameScene(this);

	enemy_.push_back(obj);
}
void GameScene::LoadEnemyPopDate() { 
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPos.csv");
	assert(file.is_open());
	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();
	//ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (waitFlag_ == true) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			// 待機完了
			waitFlag_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			AddEnemy(Vector3(x, y, z));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			waitFlag_ = true;
			waitTimer_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}


