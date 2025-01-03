#include "Effect_SurtLaser_Warning.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"

CEffect_SurtLaser_Warning::CEffect_SurtLaser_Warning(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_SurtLaser_Warning::CEffect_SurtLaser_Warning(const CEffect_SurtLaser_Warning& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_SurtLaser_Warning::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_RectNorm", L"Com_VIBuffer", (CComponent**)&m_pVICom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_LaserWarning", L"Com_Texture", (CComponent**)&m_pDiffuseTextureCom);
	m_pTransformCom->Set_WorldMatrix(XMMatrixAffineTransformation(XMVectorSet(1.f, 1.f, 1.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), XMQuaternionRotationRollPitchYaw(0.f, 0.f, XM_PIDIV2),
		XMVectorSet(0.f, 0.f, 0.f, 1.f)));
	return S_OK;
}

HRESULT CEffect_SurtLaser_Warning::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_SurtLaser_Warning::PriorityTick(_float fTimeDelta)
{
}

void CEffect_SurtLaser_Warning::Tick(_float fTimeDelta)
{
	for (_uint i = 0; i < 10; ++i)
	{
		m_fOffset[i] += fTimeDelta * m_fRandSpeed[i] * XM_PI;
	}
	_matrix LTMatrix = XMMatrixIdentity();
	LTMatrix.r[3] = m_pTransformCom->Get_Position();
	_vector vDir;
	vDir = XMLoadFloat4x4(m_pTarget).r[3] - LTMatrix.r[3];
	LTMatrix.r[1] = (vDir);
	LTMatrix.r[0] = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMVector3Normalize(LTMatrix.r[1]));
	LTMatrix.r[2] = XMVector3Cross(XMVector3Normalize(LTMatrix.r[1]), LTMatrix.r[0]);
	LTMatrix.r[0] *= 0.2f;
	m_pTransformCom->Set_WorldMatrix(LTMatrix);

	m_fLifeTime -= fTimeDelta;
	if (m_fLifeTime <= m_iLeft * 0.1f * CBeat_Manager::Get_Instance()->Get_Beat() * 0.5f)
	{
		_float4x4 Effect;
		_matrix id = XMMatrixIdentity();
		id.r[3] = m_pTransformCom->Get_Position();
		_vector vDist = XMVectorSet(m_pTarget->_41, m_pTarget->_42, m_pTarget->_43, 1.f) - id.r[3];
		id.r[3] += vDist * 0.1f * (11 - m_iLeft);
		XMStoreFloat4x4(&Effect, id);
		m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_SurtLaser", m_pGameInstance->Get_Current_LevelID(), L"Effect", nullptr, &Effect);
		id.r[3] += vDist;

		--m_iLeft;
	}

	if (m_fLifeTime < 0.f)
	{
		Dead();

	}
}

void CEffect_SurtLaser_Warning::LateTick(_float fTimeDelta)
{
	XMStoreFloat4x4(&m_TransformMat, XMLoadFloat4x4(&m_LocalOffset) * m_pTransformCom->Get_WorldMatrix_XMMat());
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_SurtLaser_Warning::Render()
{

	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));

	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pDiffuseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0);
	m_pShaderCom->Bind_Float("g_fMagicNumber", max((m_fLifeDuration - m_fLifeTime) / m_fLifeDuration * 2.f, 1.f));
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	_matrix World = XMLoadFloat4x4(&m_TransformMat);
	for(_uint i = 0;i < 10; ++i)
	{
		_float4x4 MatTmp;
		XMStoreFloat4x4(&MatTmp, XMMatrixScaling(m_fScaleOffset[i], 1.f,1.f) * XMMatrixTranslation(cos(m_fOffset[i]) * m_fMaxOffset[i], 0.f, (i - 5) * 0.01f) * World);
		m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MatTmp);
		m_pShaderCom->Begin((_uint)SHADER_VTXNOR_PASS::ColorBoost);

		m_pVICom->Render();
	}
	return S_OK;
}

void CEffect_SurtLaser_Warning::Push_From_Pool(void* pArg)
{
	LASER_DESC* pDesc = (LASER_DESC*)pArg;
	m_fLifeTime = pDesc->fLifeTime;
	m_fLifeDuration = m_fLifeTime;
	XMStoreFloat4x4(&m_LocalOffset, XMMatrixTranslation(0.f, 0.5f, 0.f));
	m_pTarget = pDesc->pTarget;
	m_iLeft = 10;
	for (_uint i = 0; i < 10; ++i)
	{
		m_fRandSpeed[i] = fRand(-6.f, 6.f);
		m_fScaleOffset[i] = fRand(0.5f, 2.f);
		m_fMaxOffset[i] = fRand(-4.f, 4.f);
		m_fOffset[i] = fRand(0.f, XM_2PI);
	}
}

void CEffect_SurtLaser_Warning::Dying()
{
}

CEffect_SurtLaser_Warning* CEffect_SurtLaser_Warning::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_SurtLaser_Warning* pInstance = new CEffect_SurtLaser_Warning(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_SurtLaser_Warning"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_SurtLaser_Warning::Clone(void* pArg)
{
	CEffect_SurtLaser_Warning* pInstance = new CEffect_SurtLaser_Warning(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_SurtLaser_Warning"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_SurtLaser_Warning::Free()
{
	__super::Free();
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pVICom);
	Safe_Release(m_pShaderCom);
}
