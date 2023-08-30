﻿#include "BossBullet.h"

void BossBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	velocity_ = velocity;

	// 衝突属性を設定
	SetCA(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定
	SetCM(kCollisionAttributePlayer);
}

void BossBullet::Update()
{
	// ワールドトランスフォームの更新
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	// 座標を移動させる（１フレーム分の移動量を足しこむ）
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	// 時間経過で消える
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void BossBullet::Draw(const ViewProjection& viewProjection) 
{
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void BossBullet::OnCollision()
{ isDead_ = true; }
