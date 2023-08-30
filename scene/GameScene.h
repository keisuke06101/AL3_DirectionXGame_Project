#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Explanation.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "Boss.h"
#include "BossBullet.h"
#include "Skydome.h"
#include "Score.h"
#include "RailCamera.h"
#include "DebugCamera.h"
#include <sstream>
#include <Novice.h>

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
	/// 操作説明表示処理
	/// </summary>
	void ExplanationUpdate();

	/// <summary>
	/// 操作説明描画
	/// </summary>
	void ExplanationDraw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	void AddEnemy(Vector3 pos);

	/// <summary>
	/// ボス弾を追加する
	/// </summary>
	/// <param name="bossBullet"></param>
	void AddBossBullet(BossBullet* bossBullet);

	/// <summary>
	/// 敵発生のデータ読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

private: // メンバ変数

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	uint32_t textureHandle_ = 0;
	uint32_t enemyTextureHandle_ = 0;
	uint32_t textureHandleE_ = 0;

	Model* model_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;
	
	// デバックカメラ有効
	bool isDebugCameraActive_ = false;

	// デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	// 操作説明
	Explanation* explanation_ = nullptr;

	//プレイヤー
	Player* player_ = nullptr;

	//敵
	std::list<Enemy*> enemy_;

	// ボス
	Boss* boss_ = nullptr;

	// プレイヤーの弾
	PlayerBullet* playerBullet_ = nullptr;

	//　敵の弾
	EnemyBullet* enemyBullet_ = nullptr;

	// ボスの弾
	BossBullet* bossBullet_ = nullptr;

	// 天球
	Skydome* skydome_ = nullptr;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;

	Vector3 trans_{};
	Vector3 rot_{};
	// 3Dモデル
	Model* modelSkydome_ = nullptr;

	std::list<EnemyBullet*> enemyBullets_;
	std::list<BossBullet*> bossBullets_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	bool isWait = 0;
	int waitTimer;

	/// <summary>
	/// コライダー２つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

};
