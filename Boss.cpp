#include "Boss.h"
#include "Player.h"

void Boss::Initialize(Model* model) 
{
	// NULLポイントチェック
	assert(model);

	// 受け渡し
	model_ = model;
	textureHandle_ = TextureManager::Load("nu.png");

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 位置の初期化
	worldTransform_.translation_ = {0, 0, 100};

	// 接近フェーズの初期化
	phaseApproachInitialize();
}

void Boss::Update()
{
	// デスフラグの立った弾を削除
	bullets_.remove_if([](BossBullet* bullet) {
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
	for (BossBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Boss::Draw(ViewProjection& viewProjection) 
{
	// 敵の描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾描画
	for (BossBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Boss::phaseApproachInitialize()
{
	// 発射タイマーの初期化
	fireTimer = kFireInterval;
}

void Boss::phaseApproach()
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

void Boss::phaseLeave()
{
	// 離脱フェーズスピード
	const float kLeaveSpeed = 0.2f;
	// 移動ベクトル
	worldTransform_.translation_.x += kLeaveSpeed;
	// worldTransform_.translation_.y += kLeaveSpeed;
	// worldTransform_.translation_.z -= kLeaveSpeed;
}

void Boss::Fire() 
{
	assert(player_);

	// 弾の速度
	const float kBulletSpeed = 1.0f;

	// 自キャラのワールド座標を取得する
	player_->GetWorldPosition();
	// 敵キャラのワールド座標を取得する
	GetWorldPosition();
	// 敵キャラ->自キャラの差分ベクトル
	Vector3 vec{
	    player_->GetWorldPosition().x - GetWorldPosition().x,
	    player_->GetWorldPosition().y - GetWorldPosition().y,
	    player_->GetWorldPosition().z - GetWorldPosition().z};
	float length = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	Vector3 dir = {vec.x / length, vec.y / length, vec.z / length};
	Vector3 velocity = {dir.x * kBulletSpeed, dir.y * kBulletSpeed, dir.z * kBulletSpeed};

	// 弾を生成し、初期化
	BossBullet* newBullet = new BossBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	bullets_.push_back(newBullet);
}

Boss::~Boss()
{
	for (BossBullet* bullet : bullets_) {
		delete bullet;
	}
}

Vector3 Boss::GetWorldPosition() 
{ 
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}
