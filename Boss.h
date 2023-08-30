#include "Model.h"
#include "Collider.h"
#include "WorldTransform.h"
#include "ImGuiManager.h"
#include "BossBullet.h"
#include "MathUtility.h"
#include "TimedCall.h"
#include <cassert>
#include <cmath>
#include <list>
#pragma once

// 自機クラスの前方宣言
class Player;

// ゲームシーンクラスの前方宣言
class GameScene;

class Boss : public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	///< param name = "model">モデル</param>
	/// ///<param name = "textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 接近フェーズの初期化
	/// </summary>
	void phaseApproachInitialize();

	/// <summary>
	/// 接近フェーズの更新処理
	/// </summary>
	void phaseApproach();

	/// <summary>
	/// 離脱フェーズの更新処理
	/// </summary>
	void phaseLeave();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Boss();

	// 発射間隔
	static const int kFireInterval = 60;

	// ワールド座標を取得
	Vector3 GetWorldPosition() override;

	// 衝撃を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	// 弾リストを取得
	// const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;

	/// <summary>
	/// 弾を発射し、タイマーをリセットするコールバック関数
	/// </summary>
	void FireReset();

	void SetPlayer(Player* player) { player_ = player; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave     // 離脱する
	};

	Phase phase_ = Phase::Approach;

	// 弾
	std::list<BossBullet*> bullets_;

	// 発射タイマー
	int32_t fireTimer = 0;

	// 自キャラ
	Player* player_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

	// 時限発動のリスト
	std::list<TimedCall*> timedCalls_;
};
