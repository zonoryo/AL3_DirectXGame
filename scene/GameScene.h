#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Enemy.h"
#include "Player.h"
#include "EnemyBullet.h"
#include "PlayerBullet.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <sstream>
#include "DebugCamera.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	//散弾を追加する
	void AddEnemyBullet(EnemyBullet* enemyBullet);
	//敵発生データの読み込み
	void LoadEnemyPopDate();
	//敵発生コマンドの更新
	void UpdateEnemyPopCommands();

	void AddEnemy(Vector3 pos);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	
	uint32_t textureHandle_ = 0;
	
	ViewProjection viewProjection_;

	Model* model_ = nullptr;

	Player* player_ = nullptr;

	//Enemy* enemy_ = nullptr;

	PlayerBullet* playerBullet_ = nullptr;

	EnemyBullet* enemyBullet_ = nullptr;

	std::list<EnemyBullet*> enemybullets_;

	std::list<Enemy*> enemy_;

	WorldTransform worldTransform_;

	Skydome* skydome_ = nullptr;

	Model* modelSkydome_ = nullptr;

	Vector3 velocity_;

	bool isDebugCameraActve_ = false;

	DebugCamera* debugCamera_ = nullptr;

	RailCamera* railCamera_ = nullptr;

	std::stringstream enemyPopCommands;

	bool waitFlag_ = false;

	int32_t waitTimer_;
};