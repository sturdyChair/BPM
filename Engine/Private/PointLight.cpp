#include "PointLight.h"

CPointLight::CPointLight() :
    CLight()
{
}

HRESULT CPointLight::Initialize(const PointLight& LightDesc)
{
    m_PointLight = LightDesc;
    m_bActive = true;
    return S_OK;
}

void CPointLight::Set_Position(const _float3& vPos)
{
    m_PointLight.position = vPos;
}

void CPointLight::Set_Position(_fvector vPos)
{
    XMStoreFloat3(&m_PointLight.position, vPos);
}

void CPointLight::Set_Range(_float fRange)
{
    m_PointLight.range = fRange;
}

void CPointLight::Set_Intensity(_float fRange)
{
    m_PointLight.intensity = fRange;
}

CPointLight* CPointLight::Create(const PointLight& LightDesc)
{
    auto pInstance = new CPointLight;
    if (FAILED(pInstance->Initialize(LightDesc)))
    {
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPointLight::Free()
{
    __super::Free();
}
