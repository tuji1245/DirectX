#include "CameraDebug.h"
#include <System/Input.h>

CameraDebug::CameraDebug()
	:m_oldMousePos{0,0}
{
	m_pos = DirectX::XMFLOAT3(5.0f, 5.0f, -5.0f);
	m_farClip = 1000.0f;
}
CameraDebug::~CameraDebug()
{
}
void CameraDebug::OnActive(const Camera& camera)
{
	*static_cast<Camera*>(this) = camera;
}
void CameraDebug::Update()
{
	POINT mousePos = GetMousePos();
	POINT mouseDist = {
		mousePos.x - m_oldMousePos.x,
		mousePos.y - m_oldMousePos.y,
	};
	DirectX::XMFLOAT2 screenSize = GetScreenSize();
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

	if (IsKeyPress(VK_MENU))
	{
		// �r���[�̉�荞��
		if (IsKeyTrigger('1'))
		{
			m_pos = m_look;
			if(IsKeyPress(VK_SHIFT)) { m_pos.x = -focus; }
			else { m_pos.x = focus; }
			m_up = DirectX::XMFLOAT3(0, 1, 0);
		}
		else if (IsKeyTrigger('2'))
		{
			m_pos = m_look;
			if (IsKeyPress(VK_SHIFT)) { m_pos.y = -focus; }
			else { m_pos.y = focus; }
			m_up = DirectX::XMFLOAT3(0, 0, 1);
		}
		else if (IsKeyTrigger('3'))
		{
			m_pos = m_look;
			if (IsKeyPress(VK_SHIFT)) { m_pos.z = -focus; }
			else { m_pos.z = focus; }
			m_up = DirectX::XMFLOAT3(0, 1, 0);
		}
		else if(mouseDist.x != 0 || mouseDist.y != 0)
		{
			// ���{�^��(�J������荞��
			if (IsKeyPress(VK_LBUTTON))
			{
				// ��]��
				float angleX = 360.f * mouseDist.x / screenSize.x;	// ��ʈ����360�x��](��ʃT�C�Y�̔�����180�x��])
				float angleY = 180.f * mouseDist.y / screenSize.y;	// ��ʈ����180�x��](��ʃT�C�Y�̔�����90�x��])

				// ��]�ʂ����ƂɁA�e���̉�]�s����v�Z
				DirectX::XMVECTOR rotPos = DirectX::XMVectorSubtract(vPos, vLook);
				DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleX));
				DirectX::XMVECTOR rotAxis = DirectX::XMVector3TransformCoord(vSide, rotY);
				DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationAxis(rotAxis, DirectX::XMConvertToRadians(angleY));

				// �����_�����_�Ƃ��ĉ�]
				rotPos = DirectX::XMVector3TransformCoord(rotPos, rotY);
				rotPos = DirectX::XMVector3TransformCoord(rotPos, rotX);
				DirectX::XMStoreFloat3(&m_pos, DirectX::XMVectorAdd(rotPos, vLook));

				// �A�b�v�x�N�g�����v�Z
				vUp = DirectX::XMVector3Cross(
					DirectX::XMVector3Normalize(DirectX::XMVectorScale(rotPos, -1.0f)), rotAxis);
				DirectX::XMStoreFloat3(&m_up, DirectX::XMVector3Normalize(vUp));
			}
			// ���{�^��(�J�����ړ�
			else if (IsKeyPress(VK_MBUTTON))
			{
				float rate = focus / m_farClip;
				float aspect = GetAspect();
				// ���A�A����B�Ƃ���O�p�`�ɂ��� tan�� = A / B�����藧��
				// ��L�������ƂɊ���o�������i�̈ړ��ʂƁA�t�H�[�J�X�ʒu / ���i �̔䗦����A�J�����̈ړ��ʂ����߂�
				float farMoveX = tanf(DirectX::XM_PI / 6.0f) * m_farClip * -mouseDist.x * aspect / (screenSize.x * 0.5f);
				float farMoveY = tanf(DirectX::XM_PI / 6.0f) * m_farClip *  mouseDist.y / (screenSize.y * 0.5f);
				// �p���s������ƂɃJ�������ړ�
				DirectX::XMVECTOR vMove = DirectX::XMVectorAdd(DirectX::XMVectorScale(vSide, farMoveX * rate), DirectX::XMVectorScale(vUp, farMoveY * rate));
				DirectX::XMStoreFloat3(&m_pos, DirectX::XMVectorAdd(vPos, vMove));
				DirectX::XMStoreFloat3(&m_look, DirectX::XMVectorAdd(vLook, vMove));
			}
			// �E�{�^��(�����ύX
			else if (IsKeyPress(VK_RBUTTON))
			{
				DirectX::XMStoreFloat3(&m_pos, DirectX::XMVectorAdd(vPos, DirectX::XMVectorScale(vFront, focus * mouseDist.y * 0.01f)));
			}
		}
	}
	else
	{
		// �X�N���[��
		int wheel = GetMouseWheel();
		if (wheel != 0)
		{
			DirectX::XMStoreFloat3(&m_pos, DirectX::XMVectorAdd(vPos, DirectX::XMVectorScale(vFront, focus * wheel * 0.1f)));
		}
	}


	m_oldMousePos = mousePos;

}