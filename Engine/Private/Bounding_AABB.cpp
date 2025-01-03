#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

CBounding_AABB::CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	CBounding(pDevice, pContext)
{
}

HRESULT CBounding_AABB::Initialize(CBounding::BOUNDING_DESC* pDesc)
{
	if (!pDesc)
	{
		return E_FAIL;
	}
	BOUNDING_AABB_DESC* desc = (BOUNDING_AABB_DESC*)pDesc;

	m_pBoundingBox_Local = new BoundingBox(desc->vCenter, desc->vExtents);
	m_pBoundingBox = new BoundingBox(*m_pBoundingBox_Local);

	return S_OK;
}

HRESULT CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	DX::Draw(pBatch, *m_pBoundingBox, Colors::Pink);

	return S_OK;
}

void CBounding_AABB::Update(_fmatrix WorldMatrix)
{

	m_pBoundingBox_Local->Transform(*m_pBoundingBox, 
		XMMatrixScaling(XMVectorGetX(XMVector3Length(WorldMatrix.r[0])),
						XMVectorGetX(XMVector3Length(WorldMatrix.r[1])), 
						XMVectorGetX(XMVector3Length(WorldMatrix.r[2]))) 
					  * XMMatrixTranslationFromVector(WorldMatrix.r[3])
	);
}

_bool CBounding_AABB::Intersect(CBounding* pOther, COLLIDER_TYPE eOthType)
{
	m_bCollided = false;
	switch (eOthType)
	{
	case Engine::COLLIDER_TYPE::SPHERE:
	{
		CBounding_Sphere* pSphere = static_cast<CBounding_Sphere*>(pOther);
		m_bCollided = m_pBoundingBox->Intersects(*pSphere->Get_Desc());
	}
	break;
	case Engine::COLLIDER_TYPE::AABB:
	{
		CBounding_AABB* pAABB = static_cast<CBounding_AABB*>(pOther);
		m_bCollided = m_pBoundingBox->Intersects(*pAABB->Get_Desc());
	}
	break;
	case Engine::COLLIDER_TYPE::OBB:
	{
		CBounding_OBB* pOBB = static_cast<CBounding_OBB*>(pOther);
		m_bCollided = m_pBoundingBox->Intersects(*pOBB->Get_Desc());
	}
	break;
	default:
		break;
	}
	return m_bCollided;
}

CBounding_AABB* CBounding_AABB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CBounding::BOUNDING_DESC* pDesc)
{
	auto pInstance = new CBounding_AABB(pDevice, pContext);
	pInstance->Initialize(pDesc);

	return pInstance;
}

void CBounding_AABB::Free()
{
	Safe_Delete(m_pBoundingBox);
	Safe_Delete(m_pBoundingBox_Local);
	__super::Free();
}
