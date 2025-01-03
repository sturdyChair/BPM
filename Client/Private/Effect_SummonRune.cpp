#include "Effect_SummonRune.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Effect_Pheonix.h"

CEffect_SummonRune::CEffect_SummonRune(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_SummonRune::CEffect_SummonRune(const CEffect_SummonRune& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_SummonRune::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_RectNorm", L"Com_VIBuffer", (CComponent**)&m_pVICom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_SummonRune", L"Com_Texture", (CComponent**)&m_pDiffuseTextureCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Corona", L"Com_EmissionTexture", (CComponent**)&m_pMaskTextureCom);
	m_pMaskTextureCom->Set_Speed(4.f);
	m_pTransformCom->Set_Scale(_float3{ 1.5f,1.5f,1.f });
	return S_OK;
}

HRESULT CEffect_SummonRune::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_SummonRune::PriorityTick(_float fTimeDelta)
{
}

void CEffect_SummonRune::Tick(_float fTimeDelta)
{

	m_fLifeTime -= fTimeDelta;
	m_pTransformCom->LookAt(m_pGameInstance->Get_CamPosition_Vector());
	if (m_fLifeTime < 0.f)
	{

		CEffect_Pheonix::PROJECTILE_DESC PDESC;
		PDESC.eCG = CG_MONSTER_PROJECTILE;
		_vector vDir = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
		XMStoreFloat3(&PDESC.vDirection, vDir * 30.f);

		m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Pheonix", m_pGameInstance->Get_Current_LevelID(),L"Effect", &PDESC, m_pTransformCom->Get_WorldMatrix_Ptr());
		Dead();
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Projectile");
	}
}

void CEffect_SummonRune::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_SummonRune::Render()
{
	//m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_TransformMat);
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pMaskTextureCom->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture");
	m_pDiffuseTextureCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture");
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	m_pShaderCom->Bind_Float("g_fMagicNumber", min((m_fLifeDuration - m_fLifeTime) / (m_fLifeDuration), 1.f));
	m_pShaderCom->Begin((_uint)SHADER_VTXNOR_PASS::Rune);
	m_pVICom->Render();

	return S_OK;
}

void CEffect_SummonRune::Push_From_Pool(void* pArg)
{
	CHARGE_DESC* pDesc = (CHARGE_DESC*)pArg;
	m_fLifeTime = pDesc->fLifeTime;
	m_fLifeDuration = m_fLifeTime;
}

void CEffect_SummonRune::Dying()
{
}

CEffect_SummonRune* CEffect_SummonRune::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_SummonRune* pInstance = new CEffect_SummonRune(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_SummonRune"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_SummonRune::Clone(void* pArg)
{
	CEffect_SummonRune* pInstance = new CEffect_SummonRune(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_SummonRune"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_SummonRune::Free()
{
	__super::Free();
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pMaskTextureCom);
	Safe_Release(m_pVICom);
	Safe_Release(m_pShaderCom);
}
