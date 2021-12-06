#pragma once
#include "Scripts/DX11Manager.h"
/**
* @file Camera.h
* @breif �J����
*
* @date 2021/06/20
* @author �җI��
*/

class Camera
{
public:
	Camera(XMFLOAT3 pos, XMFLOAT3 targetPos, XMFLOAT3 upVec);
	~Camera();

	void Update();
	void Terminate();

private:
	XMFLOAT3 m_targetPos;	//! ���Ă���Ώۂ̈ʒu
	XMFLOAT3 m_pos;			//! ���g�̈ʒu
	XMFLOAT3 m_upVec;			//! �A�b�v�x�N�g��

	float m_fAspectRatio;	//! �A�X�y�N�g��
	float m_fFovY;			//! ����p
	float m_fNearZ;			//! �j�A�N���b�v
	float m_fFarZ;			//! �t�@�[�N���b�v

	XMFLOAT4X4 m_mtxWorld;	//! ���[���h�}�g���N�X
	XMFLOAT4X4 m_mtxView;	//! �r���[�}�g���N�X
	XMFLOAT4X4 m_mtxProj;	//! �v���W�F�N�V�����}�g���N�X

	// �萔
	static constexpr XMFLOAT3	DEF_POS = { 0.0f,200.0f,-700.0f };	// �J�����̎��_�����ʒu
	static constexpr XMFLOAT3	DEF_TARGETPOS = { 0.0f,0.0f,0.0f };	// �J�����̒����_�����ʒu
	static constexpr float VIEW_ANGLE = 45.0f;	// ����p
	static constexpr float VIEW_ASPECT = ((float)defWindowRECT.right / (float)defWindowRECT.bottom);	// �A�X�y�N�g��
	static constexpr float VIEW_NEAR_Z = 1.0f;
	static constexpr float VIEW_FAR_Z = 10000.0f;

private:
	void CalcWorldMatrix();
	void UpdateMatrix();

};