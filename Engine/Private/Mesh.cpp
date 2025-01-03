#include "Mesh.h"
#include "Bone.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
    CVIBuffer(pDevice, pContext)
{
}

CMesh::CMesh(const CMesh& rhs):
    CVIBuffer(rhs), m_BoneOffset(rhs.m_BoneOffset), m_Bones(rhs.m_Bones), m_iNumBones(rhs.m_iNumBones), m_strName(rhs.m_strName), m_iMaterial_Index(rhs.m_iMaterial_Index),
	m_vMax(rhs.m_vMax),m_vMin(rhs.m_vMin)
{
}

HRESULT CMesh::Initialize_Prototype(const vector<VTXMESH>& vecVerticies, const vector<_uint>& vecIndicies, const MESH_DESC& desc)
{
    __super::Initialize_Prototype();
	m_iVertexStride = sizeof(VTXMESH);
	m_iIndexStride = 4;
	m_iNumVertices = vecVerticies.size();
	m_iNumIndices = vecIndicies.size();
	m_iNumVertexBuffers = 1;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	/* 생성하고자하는 버퍼의 속성을 설정하낟. */
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	/* 생성하고자하는 버퍼의 초기값을 설정한다. */
	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);
	m_vecVertexPositions.resize(m_iNumVertices);
	for (int i = 0; i < m_iNumVertices; ++i)
	{
		pVertices[i].vPosition = vecVerticies[i].vPosition;
		m_vecVertexPositions[i] = pVertices[i].vPosition;
		if (m_vMax.x < pVertices[i].vPosition.x)
			m_vMax.x = pVertices[i].vPosition.x;
		if (m_vMax.y < pVertices[i].vPosition.y)
			m_vMax.y = pVertices[i].vPosition.y;
		if (m_vMax.z < pVertices[i].vPosition.z)
			m_vMax.z = pVertices[i].vPosition.z;
		if (m_vMin.x > pVertices[i].vPosition.x)
			m_vMin.x = pVertices[i].vPosition.x;
		if (m_vMin.y > pVertices[i].vPosition.y)
			m_vMin.y = pVertices[i].vPosition.y;
		if (m_vMin.z > pVertices[i].vPosition.z)
			m_vMin.z = pVertices[i].vPosition.z;
		pVertices[i].vNormal = vecVerticies[i].vNormal;
		pVertices[i].vTexcoord = vecVerticies[i].vTexcoord;
		pVertices[i].vTangent = vecVerticies[i].vTangent;
	}

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
	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);
	m_vecIndicies.resize(m_iNumIndices);
	for (_uint i = 0; i < m_iNumIndices; ++i)
	{
		pIndices[i] = vecIndicies[i];
		m_vecIndicies[i] = vecIndicies[i];
	}

	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	m_strName = desc.m_strName;
	m_iMaterial_Index = desc.iMaterial_Index;

	return S_OK;
}

