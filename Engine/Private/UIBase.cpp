#include "UIBase.h"
#include "GameInstance.h"
#include "VIBuffer_Rect.h"
#include "Transform.h"
#include "Renderer.h"
#include "Shader.h"

const wstring CUIBase::UILayerTag = L"UI_Layer";
const wstring CUIBase::UITextBoxTag = L"TextBox_Layer";

CUIBase::CUIBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_TYPE eType) :
	CGameObject{ pDevice, pContext },
	m_eType{ eType },
	m_pVIBuffer{ nullptr },
	m_pParentUI{ nullptr }
{
	XMStoreFloat4x4(&m_ParentMatrix, XMMatrixIdentity());
}

CUIBase::CUIBase(const CUIBase& rhs) :
	CGameObject(rhs),
	m_eType{ rhs.m_eType },
	m_ViewMatrix{ rhs.m_ViewMatrix },
	m_ProjMatrix{ rhs.m_ProjMatrix },
	m_ParentMatrix{ rhs.m_ParentMatrix },
	m_fWinSizeX{ rhs.m_fWinSizeX },
	m_fWinSizeY{ rhs.m_fWinSizeY }
{
	m_vColor = _float4{ 1.f,1.f,1.f,1.f };
}

HRESULT CUIBase::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc{};

	__super::Initialize(&desc);

	if (nullptr != Arg)
	{
		UI_DESC* pUIDesc = static_cast<UI_DESC*>(Arg);

		if (pUIDesc->pParentUI != nullptr)
		{
			m_pParentUI = pUIDesc->pParentUI;
		}
		else
		{
			if (m_eType == UI_TYPE::UI_2D)
				pUIDesc->vPosition = { pUIDesc->vPosition.x, pUIDesc->vPosition.y, pUIDesc->vPosition.z };
		}

		if (m_pTransformCom != nullptr)
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat3(&pUIDesc->vPosition));
			m_pTransformCom->Set_Scale(pUIDesc->vSize);
		}

		m_vSize = pUIDesc->vSize;
	}

	return S_OK;
}

