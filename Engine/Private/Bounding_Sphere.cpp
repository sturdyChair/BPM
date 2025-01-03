#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

CBounding_Sphere::CBounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBounding(pDevice, pContext)
{
}

HRESULT CBounding_Sphere::Initialize(CBounding::BOUNDING_DESC* pDesc)
{
	if (!pDesc)
	{
		return E_FAIL;
	}
	BOUNDING_SPHERE_DESC* desc = (BOUNDING_SPHERE_DESC*)pDesc;

	m_pBoundingSphere_Local = new BoundingSphere(desc->vCenter, desc->fRadius);
	m_pBoundingSphere = new BoundingSphere(*m_pBoundingSphere_Local);
	return S_OK;
}

HRESULT CBounding_Sphere::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	DX::Draw(pBatch, *m_pBoundingSphere, Colors::Pink);
	return S_OK;
}

void CBounding_Sphere::Update(_fmatrix WorldMatrix)
{
	m_pBoundingSphere_Local->Transform(*m_pBoundingSphere, WorldMatrix);
}

_bool CBounding_Sphere::Intersect(CBounding* pOther, COLLIDER_TYPE eOthType)
{
	m_bCollided = false;
	switch (eOthType)
	{
	case Engine::COLLIDER_TYPE::SPHERE:
	{
		CBounding_Sphere* pSphere = static_cast<CBounding_Sphere*>(pOther);
		m_bCollided = m_pBoundingSphere->Intersects(*pSphere->Get_Desc());
	}
		break;
	case Engine::COLLIDER_TYPE::AABB:
	{
		CBounding_AABB* pAABB = static_cast<CBounding_AABB*>(pOther);
		m_bCollided = m_pBoundingSphere->Intersects(*pAABB->Get_Desc());
	}
		break;
	case Engine::COLLIDER_TYPE::OBB:
	{
		CBounding_OBB* pOBB = static_cast<CBounding_OBB*>(pOther);
		m_bCollided = m_pBoundingSphere->Intersects(*pOBB->Get_Desc());
	}
		break;
	default:
		break;
	}
	return m_bCollided;
}

CBounding_Sphere* CBounding_Sphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CBounding::BOUNDING_DESC* pDesc)
{
	auto pInstance = new CBounding_Sphere(pDevice, pContext);
	pInstance->Initialize(pDesc);

	return pInstance;
}

void CBounding_Sphere::Free()
{
	Safe_Delete(m_pBoundingSphere);
	Safe_Delete(m_pBoundingSphere_Local);
	__super::Free();
}
