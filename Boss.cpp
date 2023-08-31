#include "Boss.h"
#include "Player.h"
#include "GameScene.h"

void Boss::Initialize(Model* model, const Vector3& pos) {
	// NULLポイントチェック
	assert(model);

	// 受け渡し
	model_ = model;
	modelW_ = model;
	textureHandle_ = TextureManager::Load("nu.png");
	textureHandleW_ = TextureManager::Load("0.png");

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransformW_.Initialize();

	// スケールの初期化
	//worldTransform_.scale_ = {6, 6, 1};

	// 位置の初期化
	worldTransform_.translation_ = pos;
	worldTransformW_.translation_ = {0, 0, worldTransform_.translation_.z};

	// 接近フェーズの初期化
	phaseApproachInitialize();

	// 衝突属性を設定
	SetCA(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定
	SetCM(kCollisionAttributePlayer);
}

void Boss::Update()
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

	/*worldTransformW_.matWorld_ = MakeAffineMatrix(
	    worldTransformW_.scale_, worldTransformW_.rotation_, worldTransformW_.translation_);*/

	// 行列を定数バッファーに転送
	worldTransform_.TransferMatrix();

	//// 行列を定数バッファーに転送
	//worldTransformW_.TransferMatrix();

	switch (phase_) {
	case Phase::Approach:
	default:
		phaseApproach();
		break;

	case Phase::Leave:
		phaseLeave();
		break;
	}

	// シェイク
	if (isRand_) {
		shakeTimer_ += 1;
	}
	if (shakeTimer_ == 2) {
		randX_ = rand() % i_ - 3;
		randY_ = rand() % i_ - 3;
		randZ_ = rand() % i_ - 3;
		worldTransform_.rotation_.x += randX_;
		worldTransform_.rotation_.y -= randY_;
		worldTransform_.rotation_.z -= randZ_;
		i_--;
		shakeTimer_ = 0;

		if (i_ == 0) {
			randX_ = 0;
			randY_ = 0;
			worldTransform_.rotation_.x = 0;
			worldTransform_.rotation_.y = 0;
			worldTransform_.rotation_.z = 0;
			i_ = 21;
			isRand_ = false;
		}
	}
}

void Boss::Draw(ViewProjection& viewProjection) 
{
	//// ボスの弱点
	//modelW_->Draw(worldTransformW_, viewProjection, textureHandleW_);
	// 敵の描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

void Boss::phaseApproachInitialize()
{
	// 発射タイマーの初期化
	fireTimer = kFireInterval;

	// 発射タイマーをセットする
	timedCalls_.push_back(new TimedCall(std::bind(&Boss::FireReset, this), fireTimer));
}

void Boss::phaseApproach()
{
	// 接近フェーズスピード
	const float kApproachSpeed = 0.2f;
	// 移動ベクトル
	worldTransform_.translation_.z -= kApproachSpeed;
	//aaworldTransformW_.translation_.z -= kApproachSpeed;
	// 既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 30.0f) {
		//phase_ = Phase::Leave;
		worldTransform_.translation_.z = 30.0f;
		worldTransformW_.translation_.z = worldTransform_.translation_.z - 3;

	}

	// 範囲制限
	const float kMoveLimitX = 10.0f;
	const float kMoveLimitY = 10.0f;

	// 範囲を超えない処理
	worldTransformW_.translation_.x = max(worldTransformW_.translation_.x, -kMoveLimitX);
	worldTransformW_.translation_.x = min(worldTransformW_.translation_.x, +kMoveLimitX);
	worldTransformW_.translation_.y = max(worldTransformW_.translation_.y, -kMoveLimitY);
	worldTransformW_.translation_.y = min(worldTransformW_.translation_.y, +kMoveLimitY);

	if (worldTransformW_.translation_.x >= kMoveLimitX || worldTransformW_.translation_.x <= -kMoveLimitX)
	{
		move.x *= -1;
	}
	if (worldTransformW_.translation_.y >= kMoveLimitY || worldTransformW_.translation_.y <= -kMoveLimitY)
	{
		move.y *= -1;
	}
	// 座標移動（ベクトルの加算）
	worldTransformW_.translation_.x += move.x;
	worldTransformW_.translation_.y += move.y;

}

void Boss::phaseLeave()
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
	gameScene_->AddBossBullet(newBullet);
}

Boss::~Boss()
{
	// 範囲forでリストの全要素について回す
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
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

void Boss::OnCollision() 
{
	isDead_ = true;
	//isDead_ = true;
}

void Boss::FireReset() 
{
	// 弾を発射する
	Fire();

	// 発射タイマーをセットする
	timedCalls_.push_back(new TimedCall(std::bind(&Boss::FireReset, this), kFireInterval));
}
