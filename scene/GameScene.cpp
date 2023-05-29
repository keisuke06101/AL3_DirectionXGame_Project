#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene()
{
	delete debugCamera_;
	delete model_;
	delete player_;
	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("sample.png");
	enemyTextureHandle_ = TextureManager::Load("nu.png");
	model_ = Model::Create();
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	player_ = new Player;
	enemy_ = new Enemy;
	playerBullet_ = new PlayerBullet;
	enemyBullet_ = new EnemyBullet;
	viewProjection_.Initialize();
	player_->Initialize(model_, textureHandle_);
	enemy_->Initialize(model_, enemyTextureHandle_);

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

}

void GameScene::Update() 
{

    #ifdef _DEBUG
    	if (input_->TriggerKey(DIK_BACKSPACE)) {
    		isDebugCameraActive_ = true;
    	}
    #endif // DEBUG 
	player_->Update();
	enemy_->Update();
	CheckAllCollisions();

	if (isDebugCameraActive_)
	{
		// デバックカメラの更新
		debugCamera_->Update();

		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else
	{
		//ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::CheckAllCollisions()
{
	// 判定対象AとB座標
	Vector3 posA, posB, posAB;

	float radAB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	// 自キャラの敵弾の当たり判定
	#pragma region 
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets)
	{
		// 敵弾の取得
		posB = bullet->GetWorldPosition();

		radAB = (bullet->GetRadius() + player_->GetRadius()) *
		        (bullet->GetRadius() + player_->GetRadius());

		posAB = {
		    (posA.x - posB.x) * (posA.x - posB.x), 
			(posA.y - posB.y) * (posA.y - posB.y),
		    (posA.z - posB.z) * (posA.z - posB.z)};

		// 弾と弾の交差判定
		if (posAB.x + posAB.y + posAB.z <= radAB)
		{
			// 自キャラの衝突時コールバックを呼び出す
			player_->Oncollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->Oncollision();
		}

	}

	#pragma endregion


	// 自弾の敵キャラの当たり判定
    #pragma region
	// 敵キャラ座標
	posA = enemy_->GetWorldPosition();

	// 自弾と敵キャラすべての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 自弾の取得
		posB = bullet->GetWorldPosition();

		radAB = (bullet->GetRadius() + enemy_->GetRadius()) *
		        (bullet->GetRadius() + enemy_->GetRadius());

		posAB = {
		    (posA.x - posB.x) * (posA.x - posB.x), (posA.y - posB.y) * (posA.y - posB.y),
		    (posA.z - posB.z) * (posA.z - posB.z)};

		// 弾と弾の交差判定
		if (posAB.x + posAB.y + posAB.z <= radAB) {
			// 自弾の衝突時コールバックを呼び出す
			bullet->Oncollision();
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->Oncollision();
		}
	}

    #pragma endregion

	// 自弾の敵弾の当たり判定
    #pragma region
	// 敵弾座標
	for (EnemyBullet* eBullet : enemyBullets) {
		// 敵弾の取得
		posA = eBullet->GetWorldPosition();

		// 自弾と敵キャラすべての当たり判定
		for (PlayerBullet* pBullet : playerBullets) {
			// 自弾の取得
			posB = pBullet->GetWorldPosition();

			radAB = (pBullet->GetRadius() + eBullet->GetRadius()) *
			        (pBullet->GetRadius() + eBullet->GetRadius());

			posAB = {
			    (posA.x - posB.x) * (posA.x - posB.x), (posA.y - posB.y) * (posA.y - posB.y),
			    (posA.z - posB.z) * (posA.z - posB.z)};

			// 弾と弾の交差判定
			if (posAB.x + posAB.y + posAB.z <= radAB) {
				// 自弾の衝突時コールバックを呼び出す
				pBullet->Oncollision();
				// 敵弾の衝突時コールバックを呼び出す
				eBullet->Oncollision();
			}
		}
	}
    #pragma endregion
}
