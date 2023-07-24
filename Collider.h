#include "Vector3.h"
#include "CollisionConfig.h"
#pragma once

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider 
{
private:
	//衝突半径
	float radius_ = 1.0f;

	// 衝突属性（自分）
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突属性（相手）
	uint32_t collisionMask_ = 0xffffffff;

public:
	// 半径を取得
	const float GetRadius() { return radius_; }
	
	// 半径を設定
	void SetRadius(float radius) { radius_ = radius; }
	
	// 衝突時に呼ばれる関数
	virtual void OnCollision();
	
	// ワールド座標を取得(純粋仮想関数)
	virtual Vector3 GetWorldPosition() { return Vector3{}; }

	// 衝突属性（自分）を取得
	const uint32_t GetCA() { return collisionAttribute_; }
	// 衝突属性（自分）を設定
	void SetCA(uint32_t ca) { collisionAttribute_ = ca; }
	// 衝突マスク（相手）を取得
	const uint32_t GetCM() { return collisionMask_; }
	// 衝突マスク（相手）を設定
	void SetCM(uint32_t cm) { collisionMask_ = cm; }
};
