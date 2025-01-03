#include "Effect_Ally.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Rigidbody.h"
#include "Player_Manager.h"
#include "Player.h"
#include "RoomManager.h"
#include "Room.h"
#include "Monster_Base.h"
#include "Beat_Manager.h"
#include "AfterImage.h"
#include "Effect_AllyProjectile.h"

CEffect_Ally::CEffect_Ally(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_Ally::CEffect_Ally(const CEffect_Ally& rhs) :
	CEffectObject(rhs)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
	m_pBeatManager = CBeat_Manager::Get_Instance();
}

HRESULT CEffect_Ally::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_ModelShader", (CComponent**)&m_pShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Ally", L"Com_Model", (CComponent**)&m_pModelCom);
	m_pAfterImage = (CAfterImage*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_AfterImage_Anim_1sec_10", m_pModelCom);
	m_pAfterImage->Set_Color(_float4{ 0.5f,0.8f,1.f,1.f });
	//
	return S_OK;
}

HRESULT CEffect_Ally::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_Ally::PriorityTick(_float fTimeDelta)
{
	m_iPrevBeat = m_iCurrBeat;
	m_iCurrBeat = m_pBeatManager->Get_BeatCount();
	m_pAfterImage->PriorityTick(fTimeDelta);
}

void CEffect_Ally::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	m_pModelCom->Play_Animation(fTimeDelta, m_fAnimFrom, m_fAnimTo);
	_float fART = m_pModelCom->Get_Current_Animation_TrackRatio();
	_vector vPos = m_pTransformCom->Get_Position();
	_float fCurY = XMVectorGetY(vPos);
	_float fDistY = fCurY - m_vStartPos.y;
	if (fDistY <= 5.f)
	{
		m_pTransformCom->Set_Position(XMVectorSetY(vPos, fCurY + 5.f * fTimeDelta));
	}

	if(m_eState == START)
	{
		if (m_pModelCom->Get_Finished())
		{
			m_fAnimFrom = 6.f / 13.f;
			m_pModelCom->Set_AnimTag("MurielSepia.ao|Ultimate_Prepare", false);
		}

		if(m_iPrevBeat != m_iCurrBeat)
		{
			auto pRM = CRoomManager::Get_Instance();
			auto& vecMon = pRM->Get_Room(pRM->Get_CurrentRoomIdx())->Get_Monsters();
			for (auto& pMon : vecMon)
			{
				if (pMon)
				{
					auto vTargetPos = pMon->Get_Transform()->Get_Position();
					auto vStartPos = vPos + XMVectorSet(0.f,1.f,0.f,0.f);
					//
					CEffect_AllyProjectile::PROJECTILE_DESC pDesc;
					pDesc.eCG = CG_PLAYER_PROJECTILE;
					XMStoreFloat3(&pDesc.vDirection, XMVector3Normalize(vTargetPos - vStartPos) * 70.f);
					_float4x4 Mat;
					_matrix Temp = XMMatrixTranslationFromVector(vStartPos);
					XMStoreFloat4x4(&Mat, Temp);
					m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_AllyProjectile", m_pGameInstance->Get_Current_LevelID(), L"Effect",&pDesc, &Mat);
				}
			}
		}
	}
	else if (m_eState = DYING)
	{
		if (m_pModelCom->Get_Finished())
		{
			Dead();
		}
	}

	if (m_fLifeTime <= 0.f)
	{
		if(m_eState != DYING)
		{
			m_eState = DYING;
			m_pModelCom->Set_AnimTag("MurielSepia.ao|Ultimate_Teleport_Start", false);
			m_fAnimFrom = 0.f;
		}
	}
	m_fAfterImageTimer += fTimeDelta;
	if (m_fAfterImageTimer >= 0.05f)
	{
		m_pAfterImage->SetUp_Matrices(m_pTransformCom->Get_WorldMatrix());
		m_fAfterImageTimer -= 0.05f;
	}
	m_pAfterImage->Tick(fTimeDelta);
}

void CEffect_Ally::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
	m_pAfterImage->LateTick(fTimeDelta);
}

HRESULT CEffect_Ally::Render()
{
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));

	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	_float4 vColor = {0.9f,0.9f,1.f, 1.f};

	m_pShaderCom->Bind_Vector4("g_vColor", &vColor);
	m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		//auto& vecMat = m_pModelCom->Get_Material(i);
		//if (vecMat.size())
		//{
		//	auto pTexture = vecMat[0];
		//	if (pTexture)
		//		pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
		//	pTexture = vecMat[5];
		//	if (pTexture)
		//	{
		//		pTexture->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
		//	}
		//	else
		//	{
		//		DEFAULTNORMAL->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
		//	}
		//	pTexture = vecMat[14];
		//	if (pTexture)
		//	{
		//		pTexture->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
		//	}
		//	else
		//	{
		//		DEFAULTORM->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
		//	}
		//}
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrix", i);
		m_pShaderCom->Begin((_uint)SHADER_VTXANIMMESH_PASS::MonoColor);
		m_pModelCom->Render(i);
	}
	return S_OK;
}

void CEffect_Ally::Push_From_Pool(void* pArg)
{
	m_eState = START;
	m_pModelCom->Set_AnimTag("MurielSepia.ao|Ultimate_Prepare", false);
	m_fLifeTime = m_pBeatManager->Get_Beat() * 10.f;
	m_fLifeTimeMax = m_fLifeTime;
	m_fAnimFrom = 0.f;
	m_fAnimTo = 1.f;
	XMStoreFloat4(&m_vStartPos, m_pTransformCom->Get_Position());

	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Fire");
	m_iCurrBeat = m_pBeatManager->Get_BeatCount();
	m_iPrevBeat = m_iCurrBeat;
}

void CEffect_Ally::Dying()
{

}


CEffect_Ally* CEffect_Ally::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Ally* pInstance = new CEffect_Ally(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_Ally"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Ally::Clone(void* pArg)
{
	CEffect_Ally* pInstance = new CEffect_Ally(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_Ally"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Ally::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pAfterImage);
}
