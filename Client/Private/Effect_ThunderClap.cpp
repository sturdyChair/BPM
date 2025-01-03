#include "Effect_ThunderClap.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Room.h"
#include "RoomManager.h"
#include "Monster_Base.h"

_int CEffect_ThunderClap::iLightIdx = 0;

CEffect_ThunderClap::CEffect_ThunderClap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_ThunderClap::CEffect_ThunderClap(const CEffect_ThunderClap& rhs) :
	CEffectObject(rhs)
{
	m_strLightTag = L"ThunderPoint";
	m_strLightTag += to_wstring(iLightIdx);
	++iLightIdx;
}

HRESULT CEffect_ThunderClap::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_RectNorm", L"Com_VIBuffer", (CComponent**)&m_pVICom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_LightnigArc", L"Com_Texture", (CComponent**)&m_pDiffuseTextureCom);
	m_pTransformCom->Set_WorldMatrix(XMMatrixAffineTransformation(XMVectorSet(1.f, 1.f, 1.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), XMQuaternionRotationRollPitchYaw(0.f, 0.f, XM_PIDIV2),
		XMVectorSet(0.f, 0.f, 0.f, 1.f)));
	m_pDiffuseTextureCom->Set_Speed(400.f);
	m_vColor;

	return S_OK;
}

HRESULT CEffect_ThunderClap::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_ThunderClap::PriorityTick(_float fTimeDelta)
{
}

void CEffect_ThunderClap::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	if (m_fLifeTime < 0.f)
	{
		m_pGameInstance->Remove_PointLight(m_strLightTag);
		Dead();
	}
	auto& vecMonsters = CRoomManager::Get_Instance()->Get_Room(CRoomManager::Get_Instance()->Get_CurrentRoomIdx())->Get_Monsters();
	for (auto pMonster : vecMonsters)
	{
		if (pMonster)
		{
			if (XMVectorGetX(XMVector4Length(pMonster->Get_Transform()->Get_Position() - m_pTransformCom->Get_Position())) < 6.f)
			{
				if (Is_Dead())
					return;

				_float3 vHitPoint;
				XMStoreFloat3(&vHitPoint, m_pTransformCom->Get_Position());
				pMonster->Take_Damage(50.f, vHitPoint, DAMAGE_TYPE::DMG_EXPLODE, this);

			}
		}
	}
}

void CEffect_ThunderClap::LateTick(_float fTimeDelta)
{
	XMStoreFloat4x4(&m_TransformMat, XMLoadFloat4x4(&m_LocalOffset) * m_pTransformCom->Get_WorldMatrix_XMMat());
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_ThunderClap::Render()
{
	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_TransformMat);
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));

	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pDiffuseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTexIdx);
	m_pShaderCom->Bind_Float("g_fMagicNumber", min(m_fLifeTime / m_fLifeDuration, 1.f));

	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &m_vColor);
	m_pShaderCom->Begin((_uint)SHADER_VTXNOR_PASS::ColorBoost);
	m_pVICom->Render();

	return S_OK;
}

void CEffect_ThunderClap::Push_From_Pool(void* pArg)
{
	LIGHRNIG_DESC* pDesc = (LIGHRNIG_DESC*)pArg;
	;
	m_fLifeTime = pDesc->fLifeTime;
	m_fLifeDuration = m_fLifeTime;
	m_pTransformCom->Set_Scale(XMLoadFloat3(&pDesc->vScale));
	XMStoreFloat4x4(&m_LocalOffset, XMMatrixTranslation(0.f, 0.5f, 0.f));
	_vector vPos = (XMMatrixTranslation(0.f, 1.0f, 0.f) * m_pTransformCom->Get_WorldMatrix_XMMat()).r[3];

	PointLight pLight;
	pLight.color = { 1.f,1.f,0.5f };
	pLight.intensity = 2.f;
	pLight.range = 4.f * pDesc->vScale.x;
	XMStoreFloat3(&pLight.position, m_pTransformCom->Get_Position());
	pLight.specColor = { 1.f,1.f,0.5f };
	m_pGameInstance->Add_Light(m_strLightTag, pLight);
	XMStoreFloat4(&m_vColor, XMLoadFloat4(&s_vAmbientColor) * XMVectorSet(1.f, 0.2f, 0.8f, 1.f));
	if (pDesc->iTex >= 9)
	{
		m_iTexIdx = rand() % 8;
	}
	else
	{
		m_iTexIdx = pDesc->iTex;
	}
}

void CEffect_ThunderClap::Dying()
{
	m_pGameInstance->Remove_PointLight(m_strLightTag);
}

CEffect_ThunderClap* CEffect_ThunderClap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_ThunderClap* pInstance = new CEffect_ThunderClap(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_ThunderClap"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_ThunderClap::Clone(void* pArg)
{
	CEffect_ThunderClap* pInstance = new CEffect_ThunderClap(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_ThunderClap"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_ThunderClap::Free()
{
	__super::Free();
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pVICom);
	Safe_Release(m_pShaderCom);
}
