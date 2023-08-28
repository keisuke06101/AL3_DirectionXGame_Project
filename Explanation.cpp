#include "Explanation.h"

Explanation::Explanation() {}

Explanation::~Explanation() 
{ 
	//delete model_;
}

void Explanation::Initialize(Model* model, uint32_t textureHandle, Vector3 position) {
	// NULLポイントチェック
	assert(model);

	// 受け渡し
	model_ = model;
	textureHandle_ = textureHandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();


}

void Explanation::Update() { 

	ImGui::Begin("Explanation"); 
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("position", sliderValue, -100.0f, 20.0f);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	Indication();

	// 行列を定数バッファーに転送
	worldTransform_.UpdateMatrix();
}

void Explanation::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Explanation::Indication() 
{
	const float kMoveSpeed = 3.0f;

	if (input_->TriggerKey(DIK_M) && !isClose_)
	{
		isOpen_ = true;
	}
	if (input_->TriggerKey(DIK_C) && !isOpen_) {
		isClose_ = true;
	}

	if (isOpen_)
	{
		worldTransform_.translation_.z -= kMoveSpeed * 2.f;
		if (worldTransform_.translation_.z <= -47.0f)
		{
			isOpen_ = false;
			worldTransform_.translation_.z = -47.0f;
		}
	}

	if (isClose_) {
		worldTransform_.translation_.z += kMoveSpeed * 2.f;
		if (worldTransform_.translation_.z >= 100.0f) {
			isClose_ = false;
			worldTransform_.translation_.z = 100.0f;
		}
	}

}
