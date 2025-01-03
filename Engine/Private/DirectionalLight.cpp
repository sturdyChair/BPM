#include "DirectionalLight.h"

CDirectionalLight::CDirectionalLight() :
    CLight()
{
}

HRESULT CDirectionalLight::Initialize(const DirectionalLight& LightDesc)
{
    m_DirectionalLight = LightDesc;
    m_bActive = true;
    return S_OK;
}

CDirectionalLight* CDirectionalLight::Create(const DirectionalLight& LightDesc)
{
    auto pInstance = new CDirectionalLight;
    if (FAILED(pInstance->Initialize(LightDesc)))
    {
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDirectionalLight::Free()
{
    __super::Free();
}
