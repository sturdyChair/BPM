
#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
END

BEGIN(Client)
class CEffect_PlayerDash :public CEffectObject
{
private:
    CEffect_PlayerDash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_PlayerDash(const CEffect_PlayerDash& rhs);
    virtual ~CEffect_PlayerDash() = default;
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

    _float4x4 m_Projection;
    //_float4x4 m_View;
    _float4x4 m_TransformMat;
    _float m_fLifeTime = 0.1f;
    _int m_iDirection = 0;

public:
    static  CEffect_PlayerDash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
