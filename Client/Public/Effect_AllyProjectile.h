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

class CEffect_AllyProjectile :public CEffectObject
{
public:
    struct PROJECTILE_DESC
    {
        _float3 vDirection;
        COLLISION_GROUP eCG;
    };

private:
    CEffect_AllyProjectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_AllyProjectile(const CEffect_AllyProjectile& rhs);
    virtual ~CEffect_AllyProjectile() = default;
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

    _float    m_fLifeTime = 5.0f;
    _float3    m_vDirection;
    _float3    m_vRight;
    _float3    m_vUp;
    _float m_fGravity = 0.f;
    //_wstring m_strLightTag = L"";
    //static _int iLightIdx;
    CParticle* m_pParticle = nullptr;
    _bool m_bExplode = false;
    _float m_fExplodeDuration = 2.f;

public:
    static  CEffect_AllyProjectile* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
