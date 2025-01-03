

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
class CRoomManager;
class CParticle;
class CTrail;

class CEffect_GideonSlowProjectile :public CEffectObject
{
public:
    struct PROJECTILE_DESC
    {
        COLLISION_GROUP eCG;
    };

private:
    CEffect_GideonSlowProjectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_GideonSlowProjectile(const CEffect_GideonSlowProjectile& rhs);
    virtual ~CEffect_GideonSlowProjectile() = default;
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

    CRoomManager* m_pRoomManager = nullptr;

    _float4x4 m_matDown;
    CTrail* m_pTrail = nullptr;

    _float    m_fLifeTime = 10.0f;
    _float3    m_vDirection;
    _float3    m_vRight;
    _float3    m_vUp;
    _float m_fGravity = 0.f;
    //_wstring m_strLightTag = L"";
    //static _int iLightIdx;
    CParticle* m_pParticle = nullptr;
    _bool m_bExplode = false;
    _float m_fExplodeDuration = 1.f;

public:
    static  CEffect_GideonSlowProjectile* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
