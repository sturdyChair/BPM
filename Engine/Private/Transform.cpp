#include "..\Public\Transform.h"
#include "Shader.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent{ pDevice, pContext }
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent{ rhs },
	m_WorldMatrix(rhs.m_WorldMatrix)
{
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{

	if (nullptr == pArg)
	{
		m_fSpeedPerSec = 1.f;
		m_fRotationPerSec = 1.f;
		return S_OK;
	}

	TRANSFORM_DESC*  pTransformDesc = static_cast<TRANSFORM_DESC*>(pArg);

	m_fSpeedPerSec = pTransformDesc->fSpeedPerSec;
	m_fRotationPerSec = pTransformDesc->fRotationPerSec;

	return S_OK;
}

_vector CTransform::Get_State(STATE eState) const
{
	return XMLoadFloat4((_float4*)&m_WorldMatrix.m[eState][0]);
}

void CTransform::Set_State(STATE eState, _fvector vVec)
{
	XMStoreFloat4((_float4*)&m_WorldMatrix.m[eState][0], vVec);
	if (eState == STATE_POSITION)
	{
		m_WorldMatrix._44 = 1.f;
	}
}

_float3 CTransform::Get_Scale() const 
{
	_matrix Mat = XMLoadFloat4x4(&m_WorldMatrix);

	return _float3(XMVectorGetX(XMVector3Length(Mat.r[0])), XMVectorGetX(XMVector3Length(Mat.r[1])), XMVectorGetX(XMVector3Length(Mat.r[2])));
}

void CTransform::Set_Scale(const _float3& vScale)
{
	_matrix Mat = XMLoadFloat4x4(&m_WorldMatrix);
	Set_State(STATE_RIGHT ,XMVector3Normalize(Mat.r[0]) * vScale.x);
	Set_State(STATE_UP ,XMVector3Normalize(Mat.r[1]) * vScale.y);
	Set_State(STATE_LOOK ,XMVector3Normalize(Mat.r[2]) * vScale.z);
	
}

void CTransform::Set_Scale(_fvector vScale)
{
	_float3 vfScale;
	XMStoreFloat3(&vfScale, vScale);
	Set_Scale(vfScale);
}


void CTransform::Go_Straight(_float fTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook	  = Get_State(STATE_LOOK);
	vPosition += XMVector3Normalize(vLook) * fTimeDelta * m_fSpeedPerSec;
	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);
	vPosition -= XMVector3Normalize(vLook) * fTimeDelta * m_fSpeedPerSec;
	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vRight = Get_State(STATE_RIGHT);
	vPosition -= XMVector3Normalize(vRight) * fTimeDelta * m_fSpeedPerSec;
	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vRight = Get_State(STATE_RIGHT);
	vPosition += XMVector3Normalize(vRight) * fTimeDelta * m_fSpeedPerSec;
	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Rotation(_fvector vAxis, _float fAngle)
{
	_float3 vScale	= Get_Scale();
	_vector vRight	= XMVectorSet(vScale.x, 0.f, 0.f, 0.f);
	_vector vUp		= XMVectorSet(0.f, vScale.y, 0.f, 0.f);
	_vector vLook	= XMVectorSet(0.f, 0.f, vScale.z, 0.f);

	_matrix RotationMatrix = XMMatrixRotationAxis(vAxis, fAngle);

	Set_State(STATE_RIGHT,	XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(STATE_UP,		XMVector3TransformNormal(vUp,	 RotationMatrix));
	Set_State(STATE_LOOK,	XMVector3TransformNormal(vLook,  RotationMatrix));
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_vector vRight = Get_State(STATE_RIGHT);
	_vector vUp = Get_State(STATE_UP);
	_vector vLook = Get_State(STATE_LOOK);

	_matrix RotationMatrix = XMMatrixRotationAxis(vAxis, fTimeDelta * m_fRotationPerSec);

	Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::LookAt(_fvector vAt)
{
	_float3 vScale = Get_Scale();
	_vector vLook = vAt - Get_State(STATE_POSITION);
	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE_RIGHT,	XMVector3Normalize(vRight) * vScale.x);
	Set_State(STATE_UP,		XMVector3Normalize(vUp) * vScale.y);
	Set_State(STATE_LOOK,	XMVector3Normalize(vLook) * vScale.z);

}

void CTransform::LookAt_Horizontal(_fvector vAt)
{
	_float3 vScale = Get_Scale();
	_vector vLook = vAt - Get_State(STATE_POSITION);
	if (XMVectorGetX(XMVector3Length(vLook)) < 0.0001f)
		return;
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector vRight = XMVector3Cross(vUp, vLook);
	vLook = XMVector3Cross(vRight, vUp);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

HRESULT CTransform::Bind_ShaderResource(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);
}

_float4x4 CTransform::Get_WorldMatrix_Inversed() const
{
	auto Matrix = XMLoadFloat4x4(&m_WorldMatrix);
	Matrix = XMMatrixInverse(nullptr, Matrix);
	_float4x4 MatRet;
	XMStoreFloat4x4(&MatRet, Matrix);
	return MatRet;
}

void CTransform::Set_WorldMatrix(_fmatrix Matrix)
{
	XMStoreFloat4x4(&m_WorldMatrix, Matrix);
}


CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTransform*		pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CTransform::Clone(void * pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
