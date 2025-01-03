#include "Effect_Equipping.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Effect_Pheonix.h"

CEffect_Equipping::CEffect_Equipping(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_Equipping::CEffect_Equipping(const CEffect_Equipping& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_Equipping::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_RectNorm", L"Com_VIBuffer", (CComponent**)&m_pVICom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_VenusTrail", L"Com_Texture", (CComponent**)&m_pDiffuseTextureCom);

	m_pTransformCom->Set_Scale(_float3{ 1.5f,1.5f,1.f });
	return S_OK;
}

HRESULT CEffect_Equipping::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_Equipping::PriorityTick(_float fTimeDelta)
{
}

void CEffect_Equipping::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	m_pTransformCom->LookAt_Horizontal(m_pGameInstance->Get_CamPosition_Vector());
	m_pTransformCom->Set_Scale(XMVectorSet(1.5f, 20.f * (1.f - m_fLifeTime / m_fLifeDuration), 1.f, 0.f));
	if (m_fLifeTime < 0.f)
	{
		Dead();
	}
}

void CEffect_Equipping::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_Equipping::Render()
{
	//m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_TransformMat);
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pDiffuseTextureCom->Bind_ShaderResources(m_pShaderCom, "g_Texture");
	_float4 vColor = { 1.f,1.f,1.f,1.f };
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &vColor);
	m_pShaderCom->Bind_Float("g_fMagicNumber", (m_fLifeDuration - m_fLifeTime) / (m_fLifeDuration));
	m_pShaderCom->Begin((_uint)SHADER_VTXNOR_PASS::SmokeTrail);
	m_pVICom->Render();

	return S_OK;
}

void CEffect_Equipping::Push_From_Pool(void* pArg)
{
	m_fLifeTime = m_fLifeDuration;
}

void CEffect_Equipping::Dying()
{
}

CEffect_Equipping* CEffect_Equipping::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Equipping* pInstance = new CEffect_Equipping(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_Equipping"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Equipping::Clone(void* pArg)
{
	CEffect_Equipping* pInstance = new CEffect_Equipping(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_Equipping"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Equipping::Free()
{
	__super::Free();
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pVICom);
	Safe_Release(m_pShaderCom);
}
