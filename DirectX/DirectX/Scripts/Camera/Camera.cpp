#include "Camera.h"

Camera::Camera(XMFLOAT3 pos, XMFLOAT3 targetPos, XMFLOAT3 upVec) :
	m_pos(pos), m_targetPos(targetPos), m_upVec(upVec),
	m_fAspectRatio(VIEW_ASPECT), m_fFovY(VIEW_ANGLE),
	m_fNearZ(VIEW_NEAR_Z), m_fFarZ(VIEW_FAR_Z)
{	
	CalcWorldMatrix();
}
Camera::~Camera()
{
	Terminate();
}

void Camera::Update()
{
	UpdateMatrix();
}
void Camera::Terminate()
{

}

void Camera::CalcWorldMatrix()
{
	XMFLOAT3 vX;
	XMFLOAT3 vY;
	XMFLOAT3 vZ;

	XMVECTOR vecZ = XMVectorSet(
		m_targetPos.x - m_pos.x,
		m_targetPos.y - m_pos.y,
		m_targetPos.z - m_pos.z,
		0.0f
	);
	XMVECTOR vecY = XMLoadFloat3(&m_upVec);
	XMVECTOR vecX = XMVector3Normalize(
		XMVector3Cross(vecY, vecZ)
	);

	XMVECTOR normalVecZ = XMVector3Normalize(vecZ);
	vecY = XMVector3Normalize(
		XMVector3Cross(vecZ, vecX)
	);

	XMStoreFloat3(&vX, vecX);
	XMStoreFloat3(&vY, vecY);
	XMStoreFloat3(&vZ, normalVecZ);

	m_mtxWorld._11 = vX.x;
	m_mtxWorld._12 = vX.y;
	m_mtxWorld._13 = vX.z;
	m_mtxWorld._14 = 0.0f;
	m_mtxWorld._21 = vY.x;
	m_mtxWorld._22 = vY.y;
	m_mtxWorld._23 = vY.z;
	m_mtxWorld._24 = 0.0f;
	m_mtxWorld._31 = vZ.x;
	m_mtxWorld._32 = vZ.y;
	m_mtxWorld._33 = vZ.z;
	m_mtxWorld._34 = 0.0f;
	m_mtxWorld._41 = m_pos.x;
	m_mtxWorld._42 = m_pos.y;
	m_mtxWorld._43 = m_pos.z;
	m_mtxWorld._44 = 1.0f;
}

void Camera::UpdateMatrix()
{
	DirectX::XMMATRIX view = XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&m_pos),
		DirectX::XMLoadFloat3(&m_targetPos),
		DirectX::XMLoadFloat3(&m_upVec)
	);
	DirectX::XMMATRIX proj = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fFovY),
		m_fAspectRatio,
		m_fNearZ, m_fFarZ
	);

	DirectX::XMStoreFloat4x4(&m_mtxView, view);
	DirectX::XMStoreFloat4x4(&m_mtxProj, proj);

	//XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(
	//	XMLoadFloat3(&m_pos), XMLoadFloat3(&m_targetPos), XMLoadFloat3(&m_upVec)));
	//XMStoreFloat4x4(&m_mtxProj, XMMatrixPerspectiveFovLH(
	//	XMConvertToRadians(m_fFovY), m_fAspectRatio, m_fNearZ, m_fFarZ));
}