#include "Player.h"


/// <summary>
/// 初期化
/// </summary>
/// <param name="model"></param>
/// <param name="textureHandle"></param>
void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 playerPosition) {
	// NULLポイントチェック
	assert(model);

	// 受け渡し
	model_ = model;
	modelReticle_ = model;
	textureHandle_ = textureHandle;
	tectureHandleR_ = TextureManager::Load("kamata.png");

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = playerPosition;

	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("target.png");

	// スプライト生成
	sprite2DReticle_ = Sprite::Create(
	    textureReticle,
	    {640, 360},
	    {1, 1, 1, 1}, {0.5f, 0.5f});

	// レティクルのテクスチャ
	TextureManager::Load("target.png");

	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

/// <summary>
/// 更新
/// </summary>
void Player::Update(ViewProjection& viewProjection) {

	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DRectile = 50.f;
	// 自機から3Dレティクルへのオフセット（Z+向き）
	Vector3 offset = {0, 0, 1.f};
	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	// ベクトルの長さを変える
	offset = Normalize(offset) * kDistancePlayerTo3DRectile;
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_.x = GetWorldPosition().x + offset.x;
	worldTransform3DReticle_.translation_.y = GetWorldPosition().y + offset.y;
	worldTransform3DReticle_.translation_.z = GetWorldPosition().z + offset.z;
	worldTransform3DReticle_.UpdateMatrix();

	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 3Dレティクルのワールド座標化2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle = worldTransform3DReticle_.translation_; 
	// ビューポート行列
	Matrix4x4 matViewPort =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
	    viewProjection.matView * viewProjection.matProjection * matViewPort;

	// ワールドスクリーン座標変換（3D->2D）
	positionReticle = Transform(positionReticle, matViewProjectionViewport);

	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharcterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= kCharcterSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		move.x += kCharcterSpeed;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) 
	{
		move.y += kCharcterSpeed;
	}
	else if (input_->PushKey(DIK_DOWN))
	{
		move.y -= kCharcterSpeed;
	}

	//範囲制限
	const float kMoveLimitX = 20.0f;
	const float kMoveLimitY = 20.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//座標移動（ベクトルの加算）
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	//worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// キャラクターの攻撃処理
	Attack();

	// キャラクターの旋回
	Rotate();

	// 行列を定数バッファーに転送
	worldTransform_.UpdateMatrix();

	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Player");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("position", sliderValue, -20.0f, 20.0f);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();

	//弾更新
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Update();
	}

}

void Player::Attack()
{ 
	if (input_->TriggerKey(DIK_SPACE))
	{
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		//弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::Rotate()
{ 
	const float kRotSpeed = 0.02f;

	if (input_->PushKey(DIK_A))
	{
		worldTransform_.rotation_.y -= kRotSpeed;
	}

	if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player() 
{
	// 弾解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}

	delete sprite2DReticle_;
}

Vector3 Player::GetWorldPosition() 
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Player::Oncollision() {}

void Player::SetParent(const WorldTransform* parent)
{
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw(ViewProjection& viewProjection)
 {
	// 3Dレティクルの描画
	modelReticle_->Draw(worldTransform3DReticle_, viewProjection);

	//自キャラの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Player::DrawUI() 
{ sprite2DReticle_->Draw(); }

