#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_RectNorm;
class CShader;
class CTexture;
END

BEGIN(Client)
class CEffect_Scorch :public CEffectObject
{
private:
    CEffect_Scorch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Scorch(const CEffect_Scorch& rhs);
    virtual ~CEffect_Scorch() = default;
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

    _float m_fLifeTime = 0.1f;


public:
    static  CEffect_Scorch* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
