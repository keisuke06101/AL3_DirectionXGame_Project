#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	uint32_t textureHandle_ = 0;
	uint32_t enemyTextureHandle_ = 0;

	Model* model_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_;
	
	//デバックカメラ有効
	bool isDebugCameraActive_ = false;

	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	//プレイヤー
	Player* player_ = nullptr;

	//敵
	Enemy* enemy_ = nullptr;

	// プレイヤーの弾
	PlayerBullet* playerBullet_ = nullptr;

	//　敵の弾
	EnemyBullet* enemyBullet_ = nullptr;

	// 天球
	Skydome* skydome_ = nullptr;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;

	Vector3 trans_{};
	Vector3 rot_{};
	// 3Dモデル
	Model* modelSkydome_ = nullptr;


};
