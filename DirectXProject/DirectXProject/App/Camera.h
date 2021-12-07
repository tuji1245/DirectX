#ifndef __CAMERA_H__
#define __CAMERA_H__

// DirectX‚ª’ñ‹Ÿ‚µ‚Ä‚¢‚é
// ”ŠwŒn‚Ìƒ‰ƒCƒuƒ‰ƒŠ
#include <DirectXMath.h>
#include "Shader.h"

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