HRESULT CUIBase::Initialize_Prototype()
{
	_float2 Resolution = m_pGameInstance->Get_Resolution();
	m_fWinSizeX = Resolution.x;
	m_fWinSizeY = Resolution.y;

	XMStoreFloat4x4(&m_ParentMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(m_fWinSizeX, m_fWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUIBase::PriorityTick(_float fTimeDelta)
{
	for (auto& pChild : m_Children)
	{
		pChild->PriorityTick(fTimeDelta);
	}
}

void CUIBase::Tick(_float fTimeDelta)
{
	for (auto& pChild : m_Children)
	{
		pChild->Tick(fTimeDelta);
	}
}

void CUIBase::LateTick(_float fTimeDelta)
{
	if (m_bIsHidden)
		return;
	_float3 vScaledSize;
	XMStoreFloat3(&vScaledSize, XMLoadFloat3(&m_vSize) * m_fScale);

	m_pTransformCom->Set_Scale(vScaledSize);

	Apply_Matrix_To_Children();

	if (m_eType == UI_TYPE::UI_2D && m_pTransformCom != nullptr)
	{
		MouseOnCheck(Get_Cursor_Pos(m_pGameInstance->Get_hWnd()));
	}
	for (auto& pChild : m_Children)
	{
		pChild->LateTick(fTimeDelta);
	}
}

HRESULT CUIBase::Render()
{
	if (!m_bIsHidden)
	{
		//_float4x4 WorldMatrix = m_pTransformCom->Get_WorldMatrix();

		Bind_Shader_Constant();

		if (m_eType == UI_TYPE::UI_2D)
		{
			//m_pDevice->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
			//m_pDevice->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
		}
		else if (m_eType == UI_TYPE::UI_3D)
		{
			//m_pGameInstance->Set_ViewProjection();
		}
		m_pShaderCom->Begin(m_iRenderPass);
		if (FAILED(m_pVIBuffer->Render()))
			return E_FAIL;
		
		for (auto& pChild : m_Children)
		{
			if(!pChild->m_bIsHidden)
			pChild->Render();
		}

	}

	return S_OK;
}

HRESULT CUIBase::Render(const _float4x4& WorldMatrix, const _float4x4& ViewMatrix, const _float4x4& ProjMatrix)
{
	if (!m_bIsHidden)
	{
		//m_pDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);
		//m_pDevice->SetTransform(D3DTS_VIEW, &ViewMatrix);
		//m_pDevice->SetTransform(D3DTS_PROJECTION, &ProjMatrix);

		m_pVIBuffer->Render();
	}

	return S_OK;
}

HRESULT CUIBase::Add_ChildUI(_uint iLevelIndex, const wstring& strPrototypeTag, CUIBase** ppOut, void* pArg)
{
	CUIBase* pUI = dynamic_cast<CUIBase*>(m_pGameInstance->Clone_Object(strPrototypeTag, pArg));
	if (!pUI)
		throw;
	m_Children.push_back(pUI);

	//auto pChild = dynamic_cast<CUIBase*>(m_pGameInstance->Get_Clone(iLevelIndex, CUIBase::UILayerTag));
	//
	//if (pChild != nullptr)
	//	m_Children.push_back(pChild);
	//else
	//	return E_FAIL;

	if (ppOut != nullptr)
	{
		*ppOut = m_Children.back();
		//Safe_AddRef(m_Children.back());
	}

	return S_OK;
}

void CUIBase::Apply_Matrix_To_Children()
{
	XMStoreFloat4x4(&m_ParentMatrix, XMMatrixIdentity());

	if (m_pParentUI)
	{
		//m_ParentMatrix *= m_pParentUI->Get_Parent_Matrix();
		m_ParentMatrix = Multiply_Parent_Except_Scale();
	}

	for (auto& child : m_Children)
	{
		child->Apply_Matrix_To_Children();
	}
}

void CUIBase::Align_To_Base(_float fBase, ALIGN_DIR eDir)
{
	if (!m_pParentUI)
		return;

	_vector vLocalPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vSize = m_pTransformCom->Get_Scale();
	_float3 vResult;
	XMStoreFloat3(&vResult, vLocalPos);

	switch (eDir)
	{
	case ALIGN_DIR::LEFT:
		vResult = { fBase + vSize.x * 0.5f, vResult.y, vResult.z };
		break;

	case ALIGN_DIR::RIGHT:
		vResult = { fBase - vSize.x * 0.5f, vResult.y, vResult.z };
		break;

	case ALIGN_DIR::UP:
		vResult = { vResult.x, fBase - vSize.y * 0.5f, vResult.z };
		break;

	case ALIGN_DIR::DOWN:
		vResult = { vResult.x, fBase + vSize.y * 0.5f, vResult.z };
		break;
	}
	vLocalPos = XMLoadFloat3(&vResult);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vLocalPos);
}

void CUIBase::Add_To_Renderer()
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
}




_float4x4 CUIBase::Multiply_Parent_Except_Scale() const
{
	// 내가 갖고 있는 Parentmatrix
	// parent한테서 얻어오는 matrix 둘 다 정규화
	// 곱해서 내보냄

	_matrix mMatParent = XMLoadFloat4x4(&m_ParentMatrix);

	//_float3 vRight	= m_ParentMatrix.m[0];
	//_float3 vUp		= m_ParentMatrix.m[1];
	//_float3 vLook	= m_ParentMatrix.m[2];
	//
	//D3DXVec3Normalize(&vRight, &vRight);
	//D3DXVec3Normalize(&vUp, &vUp);
	//D3DXVec3Normalize(&vLook, &vLook);
	//
	//mMatParent._11 = vRight.x;	mMatParent._12 = vRight.y;	mMatParent._13 = vRight.z;
	//mMatParent._21 = vUp.x;		mMatParent._22 = vUp.y;		mMatParent._23 = vUp.z;
	//mMatParent._31 = vLook.x;	mMatParent._32 = vLook.y;	mMatParent._33 = vLook.z;

	_matrix GrandParent = XMLoadFloat4x4(&m_pParentUI->Get_Parent_Matrix());

	_vector vRight = GrandParent.r[0];
	_vector vUp = GrandParent.r[1];
	_vector vLook = GrandParent.r[2];

	vRight = XMVector3Normalize(vRight);
	vUp = XMVector3Normalize(vUp);
	vLook = XMVector3Normalize(vLook);
	GrandParent.r[0] = vRight;
	GrandParent.r[1] = vUp;
	GrandParent.r[2] = vLook;
	_float4x4 matRet;
	XMStoreFloat4x4(&matRet, mMatParent * GrandParent);
	return matRet;
}

_float4x4 CUIBase::Set_BillBoardMatrix(_float4x4 WorldMatrix)
{
	//_float4x4 BBMatrix = {};
	//D3DXMatrixIdentity(&BBMatrix);
	
	//_float4x4 ViewMatrix = static_cast<CTransform*>(m_pGameInstance->Get_Main_Camera()->Get_Component(Transform))->Get_WorldMatrix();

	//BBMatrix._11 = ViewMatrix._11;
	//BBMatrix._13 = ViewMatrix._13;
	//BBMatrix._31 = ViewMatrix._31;
	//BBMatrix._33 = ViewMatrix._33;
	
	//_float4x4 ResultMatrix = WorldMatrix * BBMatrix;
	
	//ResultMatrix._41 = WorldMatrix._41;
	//ResultMatrix._42 = WorldMatrix._42;
	//ResultMatrix._43 = WorldMatrix._43;
	//
	//m_pDevice->SetTransform(D3DTS_WORLD, &ResultMatrix);

	//return ResultMatrix;
	return _float4x4();
}

void CUIBase::Bind_Shader_Constant()
{
	_float4x4 WorldMatrix = m_pTransformCom->Get_WorldMatrix();

	if (m_pParentUI)
	{
		WorldMatrix.m[3][0] += m_ParentMatrix.m[3][0];
		WorldMatrix.m[3][1] += m_ParentMatrix.m[3][1];
		WorldMatrix.m[3][2] += m_ParentMatrix.m[3][2];
	}
	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &WorldMatrix);
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);
	m_pShaderCom->Bind_Vector4("g_Color", &m_vColor);
}

