#include "..\Public\Camera_FPS.h"
#include "CharacterController.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Texture.h"
#include "Beat_Manager.h"

CCamera_FPS::CCamera_FPS(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{
}

CCamera_FPS::CCamera_FPS(const CCamera_FPS& rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_FPS::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_FPS::Initialize(void* pArg)
{
	CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pLensBuffer);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_RedScreen", L"Com_RedScreenTexture", (CComponent**)&m_pRedScreenTexture);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_White", L"Com_WhiteTexture", (CComponent**)&m_pWhiteTexture);
	XMStoreFloat4x4(&m_WorldMat, XMLoadFloat4x4(m_pGameInstance->Get_ScreenQuadWorld()));
	m_WorldMat._43 = 0.95f;
	return S_OK;
}

void CCamera_FPS::PriorityTick(_float fTimeDelta)
{
	m_EffectOffset = Identity;
	for (auto& flag : m_bLensEffect)
	{
		flag = false;
	}
	_Shake(fTimeDelta);
	_Dissolve(fTimeDelta);
	if(m_pTargetTransform)
	{
		_matrix Matrix = m_pTargetTransform->Get_WorldMatrix_XMMat();
		_vector vOffset = XMLoadFloat3(&m_vOffset);
		Matrix.r[3] += vOffset;
		//_vector vPos = m_pTransformCom->Get_Position();
		//_vector vDir = Matrix.r[3] - vPos;
		//_float fYDist = XMVectorGetY(vDir);
		//vDir = XMVectorSetY(vDir, 0.f);
		//_float fDist = XMVectorGetX(XMVector3Length(vDir));
		//if (fDist > 10.f)
		//{

		//}
		//else if (fDist > 0.05f)
		//{
		//	Matrix.r[3] = vPos + vDir * fTimeDelta * 5.f;
		//	Matrix.r[3] += XMVectorSet(0.f, fYDist, 0.f, 0.f);
		//}

		m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&m_EffectOffset) * Matrix);
	}

	__super::PriorityTick(fTimeDelta);

}

void CCamera_FPS::Tick(_float fTimeDelta)
{

}

void CCamera_FPS::LateTick(_float fTimeDelta)
{
	_LensEffect(fTimeDelta);
	for(auto& flag : m_bLensEffect)
	{
		if (flag)
		{
			m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_LENS, this);
			return;
		}
	}
}

HRESULT CCamera_FPS::Render()
{
	
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &Identity);
	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMat);
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_ScreenQuadProjection());
	if(m_bLensEffect[LE_REDSCREEN])
	{
		m_pRedScreenTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0);
		_float4 vColor = m_arrLensEffect[LE_REDSCREEN].front().vColor;
		vColor.w *= m_arrLensEffect[LE_REDSCREEN].front().fTimer / m_arrLensEffect[LE_REDSCREEN].front().fDuration;
		m_pShaderCom->Bind_Vector4("g_Color", &vColor);
		m_pShaderCom->Begin((_uint)SHADER_VTXPOS_PASS::Blend);
		m_pLensBuffer->Render();
	}
	if (m_bLensEffect[LE_COLORSCREEN])
	{
		m_pWhiteTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0);
		_float4 vColor = m_arrLensEffect[LE_COLORSCREEN].front().vColor;
		//vColor.w *= m_arrLensEffect[LE_COLORSCREEN].front().fTimer / m_arrLensEffect[LE_COLORSCREEN].front().fDuration;
		m_pShaderCom->Bind_Vector4("g_Color", &vColor);
		m_pShaderCom->Begin((_uint)SHADER_VTXPOS_PASS::Blend);
		m_pLensBuffer->Render();
	}


	return S_OK;
}

void CCamera_FPS::Set_Target(CTransform* pTarget)
{
	Safe_Release(m_pTargetTransform);
	m_pTargetTransform = pTarget;
	if (m_pTargetTransform)
		m_pTransformCom->Set_WorldMatrix(m_pTargetTransform->Get_WorldMatrix());
	Safe_AddRef(m_pTargetTransform);
}

void CCamera_FPS::Set_Offset(_fvector vOffset)
{
	XMStoreFloat3(&m_vOffset, vOffset);
}

void CCamera_FPS::Shake(_float fDuration, _float fPower)
{
	m_arrEffect[CE_SHAKE].push_back({ fDuration, fPower, fDuration });
}

void CCamera_FPS::Dissolve(_float fDuration, _float fPower)
{
	m_arrEffect[CE_DISSOLVE].push_back({ fDuration, fPower, fDuration });
}

void CCamera_FPS::Clear_LensEffect()
{
	for (auto& Effects : m_arrLensEffect)
	{
		Effects.clear();
	}
}

void CCamera_FPS::RedScreen(_float fDuration, const _float4& vColor)
{
	m_arrLensEffect[LE_REDSCREEN].push_back({ fDuration,vColor,fDuration });
}

void CCamera_FPS::ColorScreen(_float fDuration, const _float4& vColor)
{
	m_arrLensEffect[LE_COLORSCREEN].push_back({ fDuration,vColor,fDuration });
}

void CCamera_FPS::_Shake(_float fTimeDelta)
{
	auto& effectVec = m_arrEffect[CE_SHAKE];
	
	auto iter = effectVec.begin();
	if (iter == effectVec.end())
		return;

	_matrix Shake = XMMatrixTranslation(fRand(-iter->fPower, iter->fPower), fRand(-iter->fPower, iter->fPower), 0.f);
	XMStoreFloat4x4(&m_EffectOffset, XMLoadFloat4x4(&m_EffectOffset) * Shake);

	if (iter->fTimer <= 0.f)
	{
		iter = effectVec.erase(iter);
		return;
	}
	iter->fTimer -= fTimeDelta;

}

void CCamera_FPS::_Dissolve(_float fTimeDelta)
{
	auto& effectVec = m_arrEffect[CE_DISSOLVE];

	auto iter = effectVec.begin();
	if (iter == effectVec.end())
		return;

	_float fDissolve = iter->fTimer / iter->fDuration;
	//if (iter->fTimer > iter->fDuration * 0.5f)
	//{
	//	fDissolve = 2.f - iter->fTimer / iter->fDuration * 2.f;
	//}
	//else
	//{
	//	fDissolve = iter->fTimer / iter->fDuration * 2.f;
	//}

	m_pGameInstance->Dissolve_On(DEFAULTNOISE, fDissolve, iter->fPower);

	if (iter->fTimer <= 0.f)
	{
		iter = effectVec.erase(iter);
		return;
	}
	iter->fTimer -= fTimeDelta;
}

void CCamera_FPS::_LensEffect(_float fTimeDelta)
{
	for (_uint i = 0; i < LE_END; ++i)
	{
		auto& effectVec = m_arrLensEffect[i];
		auto iter = effectVec.begin();
		if (iter == effectVec.end())
			continue;


		if (iter->fTimer <= 0.f)
		{
			iter = effectVec.erase(iter);
			if(iter != effectVec.end())
				m_bLensEffect[i] = true;
			continue;
		}
		iter->fTimer -= fTimeDelta;
		m_bLensEffect[i] = true;
	}

}

CCamera_FPS* CCamera_FPS::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_FPS* pInstance = new CCamera_FPS(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera_FPS"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CCamera_FPS::Clone(void* pArg)
{
	CCamera_FPS* pInstance = new CCamera_FPS(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCamera_FPS"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CCamera_FPS::Free()
{
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pRedScreenTexture);
	Safe_Release(m_pWhiteTexture);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pLensBuffer);
	__super::Free();

}
