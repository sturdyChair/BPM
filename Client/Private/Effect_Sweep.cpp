#include "Effect_Sweep.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"

CEffect_Sweep::CEffect_Sweep(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_Sweep::CEffect_Sweep(const CEffect_Sweep& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_Sweep::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Sweep", L"Com_Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CEffect_Sweep::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_Sweep::PriorityTick(_float fTimeDelta)
{
}

void CEffect_Sweep::Tick(_float fTimeDelta)
{

	m_fLifeTime -= fTimeDelta;
	if (m_fLifeTime < 0.f)
	{
		Dead();
	}
}

void CEffect_Sweep::LateTick(_float fTimeDelta)
{
	if (m_pParentMat)
	{
		_matrix Parent = XMLoadFloat4x4(m_pParentMat);
		Parent.r[0] = XMVector3Normalize(Parent.r[0]);
		Parent.r[1] = XMVector3Normalize(Parent.r[1]);
		Parent.r[2] = XMVector3Normalize(Parent.r[2]);
		_matrix World = XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * Parent;
		XMStoreFloat4x4(&m_TransformMat, World);
	}
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_Sweep::Render()
{
	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_TransformMat);
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
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
		m_pShaderCom->Begin((_uint)SHADER_VTXMESH_PASS::EffectSweep);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_Sweep::Push_From_Pool(void* pArg)
{
	SWEEP_DESC* pDesc = (SWEEP_DESC*)pArg;
	m_pParentMat = pDesc->pParent;
	m_fLifeTime = pDesc->fLifeTime;
	m_fLifeDuration = m_fLifeTime;
	//m_fLifeTime = 1.0f;
}

void CEffect_Sweep::Dying()
{
	m_pParentMat = nullptr;
}

CEffect_Sweep* CEffect_Sweep::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Sweep* pInstance = new CEffect_Sweep(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_Sweep"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Sweep::Clone(void* pArg)
{
	CEffect_Sweep* pInstance = new CEffect_Sweep(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_Sweep"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Sweep::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
