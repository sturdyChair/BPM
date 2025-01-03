
#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
END

BEGIN(Client)
class CEffect_Sonar :public CEffectObject
{
public:
    struct SONAR_DESC
    {
        _float fLifeTime = 0.0f;
        _float4 vOrigin;
        _float4 vColor;
        _float fSonarSpeed = 0.f;
        _float fSonarInterval = 0.f;
        _float fSonarThickness = 0.f;
    };


private:
    CEffect_Sonar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Sonar(const CEffect_Sonar& rhs);
    virtual ~CEffect_Sonar() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();
    virtual void Push_From_Pool(void* pArg) override;
    virtual void Dying() override;

private:
    CVIBuffer_Rect* m_pVICom = nullptr;
    CShader* m_pShaderCom = nullptr;


    _float  m_fLifeTime = 0.0f;
    _float4 m_vOrigin;
    _float4 m_vColor;
    _float  m_fTime = 0.f;
    _float  m_fSonarSpeed = 0.f;
    _float  m_fSonarInterval = 0.f;
    _float  m_fSonarThickness = 0.f;
public:
    static  CEffect_Sonar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
