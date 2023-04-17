#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle)
{ 
	//NULLポイントチェック
	assert(model); 

	//受け渡し
	model_ = model;

	//ワールド変換の初期化
	worldTransform_.Initialize();

}

void Player::Update()
{
	//行列を定数バッファーに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw()
{

} 