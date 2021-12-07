#include <System/Graphics/Text.h>
#include <System/Graphics/Geometory.h>
#include <System/Graphics/Shader.h>

Text::Text()
	: m_pVertex(nullptr)
	, m_pBuffer(nullptr)
{
}
Text::~Text()
{
}
HRESULT Text::Create(int textCount)
{
	Release();
	
	// テキスト表示用ポリゴンデータ
	UINT vtxCount = textCount * 4;
	UINT idxCount = textCount * 6;
	m_pVertex = new Vertex[vtxCount];
	BYTE *pIdx = new BYTE[idxCount];
	for(UINT i = 0; i < textCount; ++ i)
	{
		pIdx[i * 6 + 0] = i * 4;
		pIdx[i * 6 + 1] = i * 4 + 1;
		pIdx[i * 6 + 2] = i * 4 + 2;
		pIdx[i * 6 + 3] = i * 4 + 1;
		pIdx[i * 6 + 4] = i * 4 + 3;
		pIdx[i * 6 + 5] = i * 4 + 2;
	}

	// バッファ作成
	DXBuffer::Desc desc = {};
	m_pBuffer = new DXBuffer;
	desc.pVtx = m_pVertex;
	desc.vtxSize = sizeof(Vertex);
	desc.vtxCount = vtxCount;
	desc.isWrite = true;
	desc.pIdx = pIdx;
	desc.idxSize = sizeof(BYTE);
	desc.idxCount = idxCount;
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	HRESULT hr = m_pBuffer->Create(desc);

	// 終了処理
	if(SUCCEEDED(hr))
	{
		m_textCount = textCount;
	}
	SAFE_DELETE_ARRAY(pIdx);

	return hr;
}

void Text::Release()
{
	SAFE_DELETE(m_pBuffer);
	SAFE_DELETE_ARRAY(m_pVertex);
}

void Text::SetPos(const DirectX::XMFLOAT3& pos)
{
	m_pos = pos;
}
void Text::Draw(const char* text)
{
	// テキスト用シェーダ設定

	// UV切り替え
	for (UINT i = 0; i < m_textCount; ++i)
	{
		UINT idx = i * 4;
		if(*text != '\0')
		{
			for (int j = 0; j < 4; ++j)
			{
				m_pVertex[idx + j].pos = DirectX::XMFLOAT3((idx + j % 2) * 20.0f, (j / 2) * 20.0f, 0.0f);
				m_pVertex[idx + j].uv = DirectX::XMFLOAT2(j % 2, j / 2);
			}
			++text;
		}
		else
		{
			for (int j = 0; j < 4; ++j)
			{
				m_pVertex[idx + j].pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			}
		}
	}
	m_pBuffer->Write(m_pVertex);

	// 描画
	//SetTexture(m_pFont->GetTexture()->GetResource());
	m_pBuffer->Draw();
}