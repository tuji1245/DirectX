#pragma once

#include "Scripts/System/Scene.h"

#include "Scripts/MyImgui/DebugWindowManager.h"
#include "Scripts/System/Shader.h"
#include "Scripts/System/DXBuffer.h"


class SampleScene : public Scene
{
public:
	SCENE_DECLARATION(SampleScene);

private:
	VertexShader* m_pDefaultVS;
	PixelShader* m_pDefaultPS;
	//InputLayout* m_pInputLayout;

	DXBuffer* m_pPrimitive;

	//VertexBuffer* m_VB;
	ConstantBuffer* m_CB;
	//IndexBuffer* m_IndexBuffer;
	ConstantBuffer* m_IndexBuffer;

	
};