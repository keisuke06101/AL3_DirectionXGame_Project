#include "Vector3.h"
#pragma once

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider 
{
private:
	//衝突半径
	float radius_ = 1.0f;

public:
	// 半径を取得
	const float GetRadius() { return radius_; }
	
	// 半径を設定
	void SetRadius(float radius) { radius_ = radius; }
	
	// 衝突時に呼ばれる関数
	virtual void OnCollision();
	
	// ワールド座標を取得(純粋仮想関数)
	virtual Vector3 GetWorldPosition() { return Vector3{}; }
};
