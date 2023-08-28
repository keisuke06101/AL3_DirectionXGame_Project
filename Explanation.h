#include "Model.h"
#include "Collider.h"
#include "WorldTransform.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "MathUtility.h"
#include "Matrix4x4.h"
#include <cassert>
#include <cmath>
#include <list>
#include <Sprite.h>
#pragma once

class Explanation {
public:
	
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Explanation();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Explanation();

	void Initialize(Model* model, uint32_t textureHandle, Vector3 playerPosition);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void Indication();

private:
	uint32_t textureHandle_ = 0u;

	// モデルデータ
	Model* model_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// キーボード入力
	Input* input_ = nullptr;

	bool isOpen_ = 0;
	bool isClose_ = 0;
};
