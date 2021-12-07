#include <App/TangentModel.h>
#include <System/Graphics/Geometory.h>

TangentModel::TangentModel()
	: m_pBuffer(nullptr), m_pVtx(nullptr)
#ifdef _DEBUG
	, m_dbgVtxCount(0), m_dbgStride(0), m_dbgPosOffset(0), m_dbgTangentOffset(0)
#endif
{
}
TangentModel::~TangentModel()
{
	SAFE_DELETE_ARRAY(m_pVtx);
	SAFE_DELETE(m_pBuffer);
}
HRESULT TangentModel::Make(MakeDesc desc)
{
	UINT tangentStride = desc.bufDesc.vtxSize + sizeof(DirectX::XMFLOAT3);
	m_pVtx = new char[tangentStride * desc.bufDesc.vtxCount];

	char* pDst = m_pVtx;
	char* pDstRoot = pDst;
	const char* pSrc = reinterpret_cast<const char*>(desc.bufDesc.pVtx);

	//--- 接ベクトル以外の頂点データをコピー
	for (UINT i = 0; i < desc.bufDesc.vtxCount; ++i)
	{
		memcpy(pDst, pSrc, desc.bufDesc.vtxSize);
		pDst += tangentStride;
		pSrc += desc.bufDesc.vtxSize;
	}
	pDst = pDstRoot;

	//--- 接ベクトルを計算
	bool useIndex = desc.bufDesc.pIdx;
	std::vector<UINT> duplicateVtxCount(desc.bufDesc.vtxCount); // 重複頂点数
	// 面数の計算
	UINT faceNum;
	switch (desc.bufDesc.topology)
	{
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		faceNum = (useIndex ? desc.bufDesc.idxCount : desc.bufDesc.vtxCount) / 3;
		break;
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		faceNum = (useIndex ? desc.bufDesc.idxCount : desc.bufDesc.vtxCount) - 2;
		break;
	default:
		SAFE_DELETE_ARRAY(m_pVtx);
		return E_FAIL; // 未対応
	}
	// 面ごとに接ベクトルを計算
	for (UINT i = 0; i < faceNum; ++i)
	{
		// インデックスの割り出し
		UINT index[3];
		if (FAILED(CalcIndex(index, desc.bufDesc, i)))
		{
			SAFE_DELETE_ARRAY(m_pVtx);
			return E_FAIL;
		}
		
		// 面を構成するデータを吸出し
		DirectX::XMFLOAT3 *pTangents[] =
		{
			reinterpret_cast<DirectX::XMFLOAT3*>(pDst + tangentStride * index[0] + desc.bufDesc.vtxSize),
			reinterpret_cast<DirectX::XMFLOAT3*>(pDst + tangentStride * index[1] + desc.bufDesc.vtxSize),
			reinterpret_cast<DirectX::XMFLOAT3*>(pDst + tangentStride * index[2] + desc.bufDesc.vtxSize),
		};
		DirectX::XMFLOAT3 pos[] =
		{
			*reinterpret_cast<DirectX::XMFLOAT3*>(pDst + tangentStride * index[0] + desc.posOffset),
			*reinterpret_cast<DirectX::XMFLOAT3*>(pDst + tangentStride * index[1] + desc.posOffset),
			*reinterpret_cast<DirectX::XMFLOAT3*>(pDst + tangentStride * index[2] + desc.posOffset),
		};
		DirectX::XMFLOAT2 uv[] =
		{
			*reinterpret_cast<DirectX::XMFLOAT2*>(pDst + tangentStride * index[0] + desc.uvOffset),
			*reinterpret_cast<DirectX::XMFLOAT2*>(pDst + tangentStride * index[1] + desc.uvOffset),
			*reinterpret_cast<DirectX::XMFLOAT2*>(pDst + tangentStride * index[2] + desc.uvOffset),
		};

		// 接ベクトルを計算
		DirectX::XMFLOAT3 T = CalcTangent(pos[0], uv[0], pos[1], uv[1], pos[2], uv[2]);
		*pTangents[0] = T;
		*pTangents[1] = T;
		*pTangents[2] = T;
		duplicateVtxCount[index[0]] ++;
		duplicateVtxCount[index[1]] ++;
		duplicateVtxCount[index[2]] ++;
	}
	// 重複数で接ベクトルを平均化
	for (UINT i = 0; i < desc.bufDesc.vtxCount; ++i)
	{
		DirectX::XMFLOAT3* pTangent =
			reinterpret_cast<DirectX::XMFLOAT3*>(pDst + tangentStride * i + desc.bufDesc.vtxSize);
		pTangent->x /= duplicateVtxCount[i];
		pTangent->y /= duplicateVtxCount[i];
		pTangent->z /= duplicateVtxCount[i];
	}

	// 全部できたらバッファ作成
	DXBuffer::Desc bufDesc = desc.bufDesc;
	bufDesc.pVtx = m_pVtx;
	bufDesc.vtxSize = tangentStride;

	m_pBuffer = new DXBuffer;
	HRESULT hr = m_pBuffer->Create(bufDesc);
	if (FAILED(hr))
	{
		SAFE_DELETE_ARRAY(m_pVtx);
		SAFE_DELETE(m_pBuffer);
		return hr;
	}

#ifdef _DEBUG
	m_dbgVtxCount = desc.bufDesc.vtxCount;
	m_dbgStride = tangentStride;
	m_dbgPosOffset = desc.posOffset;
	m_dbgTangentOffset = desc.bufDesc.vtxSize;
#endif

	return hr;
}

