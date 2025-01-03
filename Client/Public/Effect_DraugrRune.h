#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_RectNorm;
class CShader;
class CTexture;
END

BEGIN(Client)
class CEffect_DraugrRune :public CEffectObject
{
private:
    CEffect_DraugrRune(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_DraugrRune(const CEffect_DraugrRune& rhs);
    virtual ~CEffect_DraugrRune() = default;
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

    _float4x4* m_pParentMat;
    _float4x4 m_TransformMat;
    _float m_fLifeTime = 0.1f;

    _float3 m_vRandNormal[3]{};

    _float4x4 m_matRand[3]{};
    _float4x4 m_matRotate;
    _float    m_fAngle = 0.f;
public:
    static  CEffect_DraugrRune* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
