#include "Effect_Gullveig_LightWall.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Beat_Manager.h"
#include "Effect_LightningArc.h"

CEffect_Gullveig_LightWall::CEffect_Gullveig_LightWall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_Gullveig_LightWall::CEffect_Gullveig_LightWall(const CEffect_Gullveig_LightWall& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_Gullveig_LightWall::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Gullveig_LightWall", L"Com_Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CEffect_Gullveig_LightWall::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_Gullveig_LightWall::PriorityTick(_float fTimeDelta)
{
	m_iPrevCount = m_iCurrCount;
	m_iCurrCount = CBeat_Manager::Get_Instance()->Get_BeatCount();
	if (m_iPrevCount != m_iCurrCount)
	{
		++m_iRotateCount;
		m_iRotateCount %= 6;
	}
}

void CEffect_Gullveig_LightWall::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	_float fBeat = CBeat_Manager::Get_Instance()->Get_Beat();
	if (m_fLifeTime < fBeat)
	{
		m_fDissolve = 1.f - (m_fLifeTime / fBeat);
	}
	else if (m_fLifeDuration - m_fLifeTime < fBeat)
	{
		m_fDissolve = (m_fLifeDuration - m_fLifeTime) / fBeat;
	}
	else
	{
		m_fDissolve = 0.f;
	}
	if (m_iRotateCount == 3 || m_iRotateCount == 4 || m_iRotateCount == 5)
	{
		m_fAngle += XM_PIDIV2 * fTimeDelta / fBeat * 0.33f;
	}
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fAngle);
	auto pPlayer = CPlayer_Manager::Get_Instance()->Get_Player();
	_float3 vTarget = pPlayer->Get_FootPosition();
	_float fTarget = vTarget.y;
	_vector vTargetPos = XMVectorSetW(XMLoadFloat3(&vTarget), 1.f);
	_vector vPos = m_pTransformCom->Get_Position();
	_vector vDir = vTargetPos - vPos;
	_float fDot = XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMVector3Rotate(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMQuaternionRotationRollPitchYaw(0.f,m_fAngle,0.f))));
	if((fDot < 1.05f && fDot > 0.95f) || (fDot < 0.05f && fDot > -0.05f) || (fDot > -1.05f && fDot < -0.95f))
	{
		if ((fTarget - XMVectorGetY(vPos)) < 2.0f)
		{
			pPlayer->Take_Damage(25.f, vTarget, DMG_EXPLODE, this);
		}
	}

	if (m_fLifeTime < 0.f)
	{
		Dead();
	}
}

void CEffect_Gullveig_LightWall::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_Gullveig_LightWall::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));

	DEFAULTNOISE->Bind_ShaderResource(m_pShaderCom, "g_Noise_Texture");
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	m_pShaderCom->Bind_Float("g_Dissolve", m_fDissolve);
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
		m_pShaderCom->Begin((_uint)SHADER_VTXMESH_PASS::DissolveDecal);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_Gullveig_LightWall::Push_From_Pool(void* pArg)
{
	RF_DESC* pDesc = (RF_DESC*)pArg;
	m_fLifeTime = pDesc->fLifeTime;
	m_fLifeDuration = m_fLifeTime;
	m_pTransformCom->Set_Scale(XMVectorSet(90.f, 4.f, 90.f, 0.f));
	m_fDissolve = 0.f;
	//m_fLifeTime = 1.0f;
	m_iRotateCount = 0;
	m_iCurrCount = CBeat_Manager::Get_Instance()->Get_BeatCount();
	m_iPrevCount = m_iCurrCount;
	m_fAngle = XM_PI * 0.25f;
	
}

void CEffect_Gullveig_LightWall::Dying()
{
	m_pParentMat = nullptr;
}

CEffect_Gullveig_LightWall* CEffect_Gullveig_LightWall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Gullveig_LightWall* pInstance = new CEffect_Gullveig_LightWall(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_Gullveig_LightWall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Gullveig_LightWall::Clone(void* pArg)
{
	CEffect_Gullveig_LightWall* pInstance = new CEffect_Gullveig_LightWall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_Gullveig_LightWall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Gullveig_LightWall::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
