
#include "Effect_Wall.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Player_Manager.h"
#include "Player.h"

CEffect_Wall::CEffect_Wall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_Wall::CEffect_Wall(const CEffect_Wall& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_Wall::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_RectNorm", L"Com_VIBuffer", (CComponent**)&m_pVICom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_ThrillWall", L"Com_Texture", (CComponent**)&m_pDiffuseTextureCom);

	return S_OK;
}

HRESULT CEffect_Wall::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_Wall::PriorityTick(_float fTimeDelta)
{
}

void CEffect_Wall::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	if (m_fLifeTime < 0.f)
	{
		Dead();
	}
	else if(m_fLifeTime <= 0.1f)
	{
		_matrix World = XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix());
		_vector vPlane = XMPlaneFromPointNormal(World.r[3], World.r[2]);
		_vector vPlayer = CPlayer_Manager::Get_Instance()->Get_Player()->Get_Transform()->Get_Position();
		_float3 vfPlayer;
		XMStoreFloat3(&vfPlayer, vPlayer);
		if (XMVectorGetX(XMPlaneDotCoord(vPlane, vPlayer)) < 1.f)
		{
			CPlayer_Manager::Get_Instance()->Get_Player()->Take_Damage(25.f, vfPlayer, DMG_EVADABLE, this);
		}
	}
}

void CEffect_Wall::LateTick(_float fTimeDelta)
{
	
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_Wall::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	
	m_pDiffuseTextureCom->Bind_ShaderResources(m_pShaderCom, "g_Texture");
	m_pShaderCom->Bind_Float("g_fMagicNumber", m_fLifeTime * 10.f);
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	m_pShaderCom->Begin((_uint)SHADER_VTXNOR_PASS::Wall);
	m_pVICom->Render();

	return S_OK;
}

void CEffect_Wall::Push_From_Pool(void* pArg)
{
	m_fLifeTime = 0.2f;
}

void CEffect_Wall::Dying()
{
}

CEffect_Wall* CEffect_Wall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Wall* pInstance = new CEffect_Wall(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_Wall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Wall::Clone(void* pArg)
{
	CEffect_Wall* pInstance = new CEffect_Wall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_Wall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Wall::Free()
{
	__super::Free();
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pVICom);
	Safe_Release(m_pShaderCom);
}
