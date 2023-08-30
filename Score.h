#include "Model.h"
#include "Collider.h"
#include "WorldTransform.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "MathUtility.h"
#include "Matrix4x4.h"
#include <cassert>
#include <cmath>
#include <list>
#include <Sprite.h>
#pragma once

class Score
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	///< param name = "model">モデル</param>
	/// ///<param name = "textureHandle">テクスチャハンドル</param>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// 2Dスプライト
	Sprite* sprite2DScore_ = nullptr;

	// モデル
	Model* model_ = nullptr;
	Model* modelReticle_ = nullptr;

	// テクスチャハンドル
	uint32_t tectureHandleR_ = 0u;

	int eachScore_[5] = {};
	int score_ = 0;
	// 保存用変数
	int saveScore_ = 0;
};
