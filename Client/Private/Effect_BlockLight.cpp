#include "Effect_BlockLight.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Player_Manager.h"
#include "Player.h"


CEffect_BlockLight::CEffect_BlockLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_BlockLight::CEffect_BlockLight(const CEffect_BlockLight& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_BlockLight::Initialize(void* Arg)
{
	__super::Initialize(nullptr);
	_tchar* pTexTag = (_tchar*)Arg;
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVICom);
	Add_Component(LEVEL_STATIC, pTexTag, L"Com_Texture", (CComponent**)&m_pTextureCom);

	return S_OK;
}

HRESULT CEffect_BlockLight::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_BlockLight::PriorityTick(_float fTimeDelta)
{
}

void CEffect_BlockLight::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime > m_fLifeTime && m_fTime < m_fLifeTime * 1.1f)
	{
		auto pPlayer = CPlayer_Manager::Get_Instance()->Get_Player();
		_vector vTargetPos = pPlayer->Get_Transform()->Get_Position();
		_vector vOri = XMLoadFloat4(&m_vOrigin);
		_vector vDist = vTargetPos - XMVectorSet(-10000.f, -10000.f, 0.f, 0.f) * m_fBlockSize - vOri;
		_float ipX, ipZ;
		_float fX = modf(XMVectorGetX(vDist) / m_fBlockSize, &ipX);
		_float fZ = modf(XMVectorGetZ(vDist) / m_fBlockSize, &ipZ);
		
		_int iipX = ipX;
		iipX = iipX % 2;
		_int iipZ = ipZ;
		iipZ = iipZ % 2;
		if ( ((abs(iipX) == abs(iipZ)) && m_bOdd) || ((abs(iipX) != abs(iipZ)) && !m_bOdd))
		{
			pPlayer->Take_Damage(25.f, _float3{0.f,0.f,0.f}, DMG_EXPLODE, this);
		}
	}

	if (m_fTime >= m_fLifeTime * 1.2f)
	{
		Dead();
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"ThunderClap");
	}
}

void CEffect_BlockLight::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_DISTORTION, this);
}

HRESULT CEffect_BlockLight::Render()
{
	m_pGameInstance->Bind_RT_SRV(L"RT_Position", m_pShaderCom, "g_PositionTexture");
	_float fIntensity = m_fTime / m_fLifeTime;
	if (m_fTime > m_fLifeTime)
		fIntensity = (m_fLifeTime * 1.2f - m_fTime) / (0.2f * m_fLifeTime);
	m_pShaderCom->Bind_Float("g_Dissolve", 1.f - fIntensity);
	_float4 vColor;
	XMStoreFloat4(&vColor, XMLoadFloat4(&m_vColor) * fIntensity * 2.f);
	m_pShaderCom->Bind_Vector4("g_Color", &vColor);
	m_pShaderCom->Bind_Vector4("BlockOrigin", &m_vOrigin);
	m_pShaderCom->Bind_Float("BlockSize", m_fBlockSize);
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture");
	DEFAULTNOISE->Bind_ShaderResource(m_pShaderCom, "g_Noise_Texture");

	if(!m_bOdd)
		m_pShaderCom->Begin((_uint)SHADER_VTXPOS_PASS::BlockLight);
	else
		m_pShaderCom->Begin((_uint)SHADER_VTXPOS_PASS::BlockLightEven);
	m_pVICom->Render();

	return S_OK;
}

void CEffect_BlockLight::Push_From_Pool(void* pArg)
{
	BLOCK_DESC* pDesc = (BLOCK_DESC*)pArg;
	m_fLifeTime = pDesc->fLifeTime;
	m_fBlockSize = pDesc->fBlockSize;
	m_vColor = pDesc->vColor;
	m_vOrigin = pDesc->vOrigin;
	m_bOdd = pDesc->bOdd;
	m_fTime = 0.f;
}

void CEffect_BlockLight::Dying()
{
}

CEffect_BlockLight* CEffect_BlockLight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_BlockLight* pInstance = new CEffect_BlockLight(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_BlockLight"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_BlockLight::Clone(void* pArg)
{
	CEffect_BlockLight* pInstance = new CEffect_BlockLight(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_BlockLight"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_BlockLight::Free()
{
	__super::Free();

	Safe_Release(m_pVICom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
