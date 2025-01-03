#include "Effect_DiscImpact.h"
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

CEffect_DiscImpact::CEffect_DiscImpact(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_DiscImpact::CEffect_DiscImpact(const CEffect_DiscImpact& rhs) :
	CEffectObject(rhs)
{
	m_pRoomManager = CRoomManager::Get_Instance();
}

HRESULT CEffect_DiscImpact::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_DiscImpact", L"Com_Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CEffect_DiscImpact::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_DiscImpact::PriorityTick(_float fTimeDelta)
{
}

void CEffect_DiscImpact::Tick(_float fTimeDelta)
{
	if (m_iLifeTime == 0)
	{
		Dead();
		return;
	}
	auto pBM = CBeat_Manager::Get_Instance();
	if (m_iPrevBeat != pBM->Get_BeatCount())
	{
		m_iPrevBeat = pBM->Get_BeatCount();
		--m_iLifeTime;
	}

	m_fScale += 2.f * fTimeDelta / pBM->Get_Beat();
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
				pMonster->Take_Damage(25.f, vHitPoint, DAMAGE_TYPE::DMG_EXPLODE, this);

			}
		}
	}


}

void CEffect_DiscImpact::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_DISTORTION, this);
}

HRESULT CEffect_DiscImpact::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pShaderCom->Bind_Float("g_fMagicNumber", m_fScale * XM_PI);
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	m_pGameInstance->Bind_RT_SRV(L"RT_BeforeBack", m_pShaderCom, "g_Texture");
	_float2 vResolution = { (_float)g_iWinSizeX, (_float)g_iWinSizeY };
	m_pShaderCom->Bind_RawValue("g_vResolution", &vResolution, sizeof(_float2));
	m_pShaderCom->Bind_Float("g_fNoiseStrength", ((_float)m_iLifeTime) / ((_float)m_iLifeDuration) * 0.5f);
	DEFAULTNOISE->Bind_ShaderResource(m_pShaderCom, "g_Noise_Texture");
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		if (m_pModelCom->Get_Material(i).size())
		{
			//auto& vecMat = m_pModelCom->Get_Material(i);
			//if (vecMat.size())
			//{
			//	//auto pTexture = vecMat[0];
			//	//if (pTexture)
			//	//	pTexture->Bind_ShaderResource(m_pShaderCom, "g_Noise_Texture");
			//	//pTexture = vecMat[5];
			//	//if (pTexture)
			//	//{
			//	//	pTexture->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
			//	//}
			//	//else
			//	//{
			//	//	DEFAULTNORMAL->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
			//	//}
			//	//pTexture = vecMat[14];
			//	//if (pTexture)
			//	//{
			//	//	pTexture->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
			//	//}
			//	//else
			//	//{
			//	//	DEFAULTORM->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
			//	//}
			//}
		}
		m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
		m_pShaderCom->Begin((_uint)SHADER_VTXMESH_PASS::DistortionVertex);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_DiscImpact::Push_From_Pool(void* pArg)
{
	m_iLifeTime = m_iLifeDuration;
	m_iPrevBeat = CBeat_Manager::Get_Instance()->Get_BeatCount();
	m_fScale = 0.2f;
	m_pGameInstance->Play_Sound_3D(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Fireball_Explosion", m_pTransformCom->Get_Position());
	//m_fLifeTime = 1.0f;
}

void CEffect_DiscImpact::Dying()
{
}

CEffect_DiscImpact* CEffect_DiscImpact::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_DiscImpact* pInstance = new CEffect_DiscImpact(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_DiscImpact"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_DiscImpact::Clone(void* pArg)
{
	CEffect_DiscImpact* pInstance = new CEffect_DiscImpact(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_DiscImpact"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_DiscImpact::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
