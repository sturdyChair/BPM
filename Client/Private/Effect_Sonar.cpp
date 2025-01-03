#include "Effect_Sonar.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "GameInstance.h"
#include "Texture.h"


CEffect_Sonar::CEffect_Sonar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_Sonar::CEffect_Sonar(const CEffect_Sonar& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_Sonar::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVICom);

	return S_OK;
}

HRESULT CEffect_Sonar::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_Sonar::PriorityTick(_float fTimeDelta)
{
}

void CEffect_Sonar::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime >= m_fLifeTime)
	{
		Dead();
	}
}

void CEffect_Sonar::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_Sonar::Render()
{
	m_pGameInstance->Bind_RT_SRV(L"RT_Position", m_pShaderCom, "g_PositionTexture");

	m_pShaderCom->Bind_Vector4("g_Color", &m_vColor);
	m_pShaderCom->Bind_Vector4("SonarOrigin", &m_vOrigin);
	m_pShaderCom->Bind_Float("Time", m_fTime);
	m_pShaderCom->Bind_Float("SonarSpeed", m_fSonarSpeed);
	m_pShaderCom->Bind_Float("SonarInterval", m_fSonarInterval);
	m_pShaderCom->Bind_Float("SonarThickness", m_fSonarThickness);

	m_pShaderCom->Begin((_uint)SHADER_VTXPOS_PASS::Sonar);
	m_pVICom->Render();

	return S_OK;
}

void CEffect_Sonar::Push_From_Pool(void* pArg)
{
	SONAR_DESC* pDesc = (SONAR_DESC*)pArg;
	m_fLifeTime = pDesc->fLifeTime;
	m_vOrigin = pDesc->vOrigin;
	m_vColor = pDesc->vColor;
	m_fTime = 0.f;
	m_fSonarSpeed = pDesc->fSonarSpeed;
	m_fSonarInterval = pDesc->fSonarInterval;
	m_fSonarThickness = pDesc->fSonarThickness;
	
}

void CEffect_Sonar::Dying()
{
}

CEffect_Sonar* CEffect_Sonar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Sonar* pInstance = new CEffect_Sonar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_Sonar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Sonar::Clone(void* pArg)
{
	CEffect_Sonar* pInstance = new CEffect_Sonar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_Sonar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Sonar::Free()
{
	__super::Free();

	Safe_Release(m_pVICom);
	Safe_Release(m_pShaderCom);
}
