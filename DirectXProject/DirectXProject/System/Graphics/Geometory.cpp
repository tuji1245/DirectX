#include "Geometory.h"
#include "DXBuffer.h"
#include <System/Graphics/Shader.h>

const UINT LineNum = 1000;
const UINT LineGeomerotyNum = LineNum * 2;

const char GeometoryNormalVS[] = R"EOT(\
cbuffer Matrix : register(b0) {
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};
float4 main(float3 pos : POSITION0) : SV_POSITION {
	float4 svPos = float4(pos, 1);
	svPos = mul(svPos, world);
	svPos = mul(svPos, view);
	svPos = mul(svPos, proj);
	return svPos;
})EOT";
const char GeometoryLineVS[] = R"EOT(\
struct VS_IN {
	float3 pos : POSITION0;
	float4 color : COLOR0;
};
struct VS_OUT {
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
};
cbuffer Matrix : register(b0) {
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};
VS_OUT main(VS_IN vin) {
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	vout.color = vin.color;
	return vout;
})EOT";
const char GeometoryNormalPS[] = R"EOT(\
cbuffer Material : register(b0) {
	float4 diffuse;
};
float4 main(float4 pos : SV_POSITION) : SV_TARGET {
	return diffuse;
})EOT";
const char GeometoryLinePS[] = R"EOT(\
float4 main(float4 pos : SV_POSITION, float4 color : COLOR0) : SV_TARGET {
	return color;
})EOT";

struct GeometoryLineVertex
{
	float pos[3];
	float color[4];
};
struct GeometoryMatrix
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 proj;
};

UINT g_lineIndex;
GeometoryLineVertex g_lineData[LineGeomerotyNum];
DXBuffer *g_pLines;
DXBuffer *g_pPolygon;
DXBuffer *g_pCube;
VertexShader *g_pGeometoryVS[2];
PixelShader* g_pGeometoryPS[2];
ConstantBuffer *g_pMatrixCB;
ConstantBuffer *g_pMaterialCB;
GeometoryMatrix g_geometoryMatrix;
DirectX::XMFLOAT4 g_geometoryMaterial;

HRESULT InitGeometory()
{
	HRESULT hr;
	const float d = 0.5f;
	DXBuffer::Desc desc = {};

	//--- Lines
	desc.pVtx = g_lineData;
	desc.vtxSize = sizeof(GeometoryLineVertex);
	desc.vtxCount = LineGeomerotyNum;
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	desc.isWrite = true;
	g_lineIndex = 0;
	g_pLines = new DXBuffer;
	hr = g_pLines->Create(desc);
	if (FAILED(hr)) { return hr; }

	//--- Polygon
	DirectX::XMFLOAT3 vtxPolygon[] = {
		{-d, d, 0.0f}, {d, d, 0.0f}, {-d, -d, 0.0f}, {d, -d, 0.0f}
	};
	desc.pVtx = vtxPolygon;
	desc.vtxSize = sizeof(DirectX::XMFLOAT3);
	desc.vtxCount = _countof(vtxPolygon);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	desc.isWrite = false;
	g_pPolygon = new DXBuffer;
	hr = g_pPolygon->Create(desc);
	if (FAILED(hr)) { return hr; }

	//--- Cube
	DirectX::XMFLOAT3 vtxCube[] = {
		// Žè‘O‚Ì–Ê
		{-d, d,-d}, { d, d,-d}, {-d,-d,-d}, { d, d,-d}, { d,-d,-d}, {-d,-d,-d},
		// ‰E‚Ì–Ê
		{ d, d,-d}, { d, d, d}, { d,-d,-d}, { d, d, d}, { d,-d, d}, { d,-d,-d},
		// Œã‚ë‚Ì–Ê
		{ d, d, d}, {-d, d, d}, { d,-d, d}, {-d, d, d}, {-d,-d, d}, { d,-d, d},
		// ¶‚Ì–Ê
		{-d, d, d}, {-d, d,-d}, {-d,-d, d}, {-d, d,-d}, {-d,-d,-d}, {-d,-d, d},
		// ã‚Ì–Ê
		{-d, d, d}, { d, d, d}, {-d, d,-d}, { d, d, d}, { d, d,-d}, {-d, d,-d},
		// ‰º‚Ì–Ê
		{-d,-d,-d}, { d,-d,-d}, {-d,-d, d}, { d,-d,-d}, { d,-d, d}, {-d,-d, d},
	};
	desc.pVtx = vtxCube;
	desc.vtxSize = sizeof(DirectX::XMFLOAT3);
	desc.vtxCount = _countof(vtxCube);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	g_pCube = new DXBuffer;
	hr = g_pCube->Create(desc);
	if (FAILED(hr)) { return hr; }

	//--- Shader
	g_pGeometoryVS[0] = new VertexShader;
	g_pGeometoryVS[1] = new VertexShader;
	g_pGeometoryPS[0] = new PixelShader;
	g_pGeometoryPS[1] = new PixelShader;
	g_pMatrixCB = new ConstantBuffer;
	g_pMaterialCB = new ConstantBuffer;
	hr = g_pGeometoryVS[0]->Compile(GeometoryNormalVS, sizeof(GeometoryNormalVS));
	if (FAILED(hr)) { return hr; }
	hr = g_pGeometoryVS[1]->Compile(GeometoryLineVS, sizeof(GeometoryLineVS));
	if (FAILED(hr)) { return hr; }
	hr = g_pGeometoryPS[0]->Compile(GeometoryNormalPS, sizeof(GeometoryNormalPS));
	if (FAILED(hr)) { return hr; }
	hr = g_pGeometoryPS[1]->Compile(GeometoryLinePS, sizeof(GeometoryLinePS));
	if (FAILED(hr)) { return hr; }
	hr = g_pMatrixCB->Create(sizeof(g_geometoryMatrix));
	if (FAILED(hr)) { return hr; }
	hr = g_pMaterialCB->Create(sizeof(g_geometoryMaterial));
	if (FAILED(hr)) { return hr; }

	DirectX::XMStoreFloat4x4(&g_geometoryMatrix.world, DirectX::XMMatrixIdentity());
	SetGeometoryColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	return hr;
}
void UninitGeometory()
{
	safe_delete(g_pLines);
	safe_delete(g_pPolygon);
	safe_delete(g_pCube);
	safe_delete(g_pGeometoryVS[0]);
	safe_delete(g_pGeometoryVS[1]);
	safe_delete(g_pGeometoryPS[0]);
	safe_delete(g_pGeometoryPS[1]);
	safe_delete(g_pMatrixCB);
	safe_delete(g_pMaterialCB);
}

