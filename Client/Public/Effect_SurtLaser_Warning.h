

#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_RectNorm;
class CShader;
class CTexture;
END

BEGIN(Client)
class CEffect_SurtLaser_Warning :public CEffectObject
{
public:
    struct LASER_DESC
    {
        _float     fLifeTime;
        _float4x4* pTarget;
    };

private:
    CEffect_SurtLaser_Warning(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_SurtLaser_Warning(const CEffect_SurtLaser_Warning& rhs);
    virtual ~CEffect_SurtLaser_Warning() = default;
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
    _float4x4* m_pTarget = nullptr;
    _float m_fRandSpeed[10]{};
    _float m_fMaxOffset[10]{};
    _float m_fOffset[10]{};
    _float m_fScaleOffset[10]{};

    _uint m_iLeft = 10;
public:
    static  CEffect_SurtLaser_Warning* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
