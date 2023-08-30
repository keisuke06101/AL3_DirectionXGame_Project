#include "Score.h"

void Score::Initialize() 
{
	uint32_t textureHandle_[10] = {

	    TextureManager::Load(".0.png"), TextureManager::Load(".1.png"),
	    TextureManager::Load(".2.png"), TextureManager::Load(".3.png"),
	    TextureManager::Load(".4.png"), TextureManager::Load(".5.png"),
	    TextureManager::Load(".6.png"), TextureManager::Load(".7.png"),
	    TextureManager::Load(".8.png"), TextureManager::Load(".9.png"),

	};
	
		// スプライト生成
		sprite2DScore_ =
		    Sprite::Create(textureHandle_[0], {240.f, 60.f}, {1, 1, 1, 1}, {0.5f, 0.5f});
	
	
}

void Score::Update()
{
	score_ += 1;
	// 桁数(万～一)ごとに計算して　保存用変数にnumberを代入
	saveScore_ = score_;

	eachScore_[0] = score_ / 10000;
	score_ = score_ % 10000;

	eachScore_[1] = score_ / 1000;
	score_ = score_ % 1000;

	eachScore_[2] = score_ / 100;
	score_ = score_ % 100;

	eachScore_[3] = score_ / 10;
	score_ = score_ % 10;

	eachScore_[4] = score_ / 1;
	score_ = score_ % 1;
	// numberに保存用変数を代入
	score_ = saveScore_;
}

void Score::Draw()
{
	// timeHandleを0~5で表示
	sprite2DScore_->Draw();
}
