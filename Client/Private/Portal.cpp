#include "Portal.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Model.h"
#include "Beat_Manager.h"

CPortal::CPortal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CPortal::CPortal(const CPortal& rhs) :
	CGameObject(rhs)
{
}

HRESULT CPortal::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CPortal::Initialize(void* pArg)
{

	__super::Initialize(nullptr);
	if (pArg)
	{
		PORTAL_DESC* pDesc = (PORTAL_DESC*)pArg;
		m_pTransformCom->Set_WorldMatrix(pDesc->Transform);
		m_strModelTag = pDesc->strModelTag;
		XMStoreFloat4(&m_vDoorOffset, m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
		m_vDoorOffset.w = 0.f;
	}

	Ready_Components();

	return S_OK;
}

void CPortal::PriorityTick(_float fTimeDelta)
{
}

void CPortal::Tick(_float fTimeDelta)
{
	if (m_bCleared)
	{
		m_fOpenAngle += fTimeDelta * XM_PI;
		if (m_fOpenAngle > XM_PIDIV2)
		{
			m_fOpenAngle = XM_PIDIV2;
		}
	}

	m_DoorTransformation = m_pTransformCom->Get_WorldMatrix();
	
	_matrix matTemp = XMMatrixTranslation(1.4f, 0.f, 0.f);

	XMStoreFloat4x4(&m_DoorTransformation, matTemp * XMLoadFloat4x4(&m_DoorTransformation) * XMMatrixRotationRollPitchYaw(0.f, m_fOpenAngle * 1.f, 0.f) );
	
	matTemp = XMMatrixTranslation(-2.8f, 0.f, 0.f);
	XMStoreFloat4x4(&m_DoorMirrorTransformation, matTemp * XMLoadFloat4x4(&m_DoorTransformation) * XMMatrixRotationRollPitchYaw(0.f, m_fOpenAngle * 1.f, 0.f) );

}

void CPortal::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CPortal::Render()
{
	//Bind_ShaderResources();
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));

	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_DoorTransformation);

	m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		auto& vecMat = m_pModelCom->Get_Material(i);
		if (vecMat.size())
		{
			auto pTexture = vecMat[0];
			if (pTexture)
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
			pTexture = vecMat[5];
			if (pTexture)
			{
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
			}
			else
			{
				DEFAULTNORMAL->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
			}
			pTexture = vecMat[14];
			if (pTexture)
			{
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
			}
			else
			{
				DEFAULTORM->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
			}
		}
		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}
	
	//m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");

	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_DoorMirrorTransformation);
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		if (m_pModelCom->Get_Material(i).size())
		{
			auto& vecMat = m_pModelCom->Get_Material(i);
			if (vecMat.size())
			{
				auto pTexture = vecMat[0];
				if (pTexture)
					pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
				pTexture = vecMat[5];
				if (pTexture)
				{
					pTexture->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
				}
				else
				{
					DEFAULTNORMAL->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
				}
				pTexture = vecMat[14];
				if (pTexture)
				{
					pTexture->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
				}
				else
				{
					DEFAULTORM->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
				}
			}
		}
		m_pShaderCom->Begin((_uint)SHADER_VTXMESH_PASS::Mirror_X);
		m_pModelCom->Render(i);
	}



	return S_OK;
}

void CPortal::Dying()
{

}

void CPortal::Push_From_Pool(void* pArg)
{

}

void CPortal::Open()
{
	m_bCleared = true;
}

HRESULT CPortal::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, m_strModelTag, L"Model", (CComponent**)&m_pModelCom);


	return S_OK;
}

HRESULT CPortal::Bind_ShaderResources()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Begin(0);

	return S_OK;
}

CPortal* CPortal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPortal* pInstance = new CPortal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CPortal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPortal::Clone(void* pArg)
{
	CPortal* pInstance = new CPortal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CPortal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPortal::Free()
{

	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
