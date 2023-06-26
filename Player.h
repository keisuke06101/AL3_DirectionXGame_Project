﻿#include "Model.h"
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
class Player {
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
	Vector3 GetWorldPosition();

	// 衝撃を検出したら呼び出されるコールバック関数
	void Oncollision();

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransform* parent);

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

	//キーボード入力
	Input* input_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;

};
