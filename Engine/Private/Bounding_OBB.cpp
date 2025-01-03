#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

CBounding_OBB::CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	CBounding(pDevice, pContext)
{
}

HRESULT CBounding_OBB::Initialize(CBounding::BOUNDING_DESC* pDesc)
{
	if (!pDesc)
	{
		return E_FAIL;
	}
	BOUNDING_OBB_DESC* desc = (BOUNDING_OBB_DESC*)pDesc;
	_vector vRotYawPitchRoll = XMLoadFloat3(&desc->vRotation);
	_float4 vQuatOrient;
	XMStoreFloat4(&vQuatOrient, XMQuaternionRotationRollPitchYawFromVector(vRotYawPitchRoll));

	m_pBoundingBox_Local = new BoundingOrientedBox(desc->vCenter, desc->vExtents, vQuatOrient);
	m_pBoundingBox = new BoundingOrientedBox(*m_pBoundingBox_Local);

	return S_OK;
}

HRESULT CBounding_OBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	DX::Draw(pBatch, *m_pBoundingBox, Colors::Pink);
	return S_OK;
}

void CBounding_OBB::Update(_fmatrix WorldMatrix)
{
	m_pBoundingBox_Local->Transform(*m_pBoundingBox, WorldMatrix);
}

_bool CBounding_OBB::Intersect(CBounding* pOther, COLLIDER_TYPE eOthType)
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

CBounding_OBB* CBounding_OBB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CBounding::BOUNDING_DESC* pDesc)
{
	auto pInstance = new CBounding_OBB(pDevice, pContext);
	pInstance->Initialize(pDesc);

	return pInstance;
}

void CBounding_OBB::Free()
{
	Safe_Delete(m_pBoundingBox);
	Safe_Delete(m_pBoundingBox_Local);
	__super::Free();
}
