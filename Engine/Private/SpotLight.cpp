#include "SpotLight.h"

CSpotLight::CSpotLight() :
    CLight()
{
}

HRESULT CSpotLight::Initialize(const SpotLight& LightDesc)
{
    m_SpotLight = LightDesc;
    m_bActive = true;
    return S_OK;
}

void CSpotLight::Set_Position(const _float3& vPos)
{
    m_SpotLight.position = vPos;
}

void CSpotLight::Set_Position(_fvector vPos)
{
    XMStoreFloat3(&m_SpotLight.position, vPos);
}

void CSpotLight::Set_Direction(const _float3& vDir)
{
    m_SpotLight.direction = vDir;
}

void CSpotLight::Set_Direction(_fvector vDir)
{
    XMStoreFloat3(&m_SpotLight.direction, vDir);
}

void CSpotLight::Set_Color(const _float3& vColor)
{
    m_SpotLight.color = vColor;
}

void CSpotLight::Set_Color(_fvector vColor)
{
    XMStoreFloat3(&m_SpotLight.color, vColor);
}

void CSpotLight::Set_Intensity(_float fIntensity)
{
    m_SpotLight.intensity = fIntensity;
}

CSpotLight* CSpotLight::Create(const SpotLight& LightDesc)
{
    auto pInstance = new CSpotLight;
    if (FAILED(pInstance->Initialize(LightDesc)))
    {
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSpotLight::Free()
{
    __super::Free();
}
