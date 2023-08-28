#pragma once
#include "Input.h"
#include "GameScene.h"

class SceneManager 
{
public:

	~SceneManager();

	void Initialize();

	void Update();

	void Draw();

private:

	enum scene {
		Title,
		Game,
		GameClear,
		GameOver
	};

	int sceneNo_ = 0;

	uint32_t textureHandle_ = 0;

	Model* model_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// キーボード入力
	Input* input_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// 操作説明
	//Explanation* explanation_ = nullptr;
};
