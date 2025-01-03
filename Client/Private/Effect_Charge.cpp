#include "Effect_Charge.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"

CEffect_Charge::CEffect_Charge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_Charge::CEffect_Charge(const CEffect_Charge& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_Charge::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_RectNorm", L"Com_VIBuffer", (CComponent**)&m_pVICom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Charge", L"Com_Texture", (CComponent**)&m_pDiffuseTextureCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_ChargeMask", L"Com_MaskTexture", (CComponent**)&m_pMaskTextureCom);
	m_pMaskTextureCom->Set_Speed(4.f);
	m_pTransformCom->Set_Scale(_float3{ 1.5f,1.5f,1.f });
	return S_OK;
}

HRESULT CEffect_Charge::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_Charge::PriorityTick(_float fTimeDelta)
{
}

void CEffect_Charge::Tick(_float fTimeDelta)
{

	m_fLifeTime -= fTimeDelta;
	m_pMaskTextureCom->MoveFrame(fTimeDelta, true);
	if (m_fLifeTime < 0.f)
	{
		Dead();
	}
}

void CEffect_Charge::LateTick(_float fTimeDelta)
{
	if (m_pParentMat)
	{
		_matrix Parent = XMLoadFloat4x4(m_pParentMat);
		Parent.r[0] = XMVector3Normalize(Parent.r[0]);
		Parent.r[1] = XMVector3Normalize(Parent.r[1]);
		Parent.r[2] = XMVector3Normalize(Parent.r[2]);
		_matrix World = XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * Parent;
		//XMStoreFloat4x4(&m_TransformMat, World);
		_vector vPos = World.r[3];
		_matrix View = m_pGameInstance->Get_Transform_Matrix(CPipeLine::TS_VIEW_INV);
		_vector vCamPos = View.r[3];
		_float3 vScale = m_pTransformCom->Get_Scale();

		_vector vLook = XMVector3Normalize(vCamPos - vPos);
		_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
		_vector vUp = XMVector3Cross(vLook, vRight);
		View.r[0] = vRight * vScale.x;
		View.r[1] = vUp * vScale.y;
		View.r[2] = vLook * vScale.z;
		View.r[3] = vPos;
		XMStoreFloat4x4(&m_TransformMat, View);

	}
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_Charge::Render()
{
	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_TransformMat);
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pMaskTextureCom->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", m_pMaskTextureCom->Get_Frame());
	m_pDiffuseTextureCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture");
	m_pShaderCom->Bind_Float("g_fMagicNumber", min((m_fLifeDuration - m_fLifeTime)/ (m_fLifeDuration * 0.1f), 1.f));
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	m_pShaderCom->Begin((_uint)SHADER_VTXNOR_PASS::Charge);
	m_pVICom->Render();

	return S_OK;
}

void CEffect_Charge::Push_From_Pool(void* pArg)
{
	CHARGE_DESC* pDesc = (CHARGE_DESC*)pArg;
	m_pParentMat = pDesc->pParent;
	m_fLifeTime = pDesc->fLifeTime;
	m_fLifeDuration = m_fLifeTime;
	//m_fLifeTime = 1.0f;
}

void CEffect_Charge::Dying()
{
	m_pParentMat = nullptr;
}

CEffect_Charge* CEffect_Charge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Charge* pInstance = new CEffect_Charge(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_Charge"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Charge::Clone(void* pArg)
{
	CEffect_Charge* pInstance = new CEffect_Charge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_Charge"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Charge::Free()
{
	__super::Free();
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pMaskTextureCom);
	Safe_Release(m_pVICom);
	Safe_Release(m_pShaderCom);
}
