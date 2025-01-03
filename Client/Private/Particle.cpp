#include "Particle.h"
#include "VIBuffer_Instance.h"
#include "GameInstance.h"

CParticle::CParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CParticle::CParticle(const CParticle& rhs) :
	CGameObject(rhs), m_tDesc(rhs.m_tDesc),m_fLifeTime(rhs.m_fLifeTime), m_iRenderPass(rhs.m_iRenderPass)
{
}

void CParticle::PriorityTick(_float fTimeDelta)
{
}

void CParticle::Tick(_float fTimeDelta)
{
	switch (m_tDesc.eMovementType)
	{
	case PARTICLE_MOVEMENT::SPREAD:
	{
		m_pVBCom->Update_Spread(fTimeDelta);
	}
	break;
	case PARTICLE_MOVEMENT::DROP:
	{
		m_pVBCom->Update_Drop(fTimeDelta);
	}
	break;
	case PARTICLE_MOVEMENT::FOUNTAIN:
	{
		m_pVBCom->Update_Fountain(fTimeDelta);
	}
	break;
	case PARTICLE_MOVEMENT::RISING:
	{
		m_pVBCom->Update_Rising(fTimeDelta);
	}
	break;
	case PARTICLE_MOVEMENT::CYCLE:
	{
		m_pVBCom->Update_Cycle(fTimeDelta);
	}
	break;
	case PARTICLE_MOVEMENT::ORBITAL:
	{
		m_pVBCom->Update_Orbital(fTimeDelta);
	}
	break;
	default:
		break;
	}
	if (m_tDesc.bTextureAnim)
	{
		m_pTextureCom->MoveFrame(fTimeDelta);
	}
}

void CParticle::LateTick(_float fTimeDelta)
{
	if(!m_tDesc.bInfinite)
		m_fLifeTime -= fTimeDelta;
	if (m_fLifeTime <= 0.f)
	{
		m_fLifeTime = 0.f;
		Dead();
	}
	else
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CParticle::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Float("g_fMagicNumber", m_fLifeTime)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_pTextureCom->Get_Frame())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(m_iRenderPass)))
		return E_FAIL;

	if (FAILED(m_pVBCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CParticle::Push_From_Pool(void* pArg)
{
	if (m_tDesc.fMaxLifeTime <= 0.f)
		m_fLifeTime = max(m_tDesc.vLifeTime.x, m_tDesc.vLifeTime.y);
	else
		m_fLifeTime = m_tDesc.fMaxLifeTime;
}

void CParticle::Dying()
{
	m_pVBCom->Reset();
}

HRESULT CParticle::Initialize_Prototype(const PARTICLE_DESC& desc)
{
	m_tDesc = desc;
	
	return S_OK;
}

HRESULT CParticle::Initialize(void* Arg)
{
	__super::Initialize(Arg);

	Add_Component(LEVEL_STATIC, m_tDesc.strTextureTag, L"Com_Texture", (CComponent**)&m_pTextureCom);
	if (m_tDesc.fMaxLifeTime <= 0.f)
		m_fLifeTime = max(m_tDesc.vLifeTime.x, m_tDesc.vLifeTime.y);
	else
		m_fLifeTime = m_tDesc.fMaxLifeTime;
	return S_OK;
}

void CParticle::Free()
{
	__super::Free();
	Safe_Release(m_pVBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
