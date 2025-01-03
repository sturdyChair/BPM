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
class CPlayer_Manager;
class CEffect_VortexFire :public CEffectObject
{
public:
    struct PROJECTILE_DESC
    {
        _float3 vDirection;
        COLLISION_GROUP eCG;
        _int iSpikeLeft;
        _float fLifeTime = 0.f;
    };

private:
    CEffect_VortexFire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_VortexFire(const CEffect_VortexFire& rhs);
    virtual ~CEffect_VortexFire() = default;
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
    CShader* m_pModelShaderCom = nullptr;
    CModel* m_pModelCom = nullptr;

    CPlayer_Manager* m_pPlayerManager = nullptr;

    _float    m_fLifeTime = 1.0f;
    _float    m_fLifeTimeMax = 1.0f;
    _float3    m_vDirection;
    _float     m_fAngle = 0.f;
    _bool   m_bNext = false;
    _int m_iSpikeLeft = 0;
    _float3  m_vOriScale;

public:
    static  CEffect_VortexFire* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
