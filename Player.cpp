#include "Player.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="model"></param>
/// <param name="textureHandle"></param>
void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポイントチェック
	assert(model);

	// 受け渡し
	model_ = model;
	textureHandle_ = textureHandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

/// <summary>
/// 更新
/// </summary>
void Player::Update() {
	// 行列を定数バッファーに転送
	worldTransform_.TransferMatrix();
}
/// <summary>
/// 描画
/// </summary>
void Player::Draw(ViewProjection& viewProjection)
 {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
 }
