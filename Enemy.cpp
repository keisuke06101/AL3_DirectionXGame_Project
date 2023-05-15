﻿#include "Enemy.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="model"></param>
/// <param name="textureHandle"></param>
void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	// NULLポイントチェック
	assert(model);

	// 受け渡し
	model_ = model;
	textureHandle_ = textureHandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	//位置の初期化
	worldTransform_.translation_ = {0, 0, 50};
}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update()
{

	// 敵の移動速さ
	const float kCharcterSpeed = 0.2f;

	// 座標移動（ベクトルの加算）
	worldTransform_.translation_.z -= kCharcterSpeed;

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 行列を定数バッファーに転送
	worldTransform_.TransferMatrix();
}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection"></param>
void Enemy::Draw(ViewProjection& viewProjection)
{
	// 敵の描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
