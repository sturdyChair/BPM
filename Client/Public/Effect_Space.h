#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
END

BEGIN(Client)
class CEffect_Space :public CEffectObject
{
private:
    CEffect_Space(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Space(const CEffect_Space& rhs);
    virtual ~CEffect_Space() = default;
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

    _float4x4 m_TransformMat;

    _uint m_iLifeTime = 0;
    _uint m_iLifeDuration = 16;
    _uint m_iPrevBeat = 0;
    _float m_fScale = 1.f;

public:
    static  CEffect_Space* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
