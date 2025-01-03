


#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
END

BEGIN(Client)
class CEffect_BlockLight :public CEffectObject
{
public:
    struct BLOCK_DESC
    {
        _float fLifeTime = 0.0f;
        _float4 vOrigin;
        _float4 vColor;
        _float  fBlockSize = 0.f;
        _bool  bOdd = 0.f;
    };


private:
    CEffect_BlockLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_BlockLight(const CEffect_BlockLight& rhs);
    virtual ~CEffect_BlockLight() = default;
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

    _float  m_fBlockSize = 0.f;
    _float  m_fLifeTime = 0.0f;
    _float4 m_vOrigin;
    _float4 m_vColor;
    _float  m_fTime = 0.f;
    _bool   m_bOdd = false;
public:
    static CEffect_BlockLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
