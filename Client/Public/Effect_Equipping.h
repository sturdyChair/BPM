
#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_RectNorm;
class CShader;
class CTexture;
END

BEGIN(Client)
class CEffect_Equipping :public CEffectObject
{
public:
    struct CHARGE_DESC
    {
        _float fLifeTime = 0.f;
    };

private:
    CEffect_Equipping(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Equipping(const CEffect_Equipping& rhs);
    virtual ~CEffect_Equipping() = default;
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

    _float4x4 m_TransformMat;
    _float m_fLifeTime = 0.1f;
    _float m_fLifeDuration = 1.f;


public:
    static  CEffect_Equipping* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
