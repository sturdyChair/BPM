#include "Effect_NidhoggField.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Beat_Manager.h"

CEffect_NidhoggField::CEffect_NidhoggField(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_NidhoggField::CEffect_NidhoggField(const CEffect_NidhoggField& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_NidhoggField::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_NidhoggField", L"Com_Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CEffect_NidhoggField::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_NidhoggField::PriorityTick(_float fTimeDelta)
{
}

void CEffect_NidhoggField::Tick(_float fTimeDelta)
{
}

void CEffect_NidhoggField::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND, this);
}

HRESULT CEffect_NidhoggField::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pShaderCom->Bind_Vector4("g_vColor", &m_vColor);
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		if (m_pModelCom->Get_Material(i).size())
		{
			auto pTexture = m_pModelCom->Get_Material(i)[0];
			if (pTexture)
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
		}
		m_pShaderCom->Begin((_uint)SHADER_VTXMESH_PASS::Color);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_NidhoggField::Push_From_Pool(void* pArg)
{
}

void CEffect_NidhoggField::Dying()
{
}

CEffect_NidhoggField* CEffect_NidhoggField::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_NidhoggField* pInstance = new CEffect_NidhoggField(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_NidhoggField"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_NidhoggField::Clone(void* pArg)
{
	CEffect_NidhoggField* pInstance = new CEffect_NidhoggField(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_NidhoggField"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_NidhoggField::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
