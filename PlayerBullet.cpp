﻿#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	velocity_ = velocity;
}

void PlayerBullet::Update() 
{
	//ワールドトランスフォームの更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_,worldTransform_.rotation_,worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	//座標を移動させる（１フレーム分の移動量を足しこむ）
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	//時間経過で消える
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 PlayerBullet::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

/// <summary>
/// 衝突判定
/// </summary>
void PlayerBullet::Oncollision()
{ 
	isDead_ = true;
}
