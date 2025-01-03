#include "Particle_Rect.h"
#include "Shader.h"
#include "Texture.h"
#include "GameInstance.h"
CParticle_Rect::CParticle_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	CParticle(pDevice, pContext)
{
}

CParticle_Rect::CParticle_Rect(const CParticle_Rect& rhs):
	CParticle(rhs), m_strBufferTag(rhs.m_strBufferTag)
{
}

HRESULT CParticle_Rect::Initialize(void* Arg)
{
	__super::Initialize(Arg);
	
	Add_Component(LEVEL_STATIC, m_strBufferTag, L"Com_VIBuffer", (CComponent**)&m_pVBCom, &m_tDesc);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxRectInstance", L"Com_Shader", (CComponent**)&m_pShaderCom);

	return S_OK;
}

HRESULT CParticle_Rect::Initialize_Prototype(const PARTICLE_DESC& desc)
{
	__super::Initialize_Prototype(desc);
	m_strBufferTag = _wstring(L"Prototype_Component_VIBuffer_Rect_Instance") + to_wstring(m_iObjectID);
	m_pGameInstance->Add_Prototype(LEVEL_STATIC, m_strBufferTag,
		CVIBuffer_Rect_Instance::Create(m_pDevice, m_pContext, &desc));

	return S_OK;
}



void CParticle_Rect::Push_From_Pool(void* pArg)
{
	__super::Push_From_Pool(pArg);
}

void CParticle_Rect::Dying()
{
	__super::Dying();
}

CParticle_Rect* CParticle_Rect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const PARTICLE_DESC& desc)
{
	CParticle_Rect* pInstance = new CParticle_Rect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(desc)))
	{
		MSG_BOX(TEXT("Failed to Create : CParticle_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CParticle_Rect::Clone(void* pArg)
{
	CParticle_Rect* pInstance = new CParticle_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CParticle_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Rect::Free()
{
	__super::Free();
}
