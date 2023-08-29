#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "Player.h"
#include <DebugCamera.h>

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	//複製敵リスト
	std::list<Enemy*> enemies_;

	
	uint32_t textureHandle_ = 0;
	
	ViewProjection viewProjection_;
	Model* model_ = nullptr;
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	WorldTransform worldTransform_;
	PlayerBullet* playerBullet_ = nullptr;
	Vector3 velocity_;
	bool isDebugCameraActve_ = false;
	DebugCamera* debugCamera_ = nullptr;
};