_float3 CUIBase::Get_GlobalPos() const
{
	_float4x4 WorldMatrix = m_pTransformCom->Get_WorldMatrix();

	if (m_pParentUI)
	{
		WorldMatrix.m[3][0] += m_ParentMatrix.m[3][0];
		WorldMatrix.m[3][1] += m_ParentMatrix.m[3][1];
		WorldMatrix.m[3][2] += m_ParentMatrix.m[3][2];
	}

	return _float3{ WorldMatrix.m[3] };
}

_float4x4 CUIBase::Get_Parent_Matrix() const
{
	//return m_ParentMatrix * m_pTransform->Get_WorldMatrix();

	_float4x4 WorldMatrix = m_pTransformCom->Get_WorldMatrix();

	if (m_pParentUI)
	{
		WorldMatrix.m[3][0] += m_ParentMatrix.m[3][0];
		WorldMatrix.m[3][1] += m_ParentMatrix.m[3][1];
		WorldMatrix.m[3][2] += m_ParentMatrix.m[3][2];
	}

	return WorldMatrix;
}

_float3 CUIBase::Get_Size() const
{
	return m_pTransformCom->Get_Scale();
}

void CUIBase::MouseOnCheck(_float3 vMousePos)
{
	_float3 vPos;
	XMStoreFloat3(&vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (m_pParentUI)
	{
		vPos.x += m_ParentMatrix.m[3][0];
		vPos.y += m_ParentMatrix.m[3][1];
		vPos.z += m_ParentMatrix.m[3][2];
	}

	if (vMousePos.x >= vPos.x - m_vSize.x * .5f && vMousePos.x <= vPos.x + m_vSize.x * .5f
		&& vMousePos.y >= vPos.y - m_vSize.y * .5f && vMousePos.y <= vPos.y + m_vSize.x * .5f)
	{
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}

}

void CUIBase::Set_Size(const _float3& vSize)
{
	m_vSize = vSize;
}

void CUIBase::Set_Hidden(_bool bIsHidden, _bool bChild)
{
	m_bIsHidden = bIsHidden;
	if(bChild)
	for (auto& Child : m_Children)
		Child->Set_Hidden(m_bIsHidden);
}

void CUIBase::Free()
{
	__super::Free();

	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTransformCom);
	//Safe_Release(m_pTexture);
	Safe_Release(m_pShaderCom);
	if (m_Children.size() != 0)
	{
		for (auto& iter : m_Children)
		{
			if (iter)
				Safe_Release(iter);
		}
	}

	m_Children.clear();
}

