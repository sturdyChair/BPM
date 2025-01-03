
#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
END

BEGIN(Client)
class CEffect_GideonBlackHole :public CEffectObject
{
public:
    struct RF_DESC
    {
        _float fLifeTime = 0.f;

    };

private:
    CEffect_GideonBlackHole(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_GideonBlackHole(const CEffect_GideonBlackHole& rhs);
    virtual ~CEffect_GideonBlackHole() = default;
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

    CShader* m_pShaderCom = nullptr;
    CModel* m_pModelCom = nullptr;

    _uint m_iPrevCounter = 0;
    _uint m_iCurrCounter = 0;
    _float4x4* m_pParentMat;
    _float4x4 m_TransformMat;

    _int m_iLifeTime = 20;
    
    _float4 m_vColor{};
    _wstring m_strPointLightTag = L"BlackHolePoint";
    _float4x4 m_MatWorld;
    _bool    m_bOdd = false;
public:
    static  CEffect_GideonBlackHole* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
