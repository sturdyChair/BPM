

#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_RectNorm;
class CShader;
class CTexture;
END

BEGIN(Client)
class CEffect_ThreeRune :public CEffectObject
{
private:
    CEffect_ThreeRune(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_ThreeRune(const CEffect_ThreeRune& rhs);
    virtual ~CEffect_ThreeRune() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();
    virtual void Push_From_Pool(void* pArg) override;
    virtual void Dying() override;
    void Set_ColorBoost(_float fAlpha) {
            m_fColor = fAlpha;
    }

private:
    CVIBuffer_RectNorm* m_pVICom = nullptr;
    CShader* m_pShaderCom = nullptr;

    CTexture* m_pDiffuseTextureCom = nullptr;

    _float4x4* m_pParentMat;
    _float4x4 m_TransformMat;

    _float3 m_vRandNormal[3]{};

    _float m_fColor = 1.f;
    _float4x4 m_matRand[3]{};
    _float4x4 m_matRotate;
    _float    m_fAngle = 0.f;
    _wstring m_strRuneLight = L"RunePointLight";
    PointLight m_PointLight{};
public:
    static  CEffect_ThreeRune* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
