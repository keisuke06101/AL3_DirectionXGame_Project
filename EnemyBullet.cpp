#include "EnemyBullet.h"
#include "Player.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	velocity_ = velocity;

	// Z方向に延びた形状
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;
}

void EnemyBullet::Update() 
{
	// ワールドトランスフォームの更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	// 座標を移動させる（１フレーム分の移動量を足しこむ）
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	// 敵弾から自キャラへのベクトルを計算
	Vector3 toPlayer = player_->GetWorldPosition() - worldTransform_.translation_;

	// ベクトルを正規化する
	Normalize(toPlayer);
	Normalize(velocity_);

	// 補間割合
	float t = 1.0f;
	// 弾の速度
	float kBulletSpeed = 1.0f;

	// 線形補間により、今の速度と自キャラへのベクトルを撃ち挿し、新たな速度とする
	velocity_ = Lerp(velocity_, toPlayer, t) * kBulletSpeed;
	// 進行方向に見た目の方向を合わせる
	// Y軸周りの角度(0y)
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	// X軸周りの角度(0x)
	// velocity_.y = 0.0f;
	worldTransform_.rotation_.x = std::atan2(velocity_.y, velocity_.z);


	// 時間経過で消える
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetWorldPosition()
{ 
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

void EnemyBullet::Oncollision() {
	isDead_ = true; 
}