HRESULT CMesh::Initialize_Prototype(const vector<VTXANIMMESH>& vecVerticies, const vector<_uint>& vecIndicies, const vector<_float4x4>& vecOffset, const vector<CBone*>& Bones, const MESH_DESC& desc)
{
	__super::Initialize_Prototype();
	m_BoneOffset = vecOffset;
	m_iVertexStride = sizeof(VTXANIMMESH);
	m_iIndexStride = 4;
	m_iNumVertices = vecVerticies.size();
	m_iNumIndices = vecIndicies.size();
	m_iNumVertexBuffers = 1;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	/* 생성하고자하는 버퍼의 속성을 설정하낟. */
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	/* 생성하고자하는 버퍼의 초기값을 설정한다. */
	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);
	m_vecVertexPositions.resize(m_iNumVertices);
	m_vMax = _float3{-100000.f,-100000.f, -100000.f};
	m_vMin = _float3{100000.f,100000.f, 100000.f};
	for (int i = 0; i < m_iNumVertices; ++i)
	{
		pVertices[i].vPosition = vecVerticies[i].vPosition;
		m_vecVertexPositions[i] = pVertices[i].vPosition;
		//if (m_vMax.x < pVertices[i].vPosition.x)
		//	m_vMax.x = pVertices[i].vPosition.x;
		//if (m_vMax.y < pVertices[i].vPosition.y)
		//	m_vMax.y = pVertices[i].vPosition.y;
		//if (m_vMax.z < pVertices[i].vPosition.z)
		//	m_vMax.z = pVertices[i].vPosition.z;
		//if (m_vMin.x > pVertices[i].vPosition.x)
		//	m_vMin.x = pVertices[i].vPosition.x;
		//if (m_vMin.y > pVertices[i].vPosition.y)
		//	m_vMin.y = pVertices[i].vPosition.y;
		//if (m_vMin.z > pVertices[i].vPosition.z)
		//	m_vMin.z = pVertices[i].vPosition.z;

		pVertices[i].vNormal = vecVerticies[i].vNormal;
		pVertices[i].vTexcoord = vecVerticies[i].vTexcoord;
		pVertices[i].vTangent = vecVerticies[i].vTangent;
		pVertices[i].vBlendIndex0 = vecVerticies[i].vBlendIndex0;
		pVertices[i].vBlendIndex1 = vecVerticies[i].vBlendIndex1;
		//pVertices[i].vBlendIndex2 = vecVerticies[i].vBlendIndex2;
		//pVertices[i].vBlendIndex3 = vecVerticies[i].vBlendIndex3;
		//pVertices[i].vBlendIndex4 = vecVerticies[i].vBlendIndex4;
		//pVertices[i].vBlendIndex5 = vecVerticies[i].vBlendIndex5;
		pVertices[i].vBlendWeight0 = vecVerticies[i].vBlendWeight0;
		pVertices[i].vBlendWeight1 = vecVerticies[i].vBlendWeight1;
		//pVertices[i].vBlendWeight2 = vecVerticies[i].vBlendWeight2;
		//pVertices[i].vBlendWeight3 = vecVerticies[i].vBlendWeight3;
		//pVertices[i].vBlendWeight4 = vecVerticies[i].vBlendWeight4;
		//pVertices[i].vBlendWeight5 = vecVerticies[i].vBlendWeight5;
	}

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
	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);
	m_vecIndicies.resize(m_iNumIndices);
	for (_uint i = 0; i < m_iNumIndices; ++i)
	{
		pIndices[i] = vecIndicies[i];
		m_vecIndicies[i] = vecIndicies[i];
	}

	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	m_strName = desc.m_strName;
	m_iMaterial_Index = desc.iMaterial_Index;

	m_iNumBones = desc.iNumBones;
	m_Bones.resize(m_iNumBones);
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		m_Bones[i] = desc.ArrBones[i];
	}
	
	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		_uint			iBoneIndex = { 0 };

		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone)->_bool
			{
				if (m_strName == pBone->Get_Name())
					return true;

				++iBoneIndex;

				return false;
			});

		m_Bones.emplace_back(iBoneIndex);

		_float4x4		OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());
		m_BoneOffset.emplace_back(OffsetMatrix);
	}


	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
    return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const vector<VTXMESH>& vecVerticies, const vector<_uint>& vecIndicies, const MESH_DESC& desc)
{
    CMesh* pInstance = new CMesh(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(vecVerticies, vecIndicies, desc)))
    {
        MSG_BOX(TEXT("Failed to Created : CMesh"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const vector<VTXANIMMESH>& vecVerticies, const vector<_uint>& vecIndicies, const vector<_float4x4>& vecOffset, 
	const vector<CBone*>& Bones, const MESH_DESC& desc)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(vecVerticies, vecIndicies, vecOffset, Bones, desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CMesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMesh::Clone(void* pArg)
{
    CMesh* pInstance = new CMesh(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CMesh"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMesh::Free()
{
	__super::Free();
}
