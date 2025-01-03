#include "Collider.h"
#include "GameInstance.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CComponent(pDevice, pContext)
{
}

CCollider::CCollider(const CCollider& rhs):
	CComponent(rhs), m_eType(rhs.m_eType)
#ifdef _DEBUG
	,m_pBatch(rhs.m_pBatch), m_pEffect(rhs.m_pEffect), m_pInputLayout(rhs.m_pInputLayout)
#endif // _DEBUG
{
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif
}

HRESULT CCollider::Initialize_Prototype(COLLIDER_TYPE eType)
{
	m_eType = eType;

#ifdef _DEBUG
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);
	m_pEffect->SetVertexColorEnabled(true);
	const void* pSBC;
	size_t iSBCLen;

	m_pEffect->GetVertexShaderBytecode(&pSBC, &iSBCLen);
	m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pSBC, iSBCLen, &m_pInputLayout);
#endif

	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	switch (m_eType)
	{
	case Engine::COLLIDER_TYPE::SPHERE:
		m_pBounding = CBounding_Sphere::Create(m_pDevice, m_pContext, (CBounding::BOUNDING_DESC*)pArg);
		break;
	case Engine::COLLIDER_TYPE::AABB:
		m_pBounding = CBounding_AABB::Create(m_pDevice, m_pContext, (CBounding::BOUNDING_DESC*)pArg);
		break;
	case Engine::COLLIDER_TYPE::OBB:
		m_pBounding = CBounding_OBB::Create(m_pDevice, m_pContext, (CBounding::BOUNDING_DESC*)pArg);
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CCollider::Render()
{
#ifdef _DEBUG
	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(m_pGameInstance->Get_Transform_Matrix(D3DTS_VIEW));
	m_pEffect->SetProjection(m_pGameInstance->Get_Transform_Matrix(D3DTS_PROJ));

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pEffect->Apply(m_pContext);

	m_pBatch->Begin();

	m_pBounding->Render(m_pBatch);

	m_pBatch->End();

#endif // _DEBUG
	return S_OK;
}

void CCollider::Update(_fmatrix WorldMatrix)
{
	m_pBounding->Update(WorldMatrix);
}

_bool CCollider::Intersect(CCollider* pOther)
{
	return m_pBounding->Intersect(pOther->Get_Bounding(), pOther->Get_Collider_Type());
}


CCollider* CCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, COLLIDER_TYPE eType)
{
	CCollider* pInstance = new CCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType)))
	{
		MSG_BOX(TEXT("Failed to Create : CCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider::Clone(void* pArg)
{
	CCollider* pInstance = new CCollider(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
#ifdef _DEBUG
	if (!m_isCloned)
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}
	Safe_Release(m_pInputLayout);
#endif
	Safe_Release(m_pBounding);
	__super::Free();
}
