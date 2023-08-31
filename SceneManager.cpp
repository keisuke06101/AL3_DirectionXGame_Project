#include "SceneManager.h"

SceneManager::~SceneManager() 
{ 
	delete gameScene_;
}

void SceneManager::Initialize() { 

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	//　ゲームシーンの初期化
	gameScene_ = new GameScene;
	gameScene_->Initialize();
	// 操作説明の初期化
	//explanation_ = new Explanation;
	//explanation_->Initialize(model_, textureHandle_, {0, 0, 0});

	sceneNo_ = Title; 
}

void SceneManager::Update() 
{
	switch (sceneNo_) {
	case Title:
		if (input_->PushKey(DIK_SPACE))
		{
			sceneNo_ = Game;
			//gameScene_->Initialize();
		}
		gameScene_->ExplanationUpdate();
		break;
	case Game:
		
		gameScene_->Update();
		if (gameScene_->GetIsGameOver() == true)
		{
			sceneNo_ = GameOver;
		}

		break;
	case GameClear:
		
		break;
	case GameOver:
		
		if (input_->PushKey(DIK_BACKSPACE)) {
			sceneNo_ = Game;
			Initialize();
		}

		break;
	default:
		break;
	}
}

void SceneManager::Draw() 
{
	switch (sceneNo_) {
	case Title:

		gameScene_->ExplanationDraw();

		break;
	case Game:

		gameScene_->Draw();

		break;
	case GameClear:

		break;
	case GameOver:

		break;
	default:
		break;
	}
}
