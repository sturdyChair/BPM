
#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
END

BEGIN(Client)
class CEffect_NidhoggField :public CEffectObject
{
private:
    CEffect_NidhoggField(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_NidhoggField(const CEffect_NidhoggField& rhs);
    virtual ~CEffect_NidhoggField() = default;
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
    _float4 m_vColor = { 1.f,1.f,1.f,0.4f };
   

public:
    static  CEffect_NidhoggField* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
