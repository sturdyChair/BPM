#include "PipeLine.h"
#include "Camera.h"
CPipeLine::CPipeLine()
{
}

void CPipeLine::Update()
{
	if (m_pMainCamera)
	{
		m_pMainCamera->Set_Transform();
	}

	_matrix ViewInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformationMatrices[TS_VIEW]));
	XMStoreFloat4x4(&m_TransformationMatrices[TS_VIEW_INV], ViewInv);
	XMStoreFloat4x4(&m_TransformationMatrices[TS_PROJ_INV], XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformationMatrices[TS_PROJ])));
	XMStoreFloat4(&m_vCamPosition, ViewInv.r[3]);
}

void CPipeLine::Set_Transform(PIPELINE_STATE eState, _fmatrix Matrix)
{
	XMStoreFloat4x4(&m_TransformationMatrices[eState], Matrix);
}

void CPipeLine::Set_Transform(PIPELINE_STATE eState, const _float4x4& Matrix)
{
	m_TransformationMatrices[eState] = Matrix;
}

_fmatrix CPipeLine::Get_Transform_Matrix(PIPELINE_STATE eState) const
{
	return XMLoadFloat4x4(&m_TransformationMatrices[eState]);
}

_float4x4 CPipeLine::Get_Transform_Float4x4(PIPELINE_STATE eState) const
{
	return m_TransformationMatrices[eState];
}

_float4 CPipeLine::Get_CamPosition_Float4() const
{
	return m_vCamPosition;
}

_fvector CPipeLine::Get_CamPosition_Vector() const
{
	return XMLoadFloat4(&m_vCamPosition);
}

void CPipeLine::Add_Camera(const _wstring& strTag, CCamera* pCamera)
{
	if (m_Cameras.count(strTag))
		return;
	m_Cameras[strTag] = pCamera;
	Safe_AddRef(pCamera);
}

CCamera* CPipeLine::Get_Camera(const _wstring& strTag)
{
	if(!m_Cameras.count(strTag))
		return nullptr;
	return m_Cameras[strTag];
}

CCamera* CPipeLine::Get_MainCamera()
{
	return m_pMainCamera;
}

void CPipeLine::Set_MainCamera(const _wstring& strTag)
{
	m_pMainCamera = Get_Camera(strTag);
	if (m_pMainCamera)
	{
		m_strMainCamTag = strTag;
	}
	else
	{
		m_strMainCamTag = L"";
	}
}

void CPipeLine::Remove_Camera(const _wstring& strTag)
{
	if (!m_Cameras.count(strTag))
		return;
	if (strTag == m_strMainCamTag)
	{
		m_pMainCamera = nullptr;
	}
	Safe_Release(m_Cameras[strTag]);
	m_Cameras.erase(strTag);
}

void CPipeLine::Clear_Camera()
{
	for (auto& pCam : m_Cameras)
	{
		Safe_Release(pCam.second);
	}
	m_pMainCamera = nullptr;
	m_Cameras.clear();
}


CPipeLine* CPipeLine::Create()
{
	CPipeLine* pInstance = new CPipeLine;

	return pInstance;
}

void CPipeLine::Free()
{
	Clear_Camera();
}
