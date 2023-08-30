#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene()
{
	delete debugCamera_;
	delete model_;
	delete player_;
	delete boss_;
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
	delete explanation_;
	for (Enemy* enemy : enemy_) {
		delete enemy;
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	for (BossBullet* bullet : bossBullets_) {
		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("sample.png");
	enemyTextureHandle_ = TextureManager::Load("obake1.png");
	textureHandleE_ = TextureManager::Load("nu.png");
	model_ = Model::Create();
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	player_ = new Player;
	playerBullet_ = new PlayerBullet;
	enemyBullet_ = new EnemyBullet;
	boss_ = new Boss;
	boss_->Initialize(model_, {0.f, 5.f, 100.f});
	bossBullet_ = new BossBullet;
	skydome_ = new Skydome;
	railCamera_ = new RailCamera;
	viewProjection_.Initialize();

	explanation_ = new Explanation;
	explanation_->Initialize(model_, textureHandleE_, {0.f, 0.f, 20.f});

	railCamera_->Initialize({0, 0, 0}, {0,0,0});

	// 自キャラとレールカメラの親子関係を結ぶ
	player_->Initialize(model_, textureHandle_, {0.f, 0.f, 20.f});
	player_->SetParent(&railCamera_->GetWorldTransform());

	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_->Initialize(modelSkydome_);

	AddEnemy({0.f, 5.f, 100.f});
	LoadEnemyPopData();
}

void GameScene::Update() 
{

    #ifdef _DEBUG
    	if (input_->TriggerKey(DIK_BACKSPACE)) {
    		isDebugCameraActive_ = true;
    	}
    #endif // DEBUG 
	
	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	UpdateEnemyPopCommands();
	player_->Update(viewProjection_);
	for (Enemy* enemy : enemy_) {
	        enemy->Update();
	}
	// 弾更新
	for (EnemyBullet* bullet : enemyBullets_) {
		    bullet->Update();
	}
	boss_->Update();
	// ボス弾更新
	for (BossBullet* bullet : bossBullets_) {
		    bullet->Update();
	}

	skydome_->Update();

	if (isDebugCameraActive_)
	{
		// デバックカメラの更新
		debugCamera_->Update();

		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} 
	else if (!isDebugCameraActive_)
	{
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
	}
	CheckAllCollisions();

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
	/// 
	
	//explanation_->Draw(viewProjection_);

	player_->Draw(viewProjection_);
	
	for (Enemy* enemy : enemy_) {
		enemy->Draw(viewProjection_);
	}
	skydome_->Draw(viewProjection_);
	
	// 弾描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

	boss_->Draw(viewProjection_);
	// ボス弾更新
	for (BossBullet* bullet : bossBullets_) {
		bullet->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);
	player_->DrawUI();
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::ExplanationUpdate() 
{ explanation_->Update(); }

void GameScene::ExplanationDraw()
{
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
	///

	explanation_->Draw(viewProjection_);

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
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;
	// ボス弾リスト
	const std::list<BossBullet*>& bossBullets = bossBullets_;

	// コライダー
	std::list<Collider*> colliders_;
	// コライダーをリストに登録
	colliders_.push_back(player_);
	for (Enemy* enemy : enemy_) {
		colliders_.push_back(enemy);
	}
	// 自弾すべてについて
	for (PlayerBullet* pBullet : playerBullets) {
		colliders_.push_back(pBullet);
	}
	// 敵弾すべてについて
	for (EnemyBullet* bullet : enemyBullets) {
		// ペアの衝突判定
		CheckCollisionPair(player_, bullet);
	}
	// ボス弾すべてについて
	for (BossBullet* bullet : bossBullets) {
		// ペアの衝突判定
		CheckCollisionPair(player_, bullet);
	}
	
	// リスト内のペアに総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
	
		Collider* A = *itrA;

		// イテレータBはイテレータAの次の要素から回す（重複判定を回避）
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* B = *itrB;
			
			// ペアの当たり判定
			CheckCollisionPair(A, B);
		}
	}
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet)
{
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::AddEnemy(Vector3 pos) 
{
	Enemy* obj = new Enemy;
	// 敵
	obj->Initialize(model_, pos);
	// 敵キャラに自キャラのアドレスを渡す
	obj->SetPlayer(player_);
	// 敵キャラにゲームシーンを渡す
	obj->SetGameScene(this);
	
	enemy_.push_back(obj);
}

void GameScene::AddBossBullet(BossBullet* bossBullet)
{
	// リストに登録する
	bossBullets_.push_back(bossBullet);
}

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands()
{
	// 待機処理
	if (isWait)
	{
	    waitTimer--;
		if (waitTimer <= 0)
		{
			// 待機完了
			isWait = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
	    // 1行分の文字列をストr－無に変換して解析しやすくする
		std::istringstream line_stream(line);
		
		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// *//*から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を伸ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
		
		    // x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			AddEnemy(Vector3(x, y, z));
		}

		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');
			
			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWait = true;
			waitTimer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{
	// 衝突フィルタリング
	if (colliderA->GetCA() != colliderB->GetCM() || colliderB->GetCA() != colliderA->GetCM()) {
		return;
	}

	//// colliderAのワールド座標を取得
	//colliderA->GetWorldPosition();
	//// colliderBのワールド座標を取得
	//colliderB->GetWorldPosition();
	// 前計算
	// 判定対象AとB座標
	Vector3 posA, posB, posAB;
	posA = colliderA->GetWorldPosition();
	posB = colliderB->GetWorldPosition();

	float radAB;

	radAB = (colliderA->GetRadius() + colliderB->GetRadius()) *
	        (colliderA->GetRadius() + colliderB->GetRadius());

	posAB = {
	    (posA.x - posB.x) * (posA.x - posB.x),
		(posA.y - posB.y) * (posA.y - posB.y),
	    (posA.z - posB.z) * (posA.z - posB.z)};

	// 球と球の交差判定
	if (posAB.x + posAB.y + posAB.z <= radAB) {
		// コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision();
		// コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}

}
