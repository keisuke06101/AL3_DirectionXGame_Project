#include "Enemy.h"

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
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 行列を定数バッファーに転送
	worldTransform_.TransferMatrix();

	switch (phase_) { 
	case Phase::Approach:
	default:
		phaseApproach();
		break;

	case Phase::Leave:
		phaseLeave();
		break;
	}
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

void Enemy::phaseApproach()
{
	// 接近フェーズスピード
	const float kApproachSpeed = 0.2f;
	// 移動ベクトル
	worldTransform_.translation_.z -= kApproachSpeed;
	// 既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::phaseLeave()
{
	// 離脱フェーズスピード
	const float kLeaveSpeed = 0.2f;
	// 移動ベクトル
	worldTransform_.translation_.x -= kLeaveSpeed;
	worldTransform_.translation_.y += kLeaveSpeed;
	worldTransform_.translation_.z -= kLeaveSpeed;
}
