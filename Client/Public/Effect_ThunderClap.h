
#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_RectNorm;
class CShader;
class CTexture;
END

BEGIN(Client)
class CEffect_ThunderClap :public CEffectObject
{
public:
    struct LIGHRNIG_DESC
    {
        _float3    vScale;
        _float     fLifeTime;
        _uint       iTex = 100;
    };

private:
    CEffect_ThunderClap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_ThunderClap(const CEffect_ThunderClap& rhs);
    virtual ~CEffect_ThunderClap() = default;
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
    CVIBuffer_RectNorm* m_pVICom = nullptr;
    CShader* m_pShaderCom = nullptr;
    CTexture* m_pDiffuseTextureCom = nullptr;

    _float m_fLifeTime = 0.1f;
    _float m_fLifeDuration = 0.1f;

    _float4x4 m_LocalOffset{};
    _float4x4 m_TransformMat{};
    _uint m_iTexIdx = 0;
    _wstring m_strLightTag;
    _float4 m_vColor;
    static _int iLightIdx;

public:
    static  CEffect_ThunderClap* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
