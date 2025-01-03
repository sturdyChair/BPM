#include "VIBuffer_Instance.h"

CVIBuffer_Instance::CVIBuffer_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	CVIBuffer(pDevice, pContext)

{
}

CVIBuffer_Instance::CVIBuffer_Instance(const CVIBuffer_Instance& rhs) :
	CVIBuffer(rhs)
	//, m_pVBInstance(rhs.m_pVBInstance)
	, m_tDesc(rhs.m_tDesc)
	, m_iInstanceVertexStride(rhs.m_iInstanceVertexStride)
	, m_iNumIndexPerInstance(rhs.m_iNumIndexPerInstance)
	, m_pSpeeds(rhs.m_pSpeeds)
	, m_vecInitPos(rhs.m_vecInitPos)
	, m_vecTorque(rhs.m_vecTorque)
{
}

HRESULT CVIBuffer_Instance::Initialize_Prototype(const INSTANCE_DESC* desc)
{
	m_tDesc = *desc;

	m_pSpeeds = new _float[m_tDesc.iNumInstance];
	for (_uint i = 0; i < m_tDesc.iNumInstance; ++i)
	{
		m_pSpeeds[i] = fRand(m_tDesc.vSpeed.x, m_tDesc.vSpeed.y);
	}

	return S_OK;
}

HRESULT CVIBuffer_Instance::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Instance::Render()
{
	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
		m_pVBInstance
	};

	_uint					iStrides[] = {
		m_iVertexStride,
		m_iInstanceVertexStride,
	};

	_uint					iOffsets[] = {
		0,
		0,
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	m_pContext->DrawIndexedInstanced(m_iNumIndexPerInstance, m_tDesc.iNumInstance, 0, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer_Instance::Bind_Buffer()
{
	ID3D11Buffer* pVertexBuffers[] = {
	m_pVB,
	m_pVBInstance
	};

	_uint					iStrides[] = {
		m_iVertexStride,
		m_iInstanceVertexStride,
	};

	_uint					iOffsets[] = {
		0,
		0,
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);
	return S_OK;
}

HRESULT CVIBuffer_Instance::Draw()
{
	m_pContext->DrawIndexedInstanced(m_iNumIndexPerInstance, m_tDesc.iNumInstance, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Instance::Reset()
{

}

void CVIBuffer_Instance::Free()
{
	__super::Free();
	Safe_Release(m_pVBInstance);
	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pSpeeds);
		Safe_Delete_Array(m_vecInitPos);
		Safe_Delete_Array(m_vecTorque);
	}
}
