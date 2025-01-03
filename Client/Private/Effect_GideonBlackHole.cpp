#include "Effect_GideonBlackHole.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Beat_Manager.h"
#include "Effect_GideonSpear.h"
#include "Effect_BlockLight.h"

CEffect_GideonBlackHole::CEffect_GideonBlackHole(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_GideonBlackHole::CEffect_GideonBlackHole(const CEffect_GideonBlackHole& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_GideonBlackHole::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_GideonBlackHole", L"Com_Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CEffect_GideonBlackHole::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_GideonBlackHole::PriorityTick(_float fTimeDelta)
{
	m_iPrevCounter = m_iCurrCounter;
	m_iCurrCounter = CBeat_Manager::Get_Instance()->Get_BeatCount();
}

void CEffect_GideonBlackHole::Tick(_float fTimeDelta)
{
	_float3 vScale = m_pTransformCom->Get_Scale();
	if (vScale.x < 8.f)
	{
		vScale.x += fTimeDelta * 24.f;
	}
	else
	{
		vScale.x = 8.f;
	}
	if (vScale.y < 8.f)
	{
		vScale.y += fTimeDelta * 24.f;
	}
	else
	{
		vScale.y = 8.f;
	}
	if (vScale.z < 8.f)
	{
		vScale.z += fTimeDelta * 24.f;
	}
	else
	{
		vScale.z = 8.f;
	}
	m_pTransformCom->Set_Scale(XMVectorSetW(XMLoadFloat3(&vScale), 0.f));
	XMStoreFloat4x4(&m_MatWorld, XMMatrixScaling(fRand(1.f, 1.2f), fRand(1.f, 1.2f), fRand(1.f, 1.2f)) * m_pTransformCom->Get_WorldMatrix_XMMat());
	
	m_MatWorld._41 += fRand(-1.f, 1.f);
	m_MatWorld._42 += fRand(-1.f, 1.f);
	m_MatWorld._43 += fRand(-1.f, 1.f);
	_float fBeat = CBeat_Manager::Get_Instance()->Get_Beat();
	_float fBeatTimer = CBeat_Manager::Get_Instance()->Get_Timer();
	m_vColor.y = 1.f + (fBeat - fBeatTimer) / fBeat;
	m_vColor.z = fBeatTimer / fBeat;
	m_vColor.w = m_vColor.z;
	auto pPlayer = CPlayer_Manager::Get_Instance()->Get_Player();
	_float3 vTarget = pPlayer->Get_FootPosition();
	_vector vVecTarget = XMLoadFloat3(&vTarget);
	m_pTransformCom->LookAt(vVecTarget);
	_vector vPos = m_pTransformCom->Get_Position();
	if (XMVectorGetX(XMVector3Length(vVecTarget - vPos)) <= 3.f)
	{
		_float3 vHitPoint;
		XMStoreFloat3(&vHitPoint, m_pTransformCom->Get_Position());
		pPlayer->Take_Damage(25.f, vHitPoint, DAMAGE_TYPE::DMG_DEFAULT, this);
	}
	
	if (m_iPrevCounter != m_iCurrCounter)
	{
		--m_iLifeTime;
		_float3 vPush;
		XMStoreFloat3(&vPush,XMVector3Normalize(vPos - vVecTarget) * 60.f);
		pPlayer->Add_PushVector(vPush, 0.1f);
		if(m_iLifeTime % 2)
		{
			CEffect_BlockLight::BLOCK_DESC bdesc;
			bdesc.bOdd = m_bOdd;
			m_bOdd = !m_bOdd;
			bdesc.fBlockSize = 20.f;
			bdesc.fLifeTime = fBeat * 2.f;
			bdesc.vColor = _float4{ 1.f,0.8f,0.2f,1.f };
			XMStoreFloat4(&bdesc.vOrigin, m_pTransformCom->Get_Position());
			m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_BlockLight", m_pGameInstance->Get_Current_LevelID(), L"Effect", &bdesc);
		}

	}
	if (m_iLifeTime <= 0)
	{
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"ThunderClap");

		m_pGameInstance->Remove_SpotLight(m_strPointLightTag);
		Dead();
	}
}

void CEffect_GideonBlackHole::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_GideonBlackHole::Render()
{
	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_MatWorld);
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	//m_pShaderCom->Bind_Float("g_fMagicNumber", max(0.5f, (m_fLifeDuration * 1.5f - m_fLifeTime) / m_fLifeDuration));

	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &m_vColor);
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
		m_pShaderCom->Begin((_uint)SHADER_VTXMESH_PASS::DecalToDiffuse);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_GideonBlackHole::Push_From_Pool(void* pArg)
{
	m_iLifeTime = 20;
	m_pTransformCom->Set_Scale(XMVectorSet(0.8f, 0.8f, 0.8f, 0.f));
	m_vColor = { 2.f,2.f,2.f,2.f };
	//m_fLifeTime = 1.0f;
	SpotLight sLight;
	sLight.color = { 0.3f,0.5f,1.2f };
	sLight.direction = { 0.f, 1.f,0.f };
	sLight.innerConeCos = cos(XM_PI * 0.25f);
	sLight.intensity = 5.f;
	sLight.outerConeCos = cos(XM_PI);
	XMStoreFloat3(&sLight.position, m_pTransformCom->Get_Position());
	sLight.range = 300.f;
	sLight.specColor = { 0.3f,0.5f,1.2f };
	m_pGameInstance->Add_Light(m_strPointLightTag, sLight);
}

void CEffect_GideonBlackHole::Dying()
{
	m_pParentMat = nullptr;
}

CEffect_GideonBlackHole* CEffect_GideonBlackHole::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_GideonBlackHole* pInstance = new CEffect_GideonBlackHole(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_GideonBlackHole"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_GideonBlackHole::Clone(void* pArg)
{
	CEffect_GideonBlackHole* pInstance = new CEffect_GideonBlackHole(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_GideonBlackHole"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_GideonBlackHole::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);

}
