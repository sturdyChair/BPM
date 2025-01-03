#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
END

BEGIN(Client)
class CEffect_Emission :public CEffectObject
{
public:
    struct EMIT_DESC
    {
        _float4 vColor;
        _float  fLifeTime;
        _float4x4* pTransform;
    };

private:
    CEffect_Emission(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Emission(const CEffect_Emission& rhs);
    virtual ~CEffect_Emission() = default;
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
    CTexture* m_pTextureCom = nullptr;
    _float4 m_vColor{};
    _float m_fLifeTime = 0.1f;
    _float4x4* m_pParent = nullptr;
public:
    static  CEffect_Emission* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
