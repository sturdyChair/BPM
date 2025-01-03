#include "Bounding.h"

CBounding::CBounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	m_pDevice(pDevice),m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

}

HRESULT CBounding::Initialize()
{
	return S_OK;
}

HRESULT CBounding::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	return S_OK;
}

//CBounding* CBounding::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
//{
//	auto pInstance =
//		new CBounding(pDevice, pContext);
//	pInstance->Initialize();
//
//	return pInstance;
//}

void CBounding::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	__super::Free();
}