void TangentModel::Draw()
{
	if (m_pBuffer)
	{
		m_pBuffer->Draw();
	}

#if 1
#ifdef _DEBUG
	// 接ベクトルデバッグ表示
	for (UINT i = 0; i < m_dbgVtxCount; ++i)
	{
		DirectX::XMFLOAT3 V =
			*reinterpret_cast<DirectX::XMFLOAT3*>(m_pVtx + m_dbgStride * i + m_dbgPosOffset);
		DirectX::XMFLOAT3 T =
			*reinterpret_cast<DirectX::XMFLOAT3*>(m_pVtx + m_dbgStride * i + m_dbgTangentOffset);
		T.x = V.x + T.x * 3.0f;
		T.y = V.y + T.y * 3.0f;
		T.z = V.z + T.z * 3.0f;
		AddLine(V, T);
	}
	DrawLines();
#endif
#endif
}

HRESULT TangentModel::CalcIndex(UINT *pOut, DXBuffer::Desc &desc, UINT face)
{
	bool useIndex = desc.pIdx;
	switch (desc.topology)
	{
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		if (useIndex)
		{
			for (UINT i = 0; i < 3; ++i)
			{
				switch (desc.idxSize)
				{
					case 1: pOut[i] = reinterpret_cast<const BYTE*>(desc.pIdx)[face * 3 + i]; break;
					case 2: pOut[i] = reinterpret_cast<const WORD*>(desc.pIdx)[face * 3 + i]; break;
					case 4: pOut[i] = reinterpret_cast<const DWORD*>(desc.pIdx)[face * 3 + i]; break;
				}
			}
		}
		else
		{
			for (UINT i = 0; i < 3; ++i)
				pOut[i] = face * 3 + i;
		}
		break;
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		if (useIndex)
		{
			for (UINT i = 0; i < 3; ++i)
			{
				switch (desc.idxSize)
				{
				case 1: pOut[i] = reinterpret_cast<const BYTE*>(desc.pIdx)[face + i]; break;
				case 2: pOut[i] = reinterpret_cast<const WORD*>(desc.pIdx)[face + i]; break;
				case 4: pOut[i] = reinterpret_cast<const DWORD*>(desc.pIdx)[face + i]; break;
				}
			}
		}
		else
		{
			for (UINT i = 0; i < 3; ++i)
				pOut[i] = face + i;
		}
		break;
	default:
		return E_FAIL;
	}

	return S_OK;
}

DirectX::XMFLOAT3 TangentModel::CalcTangent(
	DirectX::XMFLOAT3 pos0, DirectX::XMFLOAT2 uv0,
	DirectX::XMFLOAT3 pos1, DirectX::XMFLOAT2 uv1,
	DirectX::XMFLOAT3 pos2, DirectX::XMFLOAT2 uv2)
{
	DirectX::XMFLOAT3 v0(0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 v1(0.f, 0.f, 0.f);
	DirectX::XMFLOAT2 t0(0.f, 0.f);
	DirectX::XMFLOAT2 t1(0.f, 0.f);

	float t = t0.x * t1.y - t1.x * t0.y;
	DirectX::XMFLOAT3 tangent
	(
		(t1.y * v0.x - t0.y * v1.x) / t,
		(t1.y * v0.y - t0.y * v1.y) / t,
		(t1.y * v0.z - t0.y * v1.z) / t
	);
	
	DirectX::XMStoreFloat3
	(
		&tangent,
		DirectX::XMVector3Normalize
		(
			DirectX::XMLoadFloat3(&tangent)
		)
	);

	return tangent;
	
}