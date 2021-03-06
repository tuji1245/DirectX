#ifndef __CAMERA_H__
#define __CAMERA_H__

// DirectXが提供している
// 数学系のライブラリ
#include <DirectXMath.h>
#include <System\Graphics\Shader.h>

class Camera
{
public:
	Camera();
	~Camera();

	void Init();
	void Uninit();
	void Update();

	DirectX::XMFLOAT4& GetPos() { return m_pos; }
	DirectX::XMMATRIX GetView();
	DirectX::XMMATRIX GetProj();
private:
	DirectX::XMFLOAT4 m_pos;
	DirectX::XMFLOAT4 m_look;
	DirectX::XMFLOAT4 m_up;
	float m_near;
	float m_far;

	POINT m_oldMousePos;
};

#endif // __CAMERA_H__