void AddLine(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const DirectX::XMFLOAT4 color)
{
	if (g_lineIndex < LineNum)
	{
		g_lineData[g_lineIndex].pos[0] = start.x;
		g_lineData[g_lineIndex].pos[1] = start.y;
		g_lineData[g_lineIndex].pos[2] = start.z;
		g_lineData[g_lineIndex].color[0] = color.x;
		g_lineData[g_lineIndex].color[1] = color.y;
		g_lineData[g_lineIndex].color[2] = color.z;
		g_lineData[g_lineIndex].color[3] = color.w;
		++g_lineIndex;
		g_lineData[g_lineIndex].pos[0] = end.x;
		g_lineData[g_lineIndex].pos[1] = end.y;
		g_lineData[g_lineIndex].pos[2] = end.z;
		g_lineData[g_lineIndex].color[0] = color.x;
		g_lineData[g_lineIndex].color[1] = color.y;
		g_lineData[g_lineIndex].color[2] = color.z;
		g_lineData[g_lineIndex].color[3] = color.w;
		++g_lineIndex;
	}
}
void SetGeometoryMatrix(const DirectX::XMFLOAT4X4& world)
{
	g_geometoryMatrix.world = world;
	g_pMatrixCB->Write(&g_geometoryMatrix);
}
void SetGeometoryVP(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj)
{
	g_geometoryMatrix.view = view;
	g_geometoryMatrix.proj = proj;
	g_pMatrixCB->Write(&g_geometoryMatrix);
}
void SetGeometoryColor(const DirectX::XMFLOAT4& color)
{
	g_geometoryMaterial = color;
	g_pMaterialCB->Write(&g_geometoryMaterial);
}
void DrawLines()
{
	g_pLines->Write(g_lineData);
	g_pMatrixCB->BindVS(0);
	g_pGeometoryVS[1]->Bind();
	g_pGeometoryPS[1]->Bind();
	g_pLines->Draw();
	g_lineIndex = 0;
}
void DrawPolygon()
{
	g_pMatrixCB->BindVS(0);
	g_pMaterialCB->BindPS(0);
	g_pGeometoryVS[0]->Bind();
	g_pGeometoryPS[0]->Bind();
	g_pPolygon->Draw();
}
void DrawCube()
{
	g_pMatrixCB->BindVS(0);
	g_pMaterialCB->BindPS(0);
	g_pGeometoryVS[0]->Bind();
	g_pGeometoryPS[0]->Bind();
	g_pCube->Draw();
}