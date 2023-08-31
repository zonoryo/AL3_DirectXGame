#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// タイトル
/// </summary>
class Info {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Info();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Info();

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

	bool GetTonext() { return toNext_; }

private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio_ = nullptr;
	bool toNext_ = false;
	uint32_t textureHandle_ = 0;
	Sprite* sprite_ = nullptr;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
	/// 直切り替え防止カウント
	int count;
};
