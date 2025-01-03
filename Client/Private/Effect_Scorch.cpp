#include "Effect_Scorch.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"

CEffect_Scorch::CEffect_Scorch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_Scorch::CEffect_Scorch(const CEffect_Scorch& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_Scorch::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_RectNorm", L"Com_VIBuffer", (CComponent**)&m_pVICom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Scorch", L"Com_Texture", (CComponent**)&m_pDiffuseTextureCom);

	return S_OK;
}

HRESULT CEffect_Scorch::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_Scorch::PriorityTick(_float fTimeDelta)
{
}

void CEffect_Scorch::Tick(_float fTimeDelta)
{

	m_fLifeTime -= fTimeDelta;
	if (m_fLifeTime < 0.f)
	{
		Dead();
	}
}

void CEffect_Scorch::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_Scorch::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pShaderCom->Bind_Float("g_fMagicNumber", m_fLifeTime);
	m_pDiffuseTextureCom->Bind_ShaderResources(m_pShaderCom, "g_Texture");
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	m_pShaderCom->Begin((_uint)SHADER_VTXNOR_PASS::MaskToDiffuse);
	m_pVICom->Render();

	return S_OK;
}

void CEffect_Scorch::Push_From_Pool(void* pArg)
{
	m_fLifeTime = 1.0f;
	//_matrix Parent = XMLoadFloat4x4(m_pParentMat);
	//_matrix ViewInv = m_pGameInstance->Get_Transform_Matrix(CPipeLine::TS_VIEW_INV);
	//ViewInv.r[3] = Parent.r[3];
	//_float4x4 SparkMat;

	//XMStoreFloat4x4(&SparkMat, ViewInv);
	//
	//m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Particle_GlowStar_Spark", m_pGameInstance->Get_Current_LevelID(),
	//	L"Effect", nullptr, &SparkMat);
}

void CEffect_Scorch::Dying()
{
}

CEffect_Scorch* CEffect_Scorch::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Scorch* pInstance = new CEffect_Scorch(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_Scorch"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Scorch::Clone(void* pArg)
{
	CEffect_Scorch* pInstance = new CEffect_Scorch(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_Scorch"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Scorch::Free()
{
	__super::Free();
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pVICom);
	Safe_Release(m_pShaderCom);
}
