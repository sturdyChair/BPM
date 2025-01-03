#include "Effect_DashIndicator.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Rigidbody.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Particle.h"
#include "PointLight.h"
#include "Trail.h"
#include "RoomManager.h"
#include "Room.h"
#include "Monster_Base.h"

_int CEffect_DashIndicator::iLightIdx = 0;

CEffect_DashIndicator::CEffect_DashIndicator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_DashIndicator::CEffect_DashIndicator(const CEffect_DashIndicator& rhs) :
	CEffectObject(rhs)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CEffect_DashIndicator::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_ModelShader", (CComponent**)&m_pModelShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_DashIndicator", L"Com_Model", (CComponent**)&m_pModelCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_PerlinNoise", L"Com_NoiseTexture", (CComponent**)&m_pNoiseTexture);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_EngineFlameMask", L"Com_MaskTexture", (CComponent**)&m_pMaskTexture);


	

	return S_OK;
}

HRESULT CEffect_DashIndicator::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_DashIndicator::PriorityTick(_float fTimeDelta)
{
}

void CEffect_DashIndicator::Tick(_float fTimeDelta)
{
	m_fLifeTime += fTimeDelta;
	//m_fLifeTime -= fTimeDelta;
	//_vector vPos = m_pTransformCom->Get_Position();
	//vPos += XMLoadFloat3(&m_vDirection) * fTimeDelta;
	//m_pTransformCom->Set_Position(vPos);
	if (m_pParent)
		m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(m_pParent));
	m_pTransformCom->Set_Scale(XMVectorSet(3.f, 3.f, 3.f, 0.f));

	//if (m_fLifeTime < 0.f)
	//{
	//	m_pGameInstance->Remove_PointLight(m_strLightTag);
	//	Dead();
	//}
	auto pLight = m_pGameInstance->Find_PointLight(m_strLightTag);
	if (pLight)
	{
		pLight->Set_Position(m_pTransformCom->Get_Position());
	}

	//XMStoreFloat4x4(&m_matDown, XMMatrixTranslation(0.f, 0.3f, 0.f) * m_pTransformCom->Get_WorldMatrix_XMMat());

}

void CEffect_DashIndicator::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_DashIndicator::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pModelShaderCom, "g_WorldMatrix");
	m_pModelShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	_float4 vColor = { 1.f,1.f,1.f,1.f };
	m_pModelShaderCom->Bind_Vector4("g_vAmbientColor", &vColor);
	vColor = { 0.f,1.f,1.f,1.f };
	m_pModelShaderCom->Bind_Vector4("g_vColor", &vColor);
	DEFAULTNOISE->Bind_ShaderResource(m_pModelShaderCom, "g_Noise_Texture");
	m_pModelShaderCom->Bind_Float("g_Dissolve", 0.f);
	m_pModelShaderCom->Bind_Float("g_fMagicNumber", m_fLifeTime);
	m_pNoiseTexture->Bind_ShaderResource(m_pModelShaderCom, "g_PerlinNoise_Texture");
	m_pMaskTexture->Bind_ShaderResource(m_pModelShaderCom, "g_Mask_Texture");
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		auto& vecMat = m_pModelCom->Get_Material(i);
		if (vecMat.size())
		{
			auto pTexture = vecMat[0];
			if (pTexture)
				pTexture->Bind_ShaderResource(m_pModelShaderCom, "g_Texture");


		}
		m_pModelShaderCom->Begin(_uint(SHADER_VTXMESH_PASS::DissolveMaskNoise));
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_DashIndicator::Push_From_Pool(void* pArg)
{
	m_pGameInstance->Remove_PointLight(m_strLightTag);
	m_pParent = (_float4x4*)pArg;
	m_strLightTag = L"DashPoint";
	m_strLightTag += to_wstring(iLightIdx);
	++iLightIdx;
	PointLight pLight{};
	pLight.color = { 0.2f,0.8f,0.7f };
	pLight.intensity = 2.f;
	_vector vPos = m_pTransformCom->Get_Position();
	XMStoreFloat3(&pLight.position, vPos);
	pLight.range = 8.f;
	pLight.specColor = { 0.7f,0.7f,0.2f };
	if (FAILED(m_pGameInstance->Add_Light(m_strLightTag, pLight)))
	{
	}
}

void CEffect_DashIndicator::Dying()
{
	m_pGameInstance->Remove_PointLight(m_strLightTag);
}

CEffect_DashIndicator* CEffect_DashIndicator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_DashIndicator* pInstance = new CEffect_DashIndicator(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_DashIndicator"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_DashIndicator::Clone(void* pArg)
{
	CEffect_DashIndicator* pInstance = new CEffect_DashIndicator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_DashIndicator"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_DashIndicator::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pModelShaderCom);
	Safe_Release(m_pNoiseTexture);
	Safe_Release(m_pMaskTexture);
}
