
#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_RectNorm;
class CShader;
class CTexture;
END

BEGIN(Client)
class CEffect_GideonRune :public CEffectObject
{
private:
    CEffect_GideonRune(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_GideonRune(const CEffect_GideonRune& rhs);
    virtual ~CEffect_GideonRune() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();
    virtual void Push_From_Pool(void* pArg) override;
    virtual void Dying() override;
    void Set_Tex(_uint iId) { m_iTex = iId; }

private:
    CVIBuffer_RectNorm* m_pVICom = nullptr;
    CShader* m_pShaderCom = nullptr;

    CTexture* m_pDiffuseTextureCom = nullptr;

    _float4x4* m_pParentMat;
    _float4x4 m_TransformMat;
    _float m_fLifeTime = 0.1f;

    _float3 m_vRandNormal[3]{};

    _float4x4 m_matRand[3]{};
    _float4x4 m_matRotate;
    _float    m_fAngle = 0.f;
    _uint m_iTex = 0;
public:
    static  CEffect_GideonRune* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
