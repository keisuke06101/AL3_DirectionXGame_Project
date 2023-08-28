﻿#include "RailCamera.h"

void RailCamera::Initialize(Vector3 trans, Vector3 rot) 
{
	worldTransform_.Initialize();
	// ワールドトランスフォーム座標を受け取ってワールドトランスフォームに設定
	worldTransform_.translation_ = trans;
	worldTransform_.rotation_ = rot;

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update() 
{
	// ワールドトランスフォームの座標と角度の加算処理
	Vector3 move = {0.f, 0.f, 0.f};
	Vector3 rot = {0.f, 0.00f, 0.f};

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.rotation_.x += rot.x;
	worldTransform_.rotation_.y += rot.y;
	worldTransform_.rotation_.z += rot.z;

	worldTransform_.UpdateMatrix();
	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	viewProjection_.TransferMatrix();

	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");

	ImGui::SliderFloat3("CameraTranslate", &worldTransform_.translation_.x, 0.0f, 10.f);
	ImGui::SliderFloat3("CameraRotate", &worldTransform_.rotation_.x, 0.0f, 10.f);
	
	ImGui::End();
}
