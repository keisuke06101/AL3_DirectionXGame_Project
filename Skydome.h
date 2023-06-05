#include "Model.h"
#include "WorldTransform.h"
#pragma once

/// <summary>
/// 天球
/// </summary>
class Skydome {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& view);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// モデル
	Model* model_ = nullptr;
};
