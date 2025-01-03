#include "Particle_Point.h"
#include "Shader.h"
#include "Texture.h"
#include "GameInstance.h"
CParticle_Point::CParticle_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CParticle(pDevice, pContext)
{
}

CParticle_Point::CParticle_Point(const CParticle_Point& rhs) :
	CParticle(rhs), m_strBufferTag(rhs.m_strBufferTag)
{
}

HRESULT CParticle_Point::Initialize(void* Arg)
{
	__super::Initialize(Arg);

	Add_Component(LEVEL_STATIC, m_strBufferTag, L"Com_VIBuffer", (CComponent**)&m_pVBCom, &m_tDesc);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPointInstance", L"Com_Shader", (CComponent**)&m_pShaderCom);

	return S_OK;
}

HRESULT CParticle_Point::Initialize_Prototype(const PARTICLE_DESC& desc)
{
	__super::Initialize_Prototype(desc);
	m_strBufferTag = _wstring(L"Prototype_Component_VIBuffer_Point_Instance") + to_wstring(m_iObjectID);
	m_pGameInstance->Add_Prototype(LEVEL_STATIC, m_strBufferTag,
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &desc));

	return S_OK;
}



void CParticle_Point::Push_From_Pool(void* pArg)
{
	__super::Push_From_Pool(pArg);
}

void CParticle_Point::Dying()
{
	__super::Dying();
}

HRESULT CParticle_Point::Render()
{
	m_pShaderCom->Bind_Vector4("g_vCamPosition", &m_pGameInstance->Get_CamPosition_Float4());

	__super::Render();

	return S_OK;
}

CParticle_Point* CParticle_Point::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const PARTICLE_DESC& desc)
{
	CParticle_Point* pInstance = new CParticle_Point(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(desc)))
	{
		MSG_BOX(TEXT("Failed to Create : CParticle_Point"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CParticle_Point::Clone(void* pArg)
{
	CParticle_Point* pInstance = new CParticle_Point(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CParticle_Point"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Point::Free()
{
	__super::Free();
}
