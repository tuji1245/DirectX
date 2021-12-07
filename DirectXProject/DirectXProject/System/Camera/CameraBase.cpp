#include "CameraBase.h"

Camera::Camera()
	: m_pos(0.0f, 0.0f, -1.0f), m_look(0.0f, 0.0f, 0.0f), m_up(0.0f, 1.0f, 0.0f)
	, m_screenSize(1280.0f, 720.0f), m_nearClip(0.1f), m_farClip(100.0f)
	, m_fov(DirectX::XMConvertToRadians(60.0f))
	, m_isEnable(false)
{
	m_lookAtMatrix = m_projectionMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&m_lookAt, m_lookAtMatrix);
	DirectX::XMStoreFloat4x4(&m_projection, m_projectionMatrix);
}
Camera::~Camera()
{
}
void Camera::Execute()
{
	if (m_isEnable)
	{
		Update();

		m_lookAtMatrix = DirectX::XMMatrixLookAtLH(
			DirectX::XMLoadFloat3(&m_pos),
			DirectX::XMLoadFloat3(&m_look),
			DirectX::XMLoadFloat3(&m_up));
		m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fov, GetAspect(), m_nearClip, m_farClip);
		DirectX::XMStoreFloat4x4(&m_lookAt, m_lookAtMatrix);
		DirectX::XMStoreFloat4x4(&m_projection, m_projectionMatrix);
	}
}
void Camera::OnActive(const Camera& camera)
{
}
Camera& Camera::operator=(const Camera& camera)
{
	m_pos	= camera.m_pos;
	m_look	= camera.m_look;
	m_up	= camera.m_up;
	m_screenSize	= camera.m_screenSize;
	m_nearClip		= camera.m_nearClip;
	m_farClip		= camera.m_farClip;
	m_fov			= camera.m_fov;
	return *this;
}
void Camera::Enable(bool enable)
{
	m_isEnable = enable;
}
bool Camera::IsEnable() const
{
	return m_isEnable;
}

const DirectX::XMFLOAT3& Camera::GetPos() const
{
	return m_pos;
}
const DirectX::XMFLOAT3& Camera::GetLook() const
{
	return m_look;
}
const DirectX::XMFLOAT3& Camera::GetUp() const
{
	return m_up;
}
const DirectX::XMFLOAT2& Camera::GetScreenSize() const
{
	return m_screenSize;
}
float Camera::GetAspect() const
{
	return m_screenSize.x / m_screenSize.y;
}
float Camera::GetNearClip() const
{
	return m_nearClip;
}
float Camera::GetFarClip() const
{
	return m_farClip;
}
float Camera::GetFOV() const
{
	return m_fov;
}
const DirectX::XMFLOAT4X4& Camera::GetLookAt() const
{
	return m_lookAt;
}
const DirectX::XMFLOAT4X4& Camera::GetProjection() const
{
	return  m_projection;
}
const DirectX::XMMATRIX& Camera::GetLookAtMatrix() const
{
	return m_lookAtMatrix;
}
const DirectX::XMMATRIX& Camera::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}