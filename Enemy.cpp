#include "Enemy.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="model"></param>
/// <param name="textureHandle"></param>
void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポイントチェック
	assert(model);

	// 受け渡し
	model_ = model;
	textureHandle_ = textureHandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 位置の初期化
	worldTransform_.translation_ = {0, 0, 50};

	// 接近フェーズの初期化
	phaseApproachInitialize();

}


Enemy::~Enemy() 
{
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update()
{
	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

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

	// 弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
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

	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::phaseApproachInitialize() 
{
	// 発射タイマーの初期化
	fireTimer = kFireInterval;
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

	// 発射タイマーカウントダウン
	fireTimer--;
	// 指定時間に達した
	if (fireTimer <= 0) {
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		fireTimer = kFireInterval;
	}
}

void Enemy::phaseLeave()
{
	// 離脱フェーズスピード
	const float kLeaveSpeed = 0.2f;
	// 移動ベクトル
	worldTransform_.translation_.x += kLeaveSpeed;
	//worldTransform_.translation_.y += kLeaveSpeed;
	//worldTransform_.translation_.z -= kLeaveSpeed;
}

void Enemy::Fire()
{
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);
	
	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	bullets_.push_back(newBullet);
}

