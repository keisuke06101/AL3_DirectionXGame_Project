#include "Model.h"
#include "WorldTransform.h"
#include "ImGuiManager.h"
#include <cassert>
#include <list>

#pragma once
class Enemy 
{
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
	/// 接近フェーズの更新処理
	/// </summary>
	void phaseApproach();

	/// <summary>
	/// 離脱フェーズの更新処理
	/// </summary>
	void phaseLeave();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 行動フェーズ
	enum class Phase { 
		Approach,  //接近する
		Leave      //離脱する
	};

	Phase phase_ = phase_;

};
