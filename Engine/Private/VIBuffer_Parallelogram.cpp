#include "..\Public\VIBuffer_Parallelogram.h"

CVIBuffer_Parallelogram::CVIBuffer_Parallelogram(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Parallelogram::CVIBuffer_Parallelogram(const CVIBuffer_Parallelogram& rhs)
	: CVIBuffer{ rhs }
{
}

HRESULT CVIBuffer_Parallelogram::Initialize_Prototype()
{
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iIndexStride = 2;
	m_iNumVertices = 4;
	m_iNumIndices = 6;
	m_iNumVertexBuffers = 1;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	/* �����ϰ����ϴ� ������ �Ӽ��� �����ϳ�. */
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	/* �����ϰ����ϴ� ������ �ʱⰪ�� �����Ѵ�. */
	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

	pVertices[0].vPosition = XMFLOAT3(-0.3f, 0.5f, 0.f);
	pVertices[0].vTexcoord = XMFLOAT2(0.f, 0.f);

	pVertices[1].vPosition = XMFLOAT3(0.7f, 0.5f, 0.f);
	pVertices[1].vTexcoord = XMFLOAT2(1.f, 0.f);

	pVertices[2].vPosition = XMFLOAT3(0.3f, -0.5f, 0.f);
	pVertices[2].vTexcoord = XMFLOAT2(1.f, 1.f);

	pVertices[3].vPosition = XMFLOAT3(-0.7f, -0.5f, 0.f);
	pVertices[3].vTexcoord = XMFLOAT2(0.f, 1.f);

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion


#pragma region INDEX_BUFFER

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = /*m_iIndexStride*/0;

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Parallelogram::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Parallelogram* CVIBuffer_Parallelogram::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Parallelogram* pInstance = new CVIBuffer_Parallelogram(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent* CVIBuffer_Parallelogram::Clone(void* pArg)
{
	CVIBuffer_Parallelogram* pInstance = new CVIBuffer_Parallelogram(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Parallelogram::Free()
{
	__super::Free();
}