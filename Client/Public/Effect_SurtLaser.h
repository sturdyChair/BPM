
#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_RectNorm;
class CModel;
class CShader;
class CTexture;

END

BEGIN(Client)
class CParticle;
class CPlayer_Manager;
class CEffect_SurtLaser :public CEffectObject
{
public:
    struct PROJECTILE_DESC
    {
        _float3 vDirection;
        COLLISION_GROUP eCG;
    };

private:
    CEffect_SurtLaser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_SurtLaser(const CEffect_SurtLaser& rhs);
    virtual ~CEffect_SurtLaser() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();
    virtual void Push_From_Pool(void* pArg) override;
    virtual void Dying() override;

    virtual void Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit) override;

private:
    CVIBuffer_RectNorm* m_pVICom = nullptr;
    CShader* m_pShaderCom = nullptr;

    CTexture* m_pDiffuseTextureCom = nullptr;

    CPlayer_Manager* m_pPlayerManager = nullptr;

    _float4x4 m_OrbMat;

    _float    m_fLifeTime = 3.0f;
    CParticle* m_pParticleTail = nullptr;

    _wstring m_strLightTag = L"";
    static _int iLightIdx;

public:
    static  CEffect_SurtLaser* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
