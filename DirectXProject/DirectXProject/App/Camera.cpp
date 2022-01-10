#include "Camera.h"
#include <System\Input.h>
#include "Defines.h"

const DirectX::XMFLOAT4 CameraInitPos(0, 0, -5.0f, 0);
const DirectX::XMFLOAT4 CameraInitLook(0, 0, 0, 0);

Camera::Camera()
	: m_pos(CameraInitPos)
	, m_look(CameraInitLook)
	, m_up(0, 1.0f, 0, 0)
	, m_near(0.1f)
	, m_far(100.f)
{
}
Camera::~Camera()
{
}

void Camera::Init()
{
}
void Camera::Uninit()
{
}
void Camera::Update()
{
	POINT mousePos = GetMousePos();
	POINT mouseDist = {
		mousePos.x - m_oldMousePos.x,
		mousePos.y - m_oldMousePos.y,
	};
	DirectX::XMVECTOR vPos = DirectX::XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 0);
	DirectX::XMVECTOR vLook = DirectX::XMVectorSet(m_look.x, m_look.y, m_look.z, 0);
	DirectX::XMVECTOR vFront = DirectX::XMVectorSubtract(vLook, vPos);
	DirectX::XMVECTOR vUp = DirectX::XMVectorSet(m_up.x, m_up.y, m_up.z, 0);
	DirectX::XMVECTOR vSide;
	float focus = 0.0f;

	DirectX::XMStoreFloat(&focus, DirectX::XMVector3Length(vFront));
	vFront = DirectX::XMVector3Normalize(vFront);
	vUp = DirectX::XMVector3Normalize(vUp);
	vSide = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vUp, vFront));
	vUp = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vFront, vSide));

	// ���{�^���_�u���N���b�N(������
	if (IsMouseDoubleClick(VK_LBUTTON))
	{
		m_pos = CameraInitPos;
		m_look = CameraInitLook;
	}
	else if (mouseDist.x != 0 || mouseDist.y != 0)
	{
		// ���{�^��(�J������荞��
		if (IsKeyPress(VK_LBUTTON))
		{
			// ��]��
			float angleX = 360.f * mouseDist.x / SCREEN_WIDTH;	// ��ʈ����360�x��](��ʃT�C�Y�̔�����180�x��])
			float angleY = 180.f * mouseDist.y / SCREEN_HEIGHT;	// ��ʈ����180�x��](��ʃT�C�Y�̔�����90�x��])

			// ��]�ʂ����ƂɁA�e���̉�]�s����v�Z
			DirectX::XMVECTOR rotPos = DirectX::XMVectorSubtract(vPos, vLook);
			DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleX));
			DirectX::XMVECTOR rotAxis = DirectX::XMVector3TransformCoord(vSide, rotY);
			DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationAxis(rotAxis, DirectX::XMConvertToRadians(angleY));

			// �����_�����_�Ƃ��ĉ�]
			rotPos = DirectX::XMVector3TransformCoord(rotPos, rotY);
			rotPos = DirectX::XMVector3TransformCoord(rotPos, rotX);
			DirectX::XMStoreFloat4(&m_pos, DirectX::XMVectorAdd(rotPos, vLook));
		}
		// ���{�^��(�J�����ړ�
		else if (IsKeyPress(VK_MBUTTON))
		{
			float rate = focus / m_far;
			float aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
			// ���A�A����B�Ƃ���O�p�`�ɂ��� tan�� = A / B�����藧��
			// ��L�������ƂɊ���o�������i�̈ړ��ʂƁA�t�H�[�J�X�ʒu / ���i �̔䗦����A�J�����̈ړ��ʂ����߂�
			float farMoveX = tanf(DirectX::XM_PI / 6.0f) * m_far * -mouseDist.x * aspect / (SCREEN_WIDTH * 0.5f);
			float farMoveY = tanf(DirectX::XM_PI / 6.0f) * m_far * mouseDist.y / (SCREEN_HEIGHT * 0.5f);
			// �p���s������ƂɃJ�������ړ�
			DirectX::XMVECTOR vMove = DirectX::XMVectorAdd(DirectX::XMVectorScale(vSide, farMoveX * rate), DirectX::XMVectorScale(vUp, farMoveY * rate));
			DirectX::XMStoreFloat4(&m_pos, DirectX::XMVectorAdd(vPos, vMove));
			DirectX::XMStoreFloat4(&m_look, DirectX::XMVectorAdd(vLook, vMove));
		}
		// �E�{�^��(�����ύX
		else if (IsKeyPress(VK_RBUTTON))
		{
			DirectX::XMStoreFloat4(&m_pos, DirectX::XMVectorAdd(vPos, DirectX::XMVectorScale(vFront, focus * mouseDist.y * 0.01f)));
		}
	}
	else
	{
		// �X�N���[��
		int wheel = GetMouseWheel();
		if (wheel != 0)
		{
			DirectX::XMStoreFloat4(&m_pos, DirectX::XMVectorAdd(vPos, DirectX::XMVectorScale(vFront, focus * wheel * 0.1f)));
		}
	}

	m_oldMousePos = mousePos;
}

DirectX::XMMATRIX Camera::GetView()
{
	return 
		DirectX::XMMatrixLookAtLH(
			DirectX::XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 0),
			DirectX::XMVectorSet(m_look.x, m_look.y, m_look.z, 0),
			DirectX::XMVectorSet(m_up.x, m_up.y, m_up.z, 0)
		);
}
DirectX::XMMATRIX Camera::GetProj()
{
	return
		DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XM_PI / 3.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, m_near, m_far
		);
}