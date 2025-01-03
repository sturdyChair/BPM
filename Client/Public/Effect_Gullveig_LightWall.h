
#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
END

BEGIN(Client)
class CEffect_Gullveig_LightWall :public CEffectObject
{
public:
    struct RF_DESC
    {
        _float fLifeTime = 0.f;

    };

private:
    CEffect_Gullveig_LightWall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Gullveig_LightWall(const CEffect_Gullveig_LightWall& rhs);
    virtual ~CEffect_Gullveig_LightWall() = default;
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

    _float4x4* m_pParentMat;
    _float4x4 m_TransformMat;
    _float m_fLifeTime = 0.1f;
    _float m_fLifeDuration = 0.f;
    _float4 m_vColor{};


    _uint m_iPrevCount = 0;
    _uint m_iCurrCount = 0;
    _uint m_iRotateCount = 0;
    _float m_fDissolve = 0.f;
    _float m_fAngle = 0.f;
public:
    static  CEffect_Gullveig_LightWall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
