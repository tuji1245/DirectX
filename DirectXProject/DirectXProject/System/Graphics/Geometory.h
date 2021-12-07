#ifndef __GEOMETORY_H__
#define __GEOMETORY_H__

#include <Windows.h>
#include <DirectXMath.h>

HRESULT InitGeometory();
void UninitGeometory();

void AddLine(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const DirectX::XMFLOAT4 color = { 1,1,1,1 });
void SetGeometoryMatrix(const DirectX::XMFLOAT4X4& world);
void SetGeometoryVP(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj);
void SetGeometoryColor(const DirectX::XMFLOAT4& color);
void DrawLines();
void DrawPolygon();
void DrawCube();

#endif // __GEOMETORY_H__