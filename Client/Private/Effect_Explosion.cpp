#include "Effect_Explosion.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Beat_Manager.h"
#include "Room.h"
#include "RoomManager.h"
#include "Monster_Base.h"
#include "Graphic_Device.h"
_int CEffect_Explosion::iLightIdx = 0;

CEffect_Explosion::CEffect_Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_Explosion::CEffect_Explosion(const CEffect_Explosion& rhs) :
	CEffectObject(rhs)
{
	m_pRoomManager = CRoomManager::Get_Instance();
}

HRESULT CEffect_Explosion::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Explosion", L"Com_Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CEffect_Explosion::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_Explosion::PriorityTick(_float fTimeDelta)
{
}

void CEffect_Explosion::Tick(_float fTimeDelta)
{
	if (m_iLifeTime == 0)
	{
		Dead();
		m_pGameInstance->Remove_PointLight(m_strLightTag);
		return;
	}
	auto pBM = CBeat_Manager::Get_Instance();
	if (m_iPrevBeat != pBM->Get_BeatCount())
	{
		m_iPrevBeat = pBM->Get_BeatCount();
		--m_iLifeTime;
	}

	m_fScale += 4.f * fTimeDelta / pBM->Get_Beat();
	m_pTransformCom->Set_Scale(XMVectorSet(m_fScale, m_fScale, m_fScale, 0.f));
	auto& vecMonsters = m_pRoomManager->Get_Room(m_pRoomManager->Get_CurrentRoomIdx())->Get_Monsters();
	for (auto pMonster : vecMonsters)
	{
		if (pMonster)
		{
			if (XMVectorGetX(XMVector4Length(pMonster->Get_Transform()->Get_Position() - m_pTransformCom->Get_Position())) < m_fScale)
			{
				if (Is_Dead())
					return;

					_float3 vHitPoint;
					XMStoreFloat3(&vHitPoint, m_pTransformCom->Get_Position());
					pMonster->Take_Damage(75.f, vHitPoint, DAMAGE_TYPE::DMG_EXPLODE, this);
				
			}
		}
	}


}

void CEffect_Explosion::LateTick(_float fTimeDelta)
{
	//m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_Explosion::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pShaderCom->Bind_Float("g_fMagicNumber", m_fScale);
	_float4 vColor = s_vAmbientColor;
	vColor.w *= min(((_float)m_iLifeTime - CBeat_Manager::Get_Instance()->Get_Timer() / CBeat_Manager::Get_Instance()->Get_Beat()) / (_float)m_iLifeDuration * 2.f, 1.f);
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &vColor);
	//m_pGameInstance->Bind_RT_SRV(L"RT_BeforeBack", m_pShaderCom, "g_Texture");
	//DEFAULTNOISE->Bind_ShaderResource(m_pShaderCom, "g_Noise_Texture");
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		if (m_pModelCom->Get_Material(i).size())
		{
			auto pTexture = m_pModelCom->Get_Material(i)[0];
			if (pTexture)
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
		}
		m_pShaderCom->Begin((_uint)SHADER_VTXMESH_PASS::Smoke);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_Explosion::Push_From_Pool(void* pArg)
{
	m_pGameInstance->Remove_PointLight(m_strLightTag);
	m_iLifeTime = m_iLifeDuration;
	m_iPrevBeat = CBeat_Manager::Get_Instance()->Get_BeatCount();
	m_fScale = 0.2f;
	m_pGameInstance->Play_Sound_3D(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Fireball_Explosion", m_pTransformCom->Get_Position());
	//m_fLifeTime = 1.0f;
	CPlayer_Manager::Get_Instance()->Get_Player()->Cam_Shake(0.1f, 0.2f);
	m_strLightTag = L"SmokePoint";
	m_strLightTag += to_wstring(iLightIdx);
	++iLightIdx;
	iLightIdx;
	PointLight pLight{};
	pLight.color = { 1.f,0.75f,0.75f };
	pLight.intensity = 1.f;
	_vector vPos = m_pTransformCom->Get_Position();
	XMStoreFloat3(&pLight.position, vPos);
	pLight.range = 8.f;
	pLight.specColor = { 1.f,0.75f,0.75f };
	if (FAILED(m_pGameInstance->Add_Light(m_strLightTag, pLight)))
	{
	}
}

void CEffect_Explosion::Dying()
{

}

CEffect_Explosion* CEffect_Explosion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Explosion* pInstance = new CEffect_Explosion(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_Explosion"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Explosion::Clone(void* pArg)
{
	CEffect_Explosion* pInstance = new CEffect_Explosion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_Explosion"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Explosion::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
