#include <cassert>
#include "Skydome.h"

void Skydome::Initialize(Model* model) 
{
	assert(model);

	// 受け渡し
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 位置の初期化
	worldTransform_.translation_ = {0, 0, 0};

}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection& view) {
	// 3Dモデルの描画
	model_->Draw(worldTransform_, view);
}
