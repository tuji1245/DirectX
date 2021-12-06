#pragma once
#include "Scripts/DX11Manager.h"
/**
* @file Camera.h
* @breif カメラ
*
* @date 2021/06/20
* @author 辻悠希
*/

class Camera
{
public:
	Camera(XMFLOAT3 pos, XMFLOAT3 targetPos, XMFLOAT3 upVec);
	~Camera();

	void Update();
	void Terminate();

private:
	XMFLOAT3 m_targetPos;	//! 見ている対象の位置
	XMFLOAT3 m_pos;			//! 自身の位置
	XMFLOAT3 m_upVec;			//! アップベクトル

	float m_fAspectRatio;	//! アスペクト比
	float m_fFovY;			//! 視野角
	float m_fNearZ;			//! ニアクリップ
	float m_fFarZ;			//! ファークリップ

	XMFLOAT4X4 m_mtxWorld;	//! ワールドマトリクス
	XMFLOAT4X4 m_mtxView;	//! ビューマトリクス
	XMFLOAT4X4 m_mtxProj;	//! プロジェクションマトリクス

	// 定数
	static constexpr XMFLOAT3	DEF_POS = { 0.0f,200.0f,-700.0f };	// カメラの視点初期位置
	static constexpr XMFLOAT3	DEF_TARGETPOS = { 0.0f,0.0f,0.0f };	// カメラの注視点初期位置
	static constexpr float VIEW_ANGLE = 45.0f;	// 視野角
	static constexpr float VIEW_ASPECT = ((float)defWindowRECT.right / (float)defWindowRECT.bottom);	// アスペクト比
	static constexpr float VIEW_NEAR_Z = 1.0f;
	static constexpr float VIEW_FAR_Z = 10000.0f;

private:
	void CalcWorldMatrix();
	void UpdateMatrix();

};