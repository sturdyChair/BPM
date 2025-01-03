#include "Effect_Hex_Shield.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"

CEffect_Hex_Shield::CEffect_Hex_Shield(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_Hex_Shield::CEffect_Hex_Shield(const CEffect_Hex_Shield& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_Hex_Shield::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Hex_Shield", L"Com_Model", (CComponent**)&m_pModelCom);
	m_pParentMat = (_float4x4*)Arg;
	return S_OK;
}

HRESULT CEffect_Hex_Shield::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_Hex_Shield::PriorityTick(_float fTimeDelta)
{
}

void CEffect_Hex_Shield::Tick(_float fTimeDelta)
{
	m_fBlock -= fTimeDelta;
	if (m_fBlock <= 0.f)
	{
		m_fBlock = 0.f;
	}
}

void CEffect_Hex_Shield::LateTick(_float fTimeDelta)
{
	if (m_pParentMat)
	{
		_matrix Parent = XMLoadFloat4x4(m_pParentMat);
		Parent.r[0] = XMVector3Normalize(Parent.r[0]);
		Parent.r[1] = XMVector3Normalize(Parent.r[1]);
		Parent.r[2] = XMVector3Normalize(Parent.r[2]);
		_matrix World = XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * Parent;
		XMStoreFloat4x4(&m_TransformMat, World);
	}
	//m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND, this);
}

HRESULT CEffect_Hex_Shield::Render()
{
	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_TransformMat);
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));

	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	m_pGameInstance->Bind_RT_SRV(L"RT_BeforeBack", m_pShaderCom, "g_Texture");
	_float2 vResolution = { (_float)g_iWinSizeX, (_float)g_iWinSizeY };
	m_pShaderCom->Bind_RawValue("g_vResolution", &vResolution, sizeof(_float2));
	m_pShaderCom->Bind_Float("g_fNoiseStrength", 0.1f + m_fBlock);
	if (m_fBlock <= 0.05f)
	{
		m_pShaderCom->Bind_Float("g_fMagicNumber", 1.0f);
	}
	else
	{
		m_pShaderCom->Bind_Float("g_fMagicNumber", 2.0f);
	}

	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		auto& vecMat = m_pModelCom->Get_Material(i);
		if (vecMat.size())
		{
			auto pTexture = vecMat[0];
			if (pTexture)
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_Noise_Texture");
		}
		m_pShaderCom->Begin((_uint)SHADER_VTXMESH_PASS::Distortion);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_Hex_Shield::Push_From_Pool(void* pArg)
{



}

void CEffect_Hex_Shield::Dying()
{
	m_pParentMat = nullptr;
}

void CEffect_Hex_Shield::Block()
{
	m_fBlock = 0.2f;

}

CEffect_Hex_Shield* CEffect_Hex_Shield::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Hex_Shield* pInstance = new CEffect_Hex_Shield(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_Hex_Shield"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Hex_Shield::Clone(void* pArg)
{
	CEffect_Hex_Shield* pInstance = new CEffect_Hex_Shield(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_Hex_Shield"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Hex_Shield::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
