#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "PlayerBullet.h"
#include <MathUtility.h>
#include <cassert>
#include <list>
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
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

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

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//キーボード入力
	Input* input_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;

};
