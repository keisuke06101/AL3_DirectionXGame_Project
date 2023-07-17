#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="model"></param>
/// <param name="textureHandle"></param>
void Enemy::Initialize(Model* model, const Vector3& pos) {
	// NULLポイントチェック
	assert(model);

	// 受け渡し
	model_ = model;
	textureHandle_ = TextureManager::Load("nu.png");

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 位置の初期化
	worldTransform_.translation_ = pos;

	// 接近フェーズの初期化
	phaseApproachInitialize();

}


Enemy::~Enemy() 
{
	// 範囲forでリストの全要素について回す
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}
}

Vector3 Enemy::GetWorldPosition() 
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

void Enemy::Oncollision() {}

void Enemy::FireReset()
{
	// 弾を発射する
	Fire();

	// 発射タイマーをセットする
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::FireReset, this), kFireInterval));
}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update()
{
	// 終了したタイマーの削除
	timedCalls_.remove_if([](TimedCall* time) {
		if (time->IsFinished()) {
			delete time;
			return true;
		}
		return false;
	});

	// 範囲forでリストの全要素について回す
	for (TimedCall* timedCall : timedCalls_) {
		timedCall->Update();
	}

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

void Enemy::phaseApproachInitialize() 
{
	// 発射タイマーの初期化
	fireTimer = kFireInterval;

	// 発射タイマーをセットする
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::FireReset, this), fireTimer));

}

void Enemy::phaseApproach()
{
	// 接近フェーズスピード
	const float kApproachSpeed = 0.2f;
	// 移動ベクトル
	worldTransform_.translation_.z -= kApproachSpeed;
	// 既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 30.0f) {
		phase_ = Phase::Leave;
	}

	//// 発射タイマーカウントダウン
	//fireTimer--;
	//// 指定時間に達した
	//if (fireTimer <= 0) {
	//	// 弾を発射
	//	Fire();
	//	// 発射タイマーを初期化
	//	fireTimer = kFireInterval;
	//}
}

void Enemy::phaseLeave()
{
	// 離脱フェーズスピード
	const float kLeaveSpeed = 0.2f;
	// 移動ベクトル
	worldTransform_.translation_.x += kLeaveSpeed;
	// 時間経過で消える
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	timedCalls_.clear();
}

void Enemy::Fire()
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
	    player_->GetWorldPosition().z - GetWorldPosition().z
	};
	float length = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	Vector3 dir = {vec.x / length, vec.y / length, vec.z / length};
	Vector3 velocity = {dir.x * kBulletSpeed, dir.y * kBulletSpeed, dir.z * kBulletSpeed};
	
	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	gameScene_->AddEnemyBullet(newBullet);
}

