#include "Model.h"
#include "Collider.h"
#include "WorldTransform.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "PlayerBullet.h"
#include "MathUtility.h"
#include "Matrix4x4.h"
#include <cassert>
#include <list>
#include <Sprite.h>
#pragma once

/// <summary>
/// 自キャラ
/// </summary>
class Player : public Collider{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	///< param name = "model">モデル</param>
	/// ///<param name = "textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle, Vector3 playerPosition);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 旋回
	/// </summary>
	void Rotate();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	// ワールド座標を取得
	Vector3 GetWorldPosition() override;

	// 衝撃を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransform* parent);

	// 衝突時に呼ばれる関数
	//void OnCollision() override;

	// ゲッター
	bool GetIsDead() const { return isDead_; };

private:

	// ワールド変換データ
	WorldTransform worldTransform_;
	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

	// モデル
	Model* model_ = nullptr;
	Model* modelReticle_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	uint32_t tectureHandleR_ = 0u;

	uint32_t iconTexture_ = 0u;
	Sprite* spriteIcon_ = nullptr;

	uint32_t lifeTexture_ = 0u;
	Sprite* spriteLife_ = nullptr;

	//キーボード入力
	Input* input_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;

	// シェイク演出用変数
	int shakeTimer_ = 0;
	bool isRand_ = 0;
	bool isRandP_ = 0;

	int randX_ = 0;
	int randY_ = 0;
	int randZ_ = 0;
	int i_ = 21;

	bool isDead_ = 0;

	int playerLife_ = 5;

};
