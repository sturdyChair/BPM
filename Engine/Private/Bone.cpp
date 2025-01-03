#include "Bone.h"

CBone::CBone()
{
}

CBone::~CBone()
{
}

HRESULT CBone::Initialize(BONE_DESC& desc)
{
	m_strName = desc.strName;
	m_strParentName = desc.strParentName;
	m_TransformationMatrix = desc.Transformation;
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
	return S_OK;
}

void CBone::Update(_fmatrix ParentMatrix)
{
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * ParentMatrix);
}

void CBone::Set_TransformationMatrix(_fmatrix matrix)
{
	XMStoreFloat4x4(&m_TransformationMatrix, matrix);
}

void CBone::Add_Child(_int iIdx)
{
	m_iChildrenIdx.push_back(iIdx);
}

CBone* CBone::Create(BONE_DESC& desc)
{
	CBone* pInstance = new CBone;

	if (FAILED(pInstance->Initialize(desc)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBone* CBone::Clone()
{
	return new CBone(*this);
}

void CBone::Free()
{
	__super::Free();
}
