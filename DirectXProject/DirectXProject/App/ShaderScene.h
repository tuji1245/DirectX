#ifndef __SHADER_SCENE_H__
#define __SHADER_SCENE_H__

#include <System/Scene/SceneBase.h>
#include <System/Graphics/DXBuffer.h>
#include <system/Graphics/Shader.h>
#include <DirectXMath.h> // イランかも
#include <system/Graphics/Texture.h>
#include <App/FBXModel.h>
#include <App/TangentModel.h>

class ShaderScene : public Scene
{
public:
	SCENE_DECLARATION(ShaderScene);
private:
	// 頂点シェーダに渡す変換行列
	struct Matrix
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 proj;
	};

private:
	DXBuffer* m_pPrimitive;
	VertexShader* m_pSampleVS;
	VertexShader* m_pBumpVS;
	VertexShader* m_pToonOutlineVS;
	PixelShader* m_pSamplePS;
	PixelShader* m_pSepiaPS;
	PixelShader* m_pLambertPS;
	PixelShader* m_pPhongPS;
	PixelShader* m_pLimlightPS;
	PixelShader* m_pFogPS;
	PixelShader* m_pMultiTexturePS;
	PixelShader* m_pDissolvePS;
	PixelShader* m_pBumpPS;
	PixelShader* m_pToonPS;
	PixelShader* m_pToonOutlinesPS;
	
	ConstantBuffer* m_pMatrixCB;
	ConstantBuffer* m_pLightCB;
	ConstantBuffer* m_pCameraCB;
	ConstantBuffer* m_pParameterCB;


	Matrix m_vsMatrix;
	DirectX::XMFLOAT4 m_lightDirection;
	DirectX::XMFLOAT4 m_cameraPos;
	DirectX::XMFLOAT4 m_parameter;


	TextureFromFile* m_pTexture;
	TextureFromFile* m_pScarTexture;
	TextureFromFile* m_pDissolveTexture;
	TextureFromFile* m_pNormalMapTexture;
	TextureFromFile* m_pRampTexture;
	
	FBXModel* m_pModel;
	FBXModel* m_pModelTown;
	TangentModel* m_pTangentModel;
};

#endif // __SAMPLE_SCENE_H__